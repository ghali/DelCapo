// Sherif Ghali

#ifndef PLANAR_GRAPH_H
#define PLANAR_GRAPH_H

#include <vector>
#include <iostream>

// HEDS & Delaunay
#include "../types/geometry_types.h"
#include "heds_types.h"
#include "../delaunay/delaunay.h"

// The Moc in Qt 4.7 or 4.8 cannot handle some Boost 1.48 header.
// g++ and llvm-g++ choke with:
//     [Parse error at "BOOST_JOIN"] error
// See
//     http://thread.gmane.org/gmane.comp.lib.boost.user/71406/focus=71459
// for details.
// The following is the easy solution.
#ifndef Q_MOC_RUN
// BGL
#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#endif

using namespace boost;

typedef adjacency_list<listS, vecS, directedS,
                       no_property, property<edge_weight_t, MyDouble> > graph_t;
typedef graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
typedef graph_traits<graph_t>::edge_descriptor edge_descriptor;
typedef std::pair<int, int> Edge;

// The Planar_Graph acts as a link between the Delaunay triangulation
// class, which generates the random maze, and the Maze class.

class Planar_Graph
{
    friend class Maze;
public:
    Planar_Graph() {}
    Planar_Graph(const Vector_E2i & vertex_grid, MyDouble half_width);

    int
    find_nearest(const Point_E2d & mouse);
    void
    draw(bool track_mouse, const Point_E2d & mouse, bool reveal_tree);

    unsigned int number_of_nodes() const { return G.number_of_nodes(); }

    int get_first_node_index() const { return 0; }
    int get_last_node_index() const { return G.V.size() - 1; }

    My_node_ptr get_first_node() const { return *(G.V.begin());  }
    My_node_ptr get_last_node() const  { return *(G.V.rbegin()); }
    My_node_ptr get_random_node() const;

    Point_E2d
    get_coords(int i) const { return G.V[i]->info().coords; }

    Vector_E2i get_vertex_grid() const { return vertex_grid; }

private:
    typedef std::vector<Point_E2d>::iterator My_point_E2_input_iterator;
    typedef Delaunay< My_point_E2_input_iterator > Delaunay_E2d;

    std::vector<Point_E2d> generate_maze_vertices();
    void convert_delaunay_to_maze(const Delaunay<My_point_E2_input_iterator>& DT);

    void generate_graph_lines();
    void generate_maze_walls(MyDouble half_width);
    void build_boost_graph();

    Vector_E2i vertex_grid;

    Delaunay_E2d DT;

    std::vector<Segment_E2d> graph_segments;

    My_heds G;

    std::vector<Polygon_E2d> polygons;

    std::vector<int> predecessor;
};

#endif // PLANAR_GRAPH_H
