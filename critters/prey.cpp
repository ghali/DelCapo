// Sherif Ghali

#include <QtOpenGL>

#include "prey.hpp"
#include "scene.hpp"              // included because NUMBER_OF_PREY is needed
#include "maze.hpp"
#include "../types/lighting_constants.hpp"
#include "../opengl/gldraw_e3.hpp"

// Constants
const GLfloat prey_diffuse[] = { 1.0, 0.0, 0.0, 1.0,
                                 0.0, 1.0, 0.0, 1.0,
                                 0.0, 0.0, 1.0, 1.0 };
//    distance unit / second
//const MyDouble prey_velocity = 1.5;  // normal game
const MyDouble prey_velocity = 0.75; // slow game
//const MyDouble prey_velocity = 0.35; // very slow
//const MyDouble prey_velocity = 0.12; // ultra slow

const MyDouble prey_angular_velocity = 0.075; // radians / second -- normal
//const MyDouble prey_angular_velocity = 0.035; // radians / second -- slow
//const MyDouble prey_angular_velocity = 0.015; // radians / second -- ultra slow

// const int prey_divisions = 16;
// const MyDouble prey_half_height = 0.2;
const MyDouble prey_radius = 0.2;

const int PREY_IS_INTELLIGENT_DURATION = 10000; // QTime::elapsed() returns milliseconds.
const MyDouble PREY_INTELLIGENCE_RAISE = 0.2;

// Each prey needs to know its prey_number to set the material
// appropriately.
// 
// The prey_number is also used to minimize the number of Prey objects
// that leave the start_node via the same edge.
// 
// Bug: The Prey is ugly. Design something better.

Prey::Prey(Maze * m,
           My_node_ptr start_node,
           int prey_number)
    : Critter(m,
              start_node->get_ccw_nth_outgoing_edge(prey_number),
              prey_velocity,
              prey_angular_velocity,
              prey_radius),
      is_intelligent(false)
{
    GLfloat const * const color = prey_diffuse + (prey_number % NUMBER_OF_PREY) * 4;

    critter_list = glGenLists(1);
    glNewList(critter_list, GL_COMPILE); {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
        glTranslated(0.0, 0.0, prey_radius/2.0);
        glRotated(90.0, 0.0, 1.0, 0.0);
        GLdraw_E3d::gen_Z_cone(0.1, 0.3,  12, 6);
        glRotated(180.0, 0.0, 1.0, 0.0);
        GLdraw_E3d::gen_Z_cone(0.1, 0.0,  12, 6);
    } glEndList();
}

// Choose a random edge out of the node, but don't backtrack on the
// same edge.

void
Prey::handle_turn_at_node(const My_node_ptr & N)
{
    My_edge_ptr e;
    do {
        e = maze->get_random_outgoing_edge(N);
    } while (e->twin == edge);
    edge = e;
}

// Intelligence has a time limit.

void
Prey::frame_by_frame()
{
    if(prey_is_intelligent_time.elapsed() > PREY_IS_INTELLIGENT_DURATION)
        is_intelligent = false;
}

// We may wish to extract the commonality with Player's function to
// Critter::check_...(). But the two functions are so small it is not
// worth bothering.

void
Prey::check_collision_with_intelligence(const My_node_ptr & N)
{
    Intelligence_ptr ip= maze->node_has_intelligence(N);
    if(ip != NULL)              // not NULL
    {
        ip->initialize_to_eaten(); // See comment for Scene * Maze::scene.
        is_intelligent = true;
        prey_is_intelligent_time.restart();
    }
}

void
Prey::paint(bool is_shadow_pass)
{
    if(is_intelligent) {
        glPushMatrix(); {
            glTranslated(0.0, 0.0, PREY_INTELLIGENCE_RAISE);
            Critter::paint(is_shadow_pass);
        } glPopMatrix();
    }
    else
        Critter::paint(is_shadow_pass);
}
