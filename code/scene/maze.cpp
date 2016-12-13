// Sherif Ghali

// STL
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <iterator>
#include <algorithm>

// QDelCapo
#include "../types/number_type.hpp"
#include "../types/geometry_types.hpp"
#include "../types/color_types.hpp"
#include "../types/lighting_constants.hpp"

#include "maze.hpp"
#include "scene.hpp"
#include "../opengl/gldraw_e3.hpp"
#include "../critters/random.hpp"
#include "../critters/two_d_to_three_d.hpp"

// Maze Constants
const GLfloat MAZE_OUTER_WALLS_AND_FLOOR_DIFFUSE[] = { 0.0, 0.7, 0.4, 1.0 };

const GLfloat MAZE_GRAPH_COLOR[] = { 1.0, 0.3, 0.6, 1.0 };
const GLfloat MAZE_NORMALS_COLOR[] = { 1.0, 1.0, 1.0, 1.0 };
const MyDouble MAZE_NORMALS_LENGTH = 0.25;

// const int MAZE_FLOOR_DIVX = 20;
// const int MAZE_FLOOR_DIVY = 20;

const MyDouble MAZE_WALLS_HEIGHT = 0.2;

// We lower the maze floor so that axes, shadows, and the
// maze graph can appear at z=zero.
const MyDouble MAZE_Z_EPSILON_UNDER = -0.025;

// View Vector
Vector_E3d VIEW_VECTOR(0.0, 10.0, -7.0);

Maze::Maze(Scene * s,
           Planar_Graph * pg,
           Segment_E3d & eye_coi)
    : scene(s),
      PG(pg)
{
    Point_E3d coi( + (MyDouble(PG->get_vertex_grid().x())-1.0)/2.0, 
                   + (MyDouble(PG->get_vertex_grid().y())-1.0)/2.0,
                   0.0 );
    eye_coi = Segment_E3d(coi - VIEW_VECTOR, coi);

    maze_list = glGenLists(1);
    glNewList(maze_list, GL_COMPILE); {
        paint_extruded_walls(false); // false: not shadow pass
    } glEndList();

    maze_shadow_list = glGenLists(1);
    glNewList(maze_shadow_list, GL_COMPILE); {
        paint_extruded_walls(true); // true: is shadow pass
    } glEndList();
}

// In the absence of an input, the player will take the halfedge Ec
// that is nearest to continue on the same trajectory as the halfedge
// E originally traversed by the player.
// 
// To find Ec, determine when dot_product(E, Ec) is maximum.

My_edge_ptr
Maze::get_edge_nearest_to_extension(My_edge_ptr edge)
{
    MyDouble max_dot_product = -2.0;
    My_edge_ptr max_edge;
    My_edge_ptr exit_edge = edge->twin;
    do {
        MyDouble d = dot_product(edge->info().direction,
                                 exit_edge->info().direction);
        if(d > max_dot_product) {
            max_dot_product = d;
            max_edge = exit_edge;
        }
        exit_edge = exit_edge->pred->twin;
    } while (exit_edge->twin != edge);

    return max_edge;
}

void
Maze::add_to_score(int i)
{
    scene->add_to_score(i);
}

My_edge_ptr
Maze::get_random_outgoing_edge(My_node_ptr N)
{
    const int n = N->valence();
    const int r = get_int_random<int>(n);
    return N->get_ccw_nth_outgoing_edge(r); // first edge is #0
}

void
Maze::paint(bool is_shadow_pass)
{
    if(!is_shadow_pass) {
        glEnable(GL_LIGHTING);
        glMaterialfv(GL_FRONT, GL_SPECULAR, SCENE_SPECULAR);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, MAZE_OUTER_WALLS_AND_FLOOR_DIFFUSE);
        glMaterialfv(GL_FRONT, GL_SHININESS, SCENE_SHININESS);

        glCallList(maze_list);
    }
    else
        glCallList(maze_shadow_list);
}

void
Maze::paint_extruded_walls(bool is_shadow_pass)
{
    typedef std::vector<Polygon_E2d>::const_iterator PI;
    typedef Polygon_E2d::Point_const_iterator PntIt;
    typedef Polygon_E2d::Point_const_reverse_iterator PntRIt;

    // Use a 1/4 turn cw rotation to determine normals.
    const Transformation_E2d cw_quarter_turn(ROTATION, - M_PI/2.0);

    // We extrude each polygon on the fly to build the "walls".
    for(PI poly=PG->polygons.begin(); poly!=PG->polygons.end(); ++poly) {
        unsigned int sides = poly->size();
        if(sides == 3 || !is_shadow_pass) {
            // Draw the maze walls.
            glBegin(GL_QUADS); {
                // Get last vertex
                Point_E2d lastpnt = *(poly->rbegin());
                for(PntIt pnt=poly->begin(); pnt!=poly->end(); ++pnt) {
                    // A polygon's vertices are oriented counter-clockwise.
                    const Segment_E2d S(lastpnt, *pnt);
                    const Direction_E2d N = Direction_E2d( cw_quarter_turn(S) );
                    glNormal3f(N.x(), N.y(), 0.0);
                    glVertex3d(lastpnt.x(), lastpnt.y(), MAZE_WALLS_HEIGHT);
                    glVertex3d(lastpnt.x(), lastpnt.y(), MAZE_Z_EPSILON_UNDER);
                    glVertex3d(pnt->x(), pnt->y(), MAZE_Z_EPSILON_UNDER);
                    glVertex3d(pnt->x(), pnt->y(), MAZE_WALLS_HEIGHT);
                    lastpnt = *pnt;
                }
            } glEnd();

            // Draw the polygons of the planar graph. They become the maze
            // floor (n!=3) and the top of the maze polygons (n==3).
            glBegin(GL_POLYGON); {
                glNormal3f(0.0, 0.0, 1.0);
                if(sides == 3)
                    for(PntIt pnt=poly->begin(); pnt!=poly->end(); ++pnt)
                        glVertex3d(pnt->x(), pnt->y(), MAZE_WALLS_HEIGHT);
                else
                    for(PntRIt pnt=poly->rbegin(); pnt!=poly->rend(); ++pnt)
                        glVertex3d(pnt->x(), pnt->y(), MAZE_Z_EPSILON_UNDER);
            } glEnd();
        }
    }
}

