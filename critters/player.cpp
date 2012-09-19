// Sherif Ghali

#include <QtOpenGL>
#include <QTimer>

#include "player.h"
#include "maze.h"
#include "../types/lighting_constants.h"
#include "../opengl/gldraw_e3.h"
#include "geometry_E2/predicates_e2.h"

#include "two_d_to_three_d.h"

// Colors
const GLfloat PLAYER_BODY_DIFFUSE[] = { 1.0, 1.0, 0.0, 1.0 };
const GLfloat PLAYER_NOSE_DIFFUSE[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat PLAYER_EYE_DIFFUSE[] = { 0.0, 0.0, 0.0, 1.0 };

// Kinetics
//    in distance unit / second
//const MyDouble player_velocity = 2.0; // normal game
const MyDouble player_velocity = 1.0; // slow game
//const MyDouble player_velocity = 0.4; // very slow
//const MyDouble player_velocity = 0.15; // ultra slow

const MyDouble player_angular_velocity = 0.075; // radians / second -- normal
//const MyDouble player_angular_velocity = 0.035; // radians / second -- slow
//const MyDouble player_angular_velocity = 0.015; // radians / second -- ultra slow

const int direction_arrow_duration = 500; // 0.5 seconds -- normal
//const int direction_arrow_duration = 800; // 0.8 seconds -- slow
//const int direction_arrow_duration = 1200; // 0.8 seconds -- ultra slow

// Geometry
const MyDouble player_body_radius = 0.2;
const int      player_body_longitudes = 20;
const int      player_body_latitudes = 16;

const MyDouble nose_base_radius = 0.08;
const MyDouble nose_length = 0.275;
const int      nose_longitudes = 16;
const int      nose_stacks = 5;

const MyDouble eyes_radius = 0.04;
const int      eyes_longitudes = 8;
const int      eyes_latitudes = 8;

const MyDouble direction_arrow_body_radius = 0.08;
const MyDouble direction_arrow_body_height = 1.0;
const int      direction_arrow_body_slices = 16;
const int      direction_arrow_body_stacks = 1;

const MyDouble direction_arrow_head_radius = 0.14;
const MyDouble direction_arrow_head_height = 0.4;
const int      direction_arrow_head_slices = 16;
const int      direction_arrow_head_stacks = 1;

const int direction_arrow_disk_slices = 16;
const int direction_arrow_disk_loops = 1;


// The player starts on the last node and takes a random edge out of
// that node.
// 
// Bug: The Player is ugly. Design something better.

Player::Player(Maze * m)
    : Critter(m,
              m->get_random_outgoing_edge(m->get_last_node()),
              player_velocity,
              player_angular_velocity,
              player_body_radius)
{
    intention_arrow_timer = new QTimer();
    intention_arrow_timer->setSingleShot(true);
    intention_arrow_show = false;

    critter_list = glGenLists(1);
    glNewList(critter_list, GL_COMPILE); {
        glPushMatrix(); {
            glRotated(-90.0, 0.0, 0.0, 1.0);
            glTranslated(0.0, 0.0, player_body_radius);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, PLAYER_BODY_DIFFUSE);
            GLdraw_E3d::gen_Y_sphere(player_body_radius, player_body_longitudes, player_body_latitudes);
            glPushMatrix(); {
                glTranslated(0.0, player_body_radius * 0.975, 0.0);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, PLAYER_NOSE_DIFFUSE);
                glRotated(-90.0, 1.0, 0.0, 0.0);
                // glColor4fv(PLAYER_NOSE_DIFFUSE);
                GLdraw_E3d::gen_Z_cone(nose_base_radius, nose_length, // nose
                                       nose_longitudes, nose_stacks);
                glTranslated(player_body_radius/2.0, 0.0, 1.2 * player_body_radius/2.0);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, PLAYER_EYE_DIFFUSE);
                GLdraw_E3d::gen_Y_sphere(eyes_radius, eyes_longitudes, eyes_latitudes);
                glTranslated(-player_body_radius, 0.0, 0.0);
                GLdraw_E3d::gen_Y_sphere(eyes_radius, eyes_longitudes, eyes_latitudes);
            } glPopMatrix();
        } glPopMatrix();
    } glEndList();

    // Direction Arrow:
    intention_arrow_list = glGenLists(1);
    glNewList(intention_arrow_list, GL_COMPILE); {
        glPushMatrix(); {
            glRotated(90.0, 0.0, 1.0, 0.0);

            glTranslated(0.0, 0.0, direction_arrow_body_radius);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, PLAYER_BODY_DIFFUSE);
                glMaterialfv(GL_FRONT, GL_SPECULAR, SCENE_SPECULAR);
                glMaterialfv(GL_FRONT, GL_SHININESS, SCENE_SHININESS);
                GLdraw_E3d::gen_Z_disk(0.0, direction_arrow_body_radius,
                                       direction_arrow_disk_slices,
                                       direction_arrow_disk_loops,
                                       GLU_INSIDE); // base of arrow
                GLdraw_E3d::gen_Z_cylinder(direction_arrow_body_radius,
                                           direction_arrow_body_radius,
                                           direction_arrow_body_height,
                                           direction_arrow_body_slices,
                                           direction_arrow_body_stacks);
                glTranslated(0.0, 0.0, direction_arrow_body_height);
                GLdraw_E3d::gen_Z_disk(direction_arrow_body_radius,
                                       direction_arrow_head_radius,
                                       direction_arrow_disk_slices,
                                       direction_arrow_disk_loops,
                                       GLU_INSIDE); // arrow head separator
                GLdraw_E3d::gen_Z_cone(direction_arrow_head_radius,
                                       direction_arrow_head_height,
                                       direction_arrow_head_slices,
                                       direction_arrow_head_stacks);
        } glPopMatrix();
    } glEndList();

    intended_edge = edge;
}

