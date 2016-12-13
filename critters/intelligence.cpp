// Sherif Ghali

#include "intelligence.hpp"
#include "scene.hpp"              // included because NUMBER_OF_INTELLIGENCE is needed

#include "../misc/random_e3.hpp"
#include "../critters/two_d_to_three_d.hpp"
#include "../opengl/gldraw_e3.hpp"

// Constants
const GLfloat intelli_diffuse[] = { 1.0, 1.0, 0.0, 1.0,
                                    0.0, 1.0, 1.0, 1.0,
                                    1.0, 0.0, 1.0, 1.0 };

const MyDouble intelli_radius = 0.10;

const int INTELLIGENCE_DEATH_TIME = 8000; // QTime::elapsed() returns milliseconds.

// Bug: The cubes are ugly. Design something better.

Intelligence::Intelligence(Maze * m,
                           My_node_ptr set_node,
                           int intelli_number)
    : maze(m),
      qtime(/* h: */ 0, /* m: */ 0)
{
    initialize_to_alive(set_node);
    GLfloat const * const color = intelli_diffuse + (intelli_number %  NUMBER_OF_INTELLIGENCE) * 4;
    intelli_list = glGenLists(1);

    glNewList(intelli_list, GL_COMPILE); {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, color );
        glRotated(30.0, 0.0, 1.0, 0.0);
        GLdraw_E3d::gen_cube(intelli_radius, 1,1);
        // GLdraw_E3d::gen_Z_cone(0.1, 0.3, 12, 6);
    } glEndList();

    set_to_random(tumble);
}

// Intelligence::initialize_to_alive() is called by only
// Intelligence::Intelligence() and Intelligence::update().

void
Intelligence::initialize_to_alive(My_node_ptr set_node)
{
    state = ALIVE;
    node = set_node;
    coords = Two_D_to_Three_D::convert(node->info().coords);
    qtime.restart();
}

// Intelligence::initialize_to_eaten() is called by
// Player::check_collision_with_intelligence() and
// Prey::check_collision_with_intelligence().

void
Intelligence::initialize_to_eaten()
{
    state = EATEN;
    maze->remove_intelligence_from_node(node);
    qtime.restart();
}

// Intelligence::update() is called by Scene::paint()

void
Intelligence::update()
{
    switch(state)
    {
    case ALIVE:
        break;
    case EATEN:
        if(qtime.elapsed() > INTELLIGENCE_DEATH_TIME) {
            initialize_to_alive( maze->get_unoccupied_random_node() );
            maze->occupy_node_with_intelli(node, Intelligence_ptr(this));
        }
        break;
    }
}

void
Intelligence::paint(bool)
{
    if(state == ALIVE) {
        MyDouble time = qtime.elapsed() / 1e3;
        glPushMatrix(); {
            glTranslated(0.0, 0.0, intelli_radius/1.0);
            glTranslated(coords.x(), coords.y(), coords.z());
            glRotated(time * 250.0, tumble.x(), tumble.y(), tumble.z());
            glCallList(intelli_list);
        } glPopMatrix();
    }
}
