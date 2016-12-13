// Sherif Ghali

// Qt
#include <QtOpenGL>

// STL
#include <limits>
#include <vector>

// QDelCapo
#include "../critters/random.hpp"
#include "planar_graph.hpp"
#include "../opengl/gldraw_e2.hpp"

const unsigned int MAZE_SEED = 10;
const MyDouble NEARLY_PARALLEL = 0.05;

Planar_Graph::Planar_Graph(const Vector_E2i & vertex_grid,
                           MyDouble half_width)
    : vertex_grid(vertex_grid)
{
    srand(MAZE_SEED);
    std::vector<Point_E2d> points = generate_maze_vertices();
    DT = Delaunay_E2d(points);
    generate_graph_lines();

    convert_delaunay_to_maze(DT);
    generate_maze_walls(half_width);
    build_boost_graph();
}

My_node_ptr
Planar_Graph::get_random_node() const
{
    const int n = heds.V.size();
    const int r = get_int_random<int>(n);
    return heds.V[r];
}

// The vertices satisfy the following properties:
// - The boundary vertices lie on the convex hull.
// - The grid has roughly a triangular pattern.
// - The lengths of the Delaunay edges are not too far apart.

std::vector<Point_E2d>
Planar_Graph::generate_maze_vertices()
{
    std::vector<Point_E2d> points;
    Vector_E2d V;
    const MyDouble length_x = MyDouble(vertex_grid.x() - 1);
    const MyDouble length_y = MyDouble(vertex_grid.y() - 1);

    for(int x=0; x < vertex_grid.x(); ++x)
        for(int y=0; y < vertex_grid.y(); ++y) {

            if((x % 2 == 1 && y % 2 == 1)
               || (x % 2 == 0 && y % 2 == 0))
                continue;

            MyDouble dx = get_real_random( MyDouble(0), MyDouble(0.4) );
            MyDouble dy = get_real_random( MyDouble(0), MyDouble(0.4) );

            if(x==0 || x==vertex_grid.x()-1) {
                dx = fabs((MyDouble)y - length_y/2.0) / length_y; // 0 to 0.5
                dx = dx * dx * dx * 8.0 - 1.0; // -1.0 to 0.0
                if(x==vertex_grid.x()-1)
                    dx = -dx;
                dy = 0;
            }

            if(y==0 || y==vertex_grid.y()-1) {
                dy = fabs((MyDouble)x - length_x/2.0) / length_x; // 0 to 0.5
                dy = dy * dy * dy * 8.0 - 1.0; // -1.0 to 0.0
                if(y==vertex_grid.y()-1)
                    dy = -dy;
                dx = 0;
            }

            V = Vector_E2d(dx,dy);
            Point_E2d P = Point_E2d(x,y) + V;
            points.push_back( P );
        }
    return points;
}

// Setting pred and succ links
// 
// - We create a map M_edges from (i,k) to the actual halfedge created.
// 
// - We know while iterating which edge has just been created, so we can
//   determine the pred link correctly.
// 
// - Because we cannot determine the pred link for the first edge, we do
//   so outside, and just after, the loop.
// 
// - To create the succ links, we use a map M_pred from (i,k.succ) to
//   (i,k) while creating the pred links. We then simply iterate over the
//   entries of M_pred and connect the target with the source.