void
Maze::paint_normals()
{
    typedef std::vector<Polygon_E2d>::const_iterator PI;
    typedef Polygon_E2d::Point_const_iterator PntIt;
    typedef Polygon_E2d::Point_const_reverse_iterator PntRIt;

    // Use a 1/4 turn cw rotation to determine normals.
    const Transformation_E2d cw_quarter_turn(ROTATION, - M_PI/2.0);

    glDisable(GL_LIGHTING);
    glColor3fv( MAZE_NORMALS_COLOR );

    for(PI poly=PG->polygons.begin(); poly!=PG->polygons.end(); ++poly) {
        unsigned int sides = poly->size();
        if(sides == 3) {
            // Draw sides of maze walls.
            glBegin(GL_LINES); {
                // Get last vertex
                Point_E2d lastpnt = *(poly->rbegin());
                for(PntIt pnt=poly->begin(); pnt!=poly->end(); ++pnt) {
                    // A polygon's vertices are oriented counter-clockwise.
                    const Segment_E2d S(lastpnt, *pnt);
                    const Direction_E2d N = Direction_E2d( cw_quarter_turn(S) );
                    //const Vector_E3d V = Two_D_to_Three_D::convert(N).getVector_E3();
                    const Vector_E2d V = N.getVector_E2();
                    glVertex3d(pnt->x(), pnt->y(), MAZE_WALLS_HEIGHT);
                    const Point_E2d pn = (*pnt) + MAZE_NORMALS_LENGTH * V;
                    glVertex3d(pn.x(), pn.y(), MAZE_WALLS_HEIGHT);
                    lastpnt = *pnt;
                }
            } glEnd();

            // Draw the floor and maze-ceiling polygons.
            glBegin(GL_LINES); {
                if(sides == 3)
                    for(PntIt pnt=poly->begin(); pnt!=poly->end(); ++pnt) {
                        glVertex3d(pnt->x(), pnt->y(), MAZE_WALLS_HEIGHT);
                        glVertex3d(pnt->x(), pnt->y(), MAZE_WALLS_HEIGHT + MAZE_NORMALS_LENGTH);
                    }
            } glEnd();
        }
    }
}

void
Maze::paint_graph()
{
    // Show Graph
    glDisable(GL_LIGHTING);
    glColor3fv( MAZE_GRAPH_COLOR );

    // Edges
    typedef My_edge_container::const_iterator Edge_iterator;
    glBegin(GL_LINES); {
        for(Edge_iterator eit = PG->heds.E.begin(); eit != PG->heds.E.end(); ++eit) {
            // class Planar_Graph is in E2d, but we draw in E3d.
            Point_E2d s = (*eit)->source->info().coords;
            Point_E2d t = (*eit)->target->info().coords;
            glVertex3d(s.x(), s.y(), 0.0);
            glVertex3d(t.x(), t.y(), 0.0);
        }
    } glEnd();

    // Nodes
    typedef My_node_container::const_iterator Node_iterator;
    glPointSize( 7.0 );
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS); {
        for(Node_iterator nit = PG->heds.V.begin(); nit != PG->heds.V.end(); ++nit) {
            // Here also: class Planar_Graph is in E2d, but we draw in E3d.
            Point_E2d i = (*nit)->info().coords;
            glVertex3d(i.x(), i.y(), 0.0);
        }
    } glEnd();
    glEnable(GL_LIGHTING);
}

My_node_ptr
Maze::get_unoccupied_random_node() const
{
    My_node_ptr rn;
    do {
        rn = get_random_node();
    } while(node_to_intelli.find(rn) != node_to_intelli.end());
    return rn;
}

void
Maze::occupy_node_with_intelli(My_node_ptr rn,
                               Intelligence_ptr ip)
{
    node_to_intelli[rn] = ip;    
}

Intelligence_ptr
Maze::node_has_intelligence(const My_node_ptr & N)
{
    if(node_to_intelli.find(N) == node_to_intelli.end())
        return Intelligence_ptr(); // NULL shared pointer
    else
        return node_to_intelli[N];        
}

void
Maze::remove_intelligence_from_node(My_node_ptr N)
{
    node_to_intelli.erase(N);
}

void
Maze::list_intelligence_at_nodes() const
{
    typedef std::map<My_node_ptr, Intelligence_ptr>::const_iterator MapIt;
    for(MapIt i = node_to_intelli.begin(); i != node_to_intelli.end(); ++i)
        cout << "node: " << i->first << "  intelligence: " << i->second << endl;
}
