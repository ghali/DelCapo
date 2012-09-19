// Sherif Ghali

#include <cmath>
#include "critter.h"
#include "../geometry_E2/interpolation_e2.h"
#include "../misc/random_e3.h"
#include "random.h"
#include "maze.h"

#include "two_d_to_three_d.h"

const MyDouble KNOCKING_OUT_IMPACT_LENGTH = 0.5;
const MyDouble KNOCKING_OUT_LENGTH = 5.0;

// A Critter relives after CRITTER_DEATH_TIME has elapsed.
// 
// Rather than waste time destroying and then constructing a new
// object, we simply change the state of a Critter object to DEAD and
// back.

const int CRITTER_DEATH_TIME = 5000;

// Critters move initially along a node's first edge.

Critter::Critter(Maze * m,
                 My_edge_ptr start_edge,
                 MyDouble creation_velocity,
                 MyDouble creation_angular_velocity,
                 MyDouble radius)
    : maze(m),
      edge(start_edge),
      creation_velocity(creation_velocity),
      velocity(creation_velocity),
      angular_velocity(creation_angular_velocity),
      radius(radius)
{
    qtime = QTime(0,0,0,0);

    state = ALIVE_AND_MOVING_ON_EDGE;

    initialize_critter_on_new_edge();
}

// Set the state, but if the Critter is knocked out, we also determine
// the direction of tumbling.

void
Critter::set_state(Critter_State s,
                   const Direction_E3d & resultant,
                   MyDouble post_impact_velocity)
{
    state = s;

    if(state == KNOCKED_OUT)
    {
        qtime.restart();

        velocity = post_impact_velocity;
        initial_position_along_edge = KNOCKING_OUT_IMPACT_LENGTH;
        initial_coords = current_coords;

        // Rotate resultant off the ground plane.
        Transformation_E3d T1(ROTATION, Z_axis, M_PI/2.0);
        Direction_E3d dh = T1(resultant);
        Transformation_E3d T2(ROTATION, dh, -0.5);
        direction = T2(resultant);

        current_coords = initial_coords;

        set_to_random(tumble);
    }
}

// This function is invoked once when a Critter crosses a Node and
// begins its path on a new Edge.

void
Critter::initialize_critter_on_new_edge()
{
    qtime.start();
    initial_position_along_edge = 0.0;
    current_position_along_edge = 0.0;

    const My_node_ptr N = edge->source;
    initial_coords = Two_D_to_Three_D::convert(N->info().coords);
    direction      = Two_D_to_Three_D::convert(edge->info().direction);

    instantaneous_direction = edge->info().direction;

    current_Z_angle_in_degrees = edge->info().z_angle_in_degrees;

    velocity = creation_velocity;

    current_coords = initial_coords;
}

void
Critter::initialize_critter_at_start_of_turn()
{
    qtime.start();

    const Vector_E2d V1 = last_edge->info().direction.getVector_E2();
    const Vector_E2d V2 =      edge->info().direction.getVector_E2();

    rotate_clockwise = cross_product(V1, V2) > 0.0;
}

void
Critter::initialize_on_death()
{
    qtime.start();
}

// We initialize qtime for each critter on two occasions:
// 1- when a critter leaves a node along an edge, and
// 2- when a critter starts the rotation sequence at a node.
// 
// The first is used to walk at a constant speed along the edge.
// The second is used to rotate at a constant speed at a node.

bool
Critter::is_alive() const
{ 
    return state == ALIVE_AND_MOVING_ON_EDGE || state == ALIVE_AND_TURNING_ON_NODE;
}

// The logic for traversing the Maze is common between the Player and
// the Prey objects. Critter::move() contains this logic.

void
Critter::move()
{
    const MyDouble T = MyDouble(qtime.elapsed()) / 1e3;
    MyDouble cp;
    switch(state)
    {
    case ALIVE_AND_MOVING_ON_EDGE:
        current_position_along_edge = initial_position_along_edge + velocity * T; // scalar
        if(current_position_along_edge < edge->info().length) {
            current_coords = initial_coords + direction * current_position_along_edge; // vector
            frame_by_frame();
        }
        else {
            // Turn at node
            const My_node_ptr N = edge->target;

            last_edge = edge;   // We need last_edge to interpolate turning.

            handle_turn_at_node(N);
            check_collision_with_intelligence(N);
            
            cout << endl << "Crossing node " << N << endl;
            maze->list_intelligence_at_nodes();

            // handle_turn_at_node(N) updated edge; therefore the edge
            // referenced in initialize_critter_on_new_edge() is
            // different from the one just referenced.

            // initialize_critter_on_new_edge();
            initialize_critter_at_start_of_turn();
            state = ALIVE_AND_TURNING_ON_NODE;
        }
        break;                  // We break _out_ of switch statement.
    case ALIVE_AND_TURNING_ON_NODE:
        if(rotate_clockwise) {
            Transformation_E2d rotate_cw =
                Transformation_E2d(ROTATION,
                                   angular_velocity * T);
            instantaneous_direction = rotate_cw(instantaneous_direction);
            frame_by_frame();
        }
        else {
            Transformation_E2d rotate_ccw =
                Transformation_E2d(ROTATION,
                                   - angular_velocity * T);
            instantaneous_direction = rotate_ccw(instantaneous_direction);
            frame_by_frame();
        }

        /*MyDouble*/ cp = cross_product(instantaneous_direction.getVector_E2(),
                                    edge->info().direction.getVector_E2());

        if( (rotate_clockwise && cp > 0.0) || (!rotate_clockwise && cp < 0.0) )
            current_Z_angle_in_degrees = Two_D_to_Three_D::z_angle_in_radians(instantaneous_direction) * 180.0 / M_PI;
        else {
            state = ALIVE_AND_MOVING_ON_EDGE;
            initialize_critter_on_new_edge();
            qtime.start();
        }
        break;
    case KNOCKED_OUT:
        current_position_along_edge = initial_position_along_edge + velocity * T; // scalar
        if(current_position_along_edge < KNOCKING_OUT_LENGTH)
            current_coords = initial_coords + direction * current_position_along_edge; // vector
        else {
            state = DEAD;
            initialize_on_death();
        }
        break;
    case DEAD:
        if(qtime.elapsed() > CRITTER_DEATH_TIME) {
            state = ALIVE_AND_MOVING_ON_EDGE;

            edge = maze->get_random_outgoing_edge( maze->get_random_node() );

            initialize_critter_on_new_edge();
        }
        break;
    }
}

void
Critter::paint(bool)
{
    if(state == DEAD)
        return;
    glPushMatrix(); {
        glTranslated(current_coords.x(), current_coords.y(), current_coords.z());
        if(state == KNOCKED_OUT)
            glRotated(current_position_along_edge * 250.0, tumble.x(), tumble.y(), tumble.z());

        // Critter initially faces +X.
        glRotated(current_Z_angle_in_degrees, 0.0, 0.0, 1.0);
        glCallList(critter_list);
    } glPopMatrix();
}