void
Planar_Graph::convert_delaunay_to_maze(const Delaunay<My_point_E2_input_iterator>& DT)
{
    std::map<int, My_node_ptr>                 M_nodes;
    std::map<std::pair<int,int>, My_edge_ptr>  M_edges;

    //----------------Graph Nodes----------------
    for(int i=0; i<DT.num_points; i++) {

        Point_E2d p = DT.ver[i].p;
        Junction J(p);
        My_node_ptr np(new My_node(J));
        M_nodes[i] = np;
        heds.V.push_back( np );
    }

    //----------------Graph Edges----------------
    for(int i=0; i<DT.num_points; i++) {

        // Create edges and set source and target nodes
        int k;
        const int j = k = DT.first(i);
        if (j != Delaunay<My_point_E2_input_iterator>::NO_EDGE)
            do {
                k = DT.succ(i, k);
                // Create edge i --> k
                My_edge_ptr eptr( new My_edge(M_nodes[i],
                                              M_nodes[k],
                                              Route(M_nodes[i]->info().coords,
                                                    M_nodes[k]->info().coords)) );
                heds.E.push_back( eptr );
                M_edges[std::make_pair(i,k)] = eptr;
            } while (k != j);
    }

    // Because we use (k2, i) rather than (i, k2), we could delay
    // stitching twins until after pred and succ links are set.
    heds.stitch_twins();
    heds.set_node_outgoing_edge();

    //----------------pred and succ links----------------
    for(int i=0; i<DT.num_points; i++) {

        int k = DT.first(i);
        const int j = k = DT.first(i);
        if (j != Delaunay<My_point_E2_input_iterator>::NO_EDGE)
            do {
                // k1 and k2 are adjacent around i.
                My_edge_ptr eptr_i_k1 = M_edges[std::make_pair(i, k)];
                k = DT.succ(i, k);
                My_edge_ptr eptr_k2_i = M_edges[std::make_pair(k, i)];

                eptr_k2_i->succ = eptr_i_k1;
                eptr_i_k1->pred = eptr_k2_i;
            } while (k != j);
    }

    //----------------Graph Faces----------------
    typedef My_edge_container::const_iterator  EdgeP_iterator;
    for(EdgeP_iterator ei = heds.E.begin(); ei != heds.E.end(); ++ei)
        (*ei)->adjFace = My_face_ptr();
    assert( heds.number_of_faces() == 0);
    for(EdgeP_iterator eip = heds.E.begin(); eip != heds.E.end(); ++eip) {
        const My_edge_ptr ei = (*eip);
        if( ei->adjFace.get() != NULL )
            continue;           // We have already found this halfedge's adjFace.
        My_face_ptr fptr( new My_face(ei, myPolygon()) );
        heds.F.push_back( fptr );
        My_edge_ptr ej = ei->succ;
        while( ej != ei ) {
            ej->adjFace = fptr;
            ej = ej->succ;
        }
    }
}

void
Planar_Graph::generate_graph_lines()
{
    graph_segments.clear();
    for(int i=0; i<DT.num_points; i++)
    {
        int k;
        const int j = k = DT.first(i);
        if (j != Delaunay_E2d::NO_EDGE)
            do {
                k = DT.succ(i, k);
                // Create edge i --> k
                if(i<k) {
                    const Point_E2d pi = DT.ver[i].p;
                    const Point_E2d pk = DT.ver[k].p;
                    graph_segments.push_back(Segment_E2d(pi, pk));
                }
            } while (k != j);
    }
}

// - Create a map M_int from (i, k) to a sequential number N around each
//   vertex.
// 
// - Create a map M_edge from (i, k, N) to the corresponding halfedge.
// 
// - For each entry in M_int
// 	- Link the edge (i, k, N) with the edges (i, k, N +/- 1)


void
Planar_Graph::generate_maze_walls(MyDouble half_width)
{
    polygons.clear();
    typedef My_face_container::const_iterator  FaceP_iterator;
    for(FaceP_iterator fi = heds.F.begin(); fi != heds.F.end(); ++fi) {

        const My_edge_ptr ei = (*fi)->edge;
        My_edge_ptr e1 = ei;
        My_edge_ptr e2 = e1->succ; // Iterate counter-clockwise around face.
        Polygon_E2d P;
        P.clear();
        do {
            // A coordinate-free formulation for intersection by
            // applying a 1/4 turn rotation.
            const Transformation_E2d quarter_turn(ROTATION, M_PI/2.0);

            const Direction_E2d D1 = e1->info().direction;
            const Direction_E2d D2 = e2->info().direction;

            Point_E2d p;
            // Make sure we don't intersect nearly parallel lines.
            if(fabs(cross_product(D1, D2)) < NEARLY_PARALLEL)
            {
                const Point_E2d q(e1->target->info().coords);
                const Vector_E2d v1(D1.getVector_E2());
                const Vector_E2d v2(D2.getVector_E2());
                const Vector_E2d v = Direction_E2d(v1 + v2).getVector_E2();

                const Vector_E2d vr_scaled = quarter_turn(v) * (half_width);
                const Transformation_E2d translate(TRANSLATION, vr_scaled);
                p = translate(q);
            }
            else
            {
                Segment_E2d s1(e1->source->info().coords,
                               e1->target->info().coords);
                Segment_E2d s2(e2->source->info().coords,
                               e2->target->info().coords);

                const Direction_E2d d1 = Direction_E2d( quarter_turn(s1) );
                const Vector_E2d v1 = d1.getVector_E2() * half_width;
                const Transformation_E2d translate1(TRANSLATION, v1);
                s1 = translate1(s1);

                const Direction_E2d d2 = Direction_E2d( quarter_turn(s2) );
                const Vector_E2d v2 = d2.getVector_E2() * half_width;
                const Transformation_E2d translate2(TRANSLATION, v2);
                s2 = translate2(s2);
                p = intersection_of_lines(s1, s2);
            }
            P.push_back( p );

            e1 = e2;
            e2 = e2->succ;
        } while( e1 != ei );
        polygons.push_back(P);
    }
}

