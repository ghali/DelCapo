// Sherif Ghali

#ifndef CRITTER_H
#define CRITTER_H

#include <QtOpenGL>
#include <QTime>

#include "../types/heds_types.h"

class Maze;

enum Critter_State
{
    ALIVE_AND_MOVING_ON_EDGE,
    ALIVE_AND_TURNING_ON_NODE,
    KNOCKED_OUT,
    DEAD
};

// Critter is the parent class to both the player and the prey.
// 
// Each critter is in one of the three states of Critter_State.
// 
// Because a critter object stores the halfedge along which it
// travels, the direction along a edge is unambiguous.

class Critter : public QObject
{
public:
    Critter(Maze * m,
            My_edge_ptr start_edge,
            MyDouble velocity,
            MyDouble angular_velocity,
            MyDouble radius);
    virtual ~Critter() {}

    void initialize_critter_on_new_edge();
    void initialize_critter_at_start_of_turn();
    void initialize_on_death();

    void move();

    bool is_alive() const;

    virtual void paint(bool is_shadow_pass);

    void
    set_state(Critter_State s,
              const Direction_E3d & resultant,
              MyDouble post_impact_velocity);

    My_edge_ptr   get_edge()              const { return edge; }
    Point_E3d     get_current_coords()    const { return current_coords; }
    Direction_E2d get_instantaneous_direction() const { return instantaneous_direction; }
    Direction_E3d get_direction()         const { return direction; }
    MyDouble      get_radius()            const { return radius; }
    Critter_State get_state()             const { return state; }
    MyDouble      get_velocity()          const { return velocity; }

    virtual void handle_turn_at_node(const My_node_ptr & N) = 0;
    virtual void check_collision_with_intelligence(const My_node_ptr & N) = 0;

    virtual void frame_by_frame() = 0;

protected:
    Maze * maze;

    My_edge_ptr   last_edge;
    My_edge_ptr   edge;                        // new edge (if we're turning)
    ;                                          // or current edge (if we're on it)
    MyDouble      initial_position_along_edge; // measured from source
    MyDouble      current_position_along_edge; // measured from source
    Point_E3d     initial_coords;      // of source

    // We store the direction in 3D, but do not project the direction
    // to get it on 2D. Use instead edge->info().direction.

    Direction_E3d direction;           // of edge

    Point_E3d     current_coords;
    Direction_E2d instantaneous_direction; // Direction_E2d current_direction;
    MyDouble      current_Z_angle_in_degrees;
    bool          rotate_clockwise; // Is shortest turn cw?

    QTime qtime;

    Critter_State state;

    Direction_E3d tumble;       // during KNOCKED_OUT state

    const MyDouble creation_velocity;
    MyDouble velocity;
    MyDouble angular_velocity;
    const MyDouble radius;

    GLuint critter_list;
};

#endif // CRITTER_H
