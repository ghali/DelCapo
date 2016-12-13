// Sherif Ghali

#ifndef MAZE_H
#define MAZE_H

// Qt
#include <QtOpenGL>

// STL
#include <map>

// QDelCapo
#include "../types/ptr_types.hpp"

#include "../types/geometry_types.hpp"
#include "../planar_graph/planar_graph.hpp"
class Scene;

// The Maze class acts as a link between the Scene object (along with
// the Critter objects) and the Planar_Graph. While maintaining the
// code, one should make sure that this isolation continues; the Scene
// object does not communicate directly (nor reference) the Planar_Graph.

class Maze
{
public:
    Maze(Scene * s,
         Planar_Graph * pg,
         Segment_E3d & eye_coi);

    void paint(bool is_shadow_pass);
    void paint_normals();
    void paint_graph();
    void paint_extruded_walls(bool is_shadow_pass);

    unsigned int number_of_nodes() const { return PG->number_of_nodes(); }

    My_node_ptr get_first_node() const { return PG->get_first_node(); }
    My_node_ptr get_last_node() const  { return PG->get_last_node(); }
    My_node_ptr get_random_node() const { return PG->get_random_node(); }
    My_node_ptr get_unoccupied_random_node() const;
    void occupy_node_with_intelli(My_node_ptr rn,
                                  Intelligence_ptr ip);
    Intelligence_ptr node_has_intelligence(const My_node_ptr & N);
    void remove_intelligence_from_node(My_node_ptr N);

    My_edge_ptr get_random_outgoing_edge(My_node_ptr N);
    My_edge_ptr get_edge_nearest_to_extension(My_edge_ptr E);

    void list_intelligence_at_nodes() const;

    void add_to_score(int i);

private:
    // The maze needs to know about the scene because the critters are
    // autonomous. When they encounter an Intelligence, they let the
    // maze know, then the maze lets the scene know, and in turn the
    // scene lets QDelCapo_Window know.
    Scene * scene;

    Planar_Graph * PG;

    // The following map makes it possible to determine quickly
    // whether a given node holds an Intelligence (and if it does,
    // a pointer to the object).
    std::map<My_node_ptr, Intelligence_ptr> node_to_intelli;

    GLuint maze_list;
    GLuint maze_shadow_list;

    // GLuint movingTargetTextureName;
};

#endif // MAZE_H