void
Planar_Graph::build_boost_graph()
{
    const int num_nodes = heds.V.size();
    std::vector<Point_E2d> nodes;
    nodes.reserve(num_nodes);
    std::map<My_node_ptr,int> M_nodes;
    for(int v=0; v!=num_nodes; ++v) {
        My_node_ptr mnp = heds.V[v];
        nodes.push_back(mnp->info().coords);
        M_nodes[mnp] = v;
    }

    const int num_edges = heds.E.size();
    std::vector<Edge> my_edges;
    my_edges.reserve(num_edges);
    MyDouble * weights = new MyDouble[num_edges];
    for(int e=0; e!=num_edges; ++e) {
        My_edge_ptr mep = heds.E[e];
        My_node_ptr s = mep->source;
        My_node_ptr t = mep->target;
        my_edges.push_back( Edge( M_nodes[s], M_nodes[t] ) );
        weights[e] = std::sqrt(squared_distance(s->info().coords, t->info().coords));
    }

    graph_t BG(my_edges.begin(), my_edges.end(), weights, num_nodes);
    std::vector<vertex_descriptor> p;
    p.reserve(num_vertices(BG));
    std::vector<MyDouble> d;
    d.reserve(num_vertices(BG));
    vertex_descriptor s = vertex(0 /*first vertex*/, BG);

    dijkstra_shortest_paths(BG, s, boost::predecessor_map(&p[0]).distance_map(&d[0]));

    predecessor.clear();
    boost::graph_traits < graph_t >::vertex_iterator vi, vend;
    for (boost::tie(vi, vend) = vertices(BG); vi != vend; ++vi)
        predecessor.push_back(p[*vi]);
    delete [] weights;
}

int
Planar_Graph::find_nearest(const Point_E2d & mouse)
{
    MyDouble closest = std::numeric_limits<MyDouble>::max();
    int index = -1;
    MyDouble d;
    for(unsigned v=0; v!=heds.V.size(); ++v)
        if((d = squared_distance(mouse, heds.V[v]->info().coords)) < closest) {
            index = v;
            closest = d;
        }
    return index;
}

void
Planar_Graph::draw(bool track_mouse,
                   const Point_E2d & mouse,
                   bool reveal_tree)
{
    // draw delaunay
    GLdraw_E2::draw(graph_segments, Qt::yellow,
                    (graph_segments.size() < 2000) ? 3.0 // avoid clutter
                    : ((graph_segments.size() < 4000) ? 2.0 : 1.0));
    // draw maze polygons
    GLdraw_E2::draw(polygons);

    if(reveal_tree)
        for(unsigned i=0;i<predecessor.size();++i)
            GLdraw_E2::draw(heds.V[i]->info().coords,
                            heds.V[predecessor[i]]->info().coords,
                            Qt::red, 3.0,
                            true, Qt::red, 7.0);
    else if(track_mouse)
    {
        // track
        int j = find_nearest(mouse);
        while(j) {
            int i = j;
            j = predecessor[j];
            GLdraw_E2::draw(heds.V[i]->info().coords,
                            heds.V[j]->info().coords,
                            Qt::red, 3.0,
                            true, Qt::red, 7.0);
        } 
    }
}