Player::~Player()
{
    delete intention_arrow_timer;
}

void
Player::stop_showing_intention_direction()
{
    intention_arrow_show = false;    
}

// Player::notify_of_turn(..) is reached from 
//     QDelCapo_Subwindow::keyPressEvent(..).

void
Player::notify_of_turn(int key)
{
    // No need to test for k; we are only notified if it is one of
    // Qt::Key_Left Qt::Key_Right Qt::Key_Down
    switch(key) {
    case Qt::Key_Left:
    case Qt::Key_Right:
        intention_arrow_timer->start(direction_arrow_duration);
        connect(intention_arrow_timer, SIGNAL(timeout()),
                this,                  SLOT(stop_showing_intention_direction()));
        intention_arrow_show = true;
    }

    switch(key) {
    case Qt::Key_Left:
        if(intended_edge == edge)   // Intention has been previously expressed.
            intended_edge = intended_edge->succ;
        else
            intended_edge = intended_edge->twin->succ;
        break;
    case Qt::Key_Right:
        if(intended_edge == edge)   // Intention has been previously expressed.
            intended_edge = intended_edge->twin->pred->twin;
        else
            intended_edge = intended_edge->pred->twin;
        break;
    case Qt::Key_Down:
        initial_coords = Two_D_to_Three_D::convert(edge->target->info().coords);
        edge = edge->twin;
        //initial_position_along_edge = 0.0;
        //current_position_along_edge = edge->info().length - current_position_along_edge;
        direction          = Two_D_to_Three_D::convert(edge->info().direction);
        current_Z_angle_in_degrees = edge->info().z_angle_in_degrees;

        qtime.restart();
        // qtime = qtime.addMSecs(current_position_along_edge / velocity * 1e3);

        break;
    };

    // An alternative policy is to take a random edge out when reaching a node.
    // edge = get_random_outgoing_edge(N);
    Direction_E3d direction               = Two_D_to_Three_D::convert(intended_edge->info().direction);
    intended_direction_Z_angle_in_degrees = intended_edge->info().z_angle_in_degrees;
}

// This function is called once whenever a node in the maze is
// reached.

void
Player::handle_turn_at_node(const My_node_ptr &)
{
    intention_arrow_timer->stop();
    stop_showing_intention_direction();

    if(intended_edge != edge)   // User made a selection; take it.
        edge = intended_edge;
    else
        intended_edge = edge = maze->get_edge_nearest_to_extension(edge);
}

void
Player::check_collision_with_intelligence(const My_node_ptr & N)
{
    Intelligence_ptr ip= maze->node_has_intelligence(N);
    if(ip != NULL)
    {
        ip->initialize_to_eaten(); // See comment for Scene * Maze::scene.
        maze->add_to_score(1);
    }
}

void
Player::paint(bool is_shadow_pass)
{
    Critter::paint(is_shadow_pass);

    if(intention_arrow_show && !is_shadow_pass)
    {
        Point_E3d t = Two_D_to_Three_D::convert(edge->target->info().coords);
        glPushMatrix(); {
            glTranslated(t.x(), t.y(), t.z());
            glRotated(intended_direction_Z_angle_in_degrees,
                      0.0, 0.0, 1.0);
            glCallList(intention_arrow_list);
        } glPopMatrix();
    }
}
