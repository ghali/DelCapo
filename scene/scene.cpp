// Sherif Ghali

// STL
#include <algorithm>
#include <functional>
#include <cassert>

// Boost
#include <boost/mem_fn.hpp>
#include <boost/bind.hpp>

// QDelCapo
#include "../types/lighting_constants.h"
#include "../types/geometry_types.h"
#include "gldraw_e3.h"
#include "scene.h"
#include "../delcapo/Qdelcapo_window.h"

const MyDouble LENGTH_OF_AXES = 10.0;

const MyDouble VIEWER_BEHIND = 5.0;
const MyDouble VIEWER_ALTITUDE = 5.0;

Scene::Scene(Planar_Graph * pg,
             QDelCapo_Window * myQDelCapo_Window,
             Segment_E3d & eye_coi)
    : myQDelCapo_Window(myQDelCapo_Window),
      display_normals(false),
      display_axes(false)
{
    maze = new Maze(this, pg, eye_coi);
    player = new Player(maze); // The player starts on the last node.

    int p = -1;
    // The prey objects start on the first node.
    const My_node_ptr first_node = maze->get_first_node();
    while(++p != NUMBER_OF_PREY)
    {
        Prey_ptr pp = Prey_ptr( new Prey(maze, first_node, p) );
        prey.push_back(pp);
    }

    assert(NUMBER_OF_INTELLIGENCE <= int(maze->number_of_nodes()));

    int i = -1;
    // Intelligence gizmos are strewn.
    while(++i != NUMBER_OF_INTELLIGENCE)
    {
        // Ensure there is no more than one intelligence on a node.
        My_node_ptr rn = maze->get_unoccupied_random_node();
        Intelligence_ptr ip = Intelligence_ptr( new Intelligence(maze, rn, i) );
        intelligence.push_back(ip);
        maze->occupy_node_with_intelli(rn, ip);
    }
}

Scene::~Scene()
{
    delete player;
    delete maze;

    typedef std::vector<Intelligence_ptr>::iterator II;
    for(II i=intelligence.begin(); i!=intelligence.end(); ++i)
        delete *i;
}

// In Scene::check_collisions() we only check collisions between the
// player and the prey. Because Intelligence objects are always on
// nodes, we do not need to check each prey against each intelligence
// (nor do we need to check collisions between the player and each
// intelligence at each frame). We handle both when a critter crosses
// a node. The relevant functions are:
//     Critter::handle_turn_at_node() = 0 and
//     Critter::check_collision_with_intelligence() = 0.
// Both functions are abstract and are defined in the child classes.

void Scene::check_collisions()
{
    int i = -1;
    while(++i != NUMBER_OF_PREY)
        if(player->is_alive() && prey[i]->is_alive()
           && sqrt(squared_distance(player->get_current_coords(), prey[i]->get_current_coords())) <
           player->get_radius() + prey[i]->get_radius())
        {
            add_to_score(1);    // delegated to myQDelCapo_Window

            Direction_E3d resultant;
            if(player->get_edge()->twin == prey[i]->get_edge())
                resultant = player->get_direction();
            else
                resultant = player->get_direction() + prey[i]->get_direction();
            prey[i]->set_state(KNOCKED_OUT, resultant,
                               player->get_velocity() + prey[i]->get_velocity());
        }
}

// Scene::paint(..) is the main cyclic routine. It is called by
// QDelCapo_Subwindow::paintGL().
// It doesn't perform the painting itself, but delegates to the paint
// functions of the referenced classes.

void
Scene::paint(bool is_in_debug_mode)
{
    using std::for_each;
    using boost::mem_fn;

    glEnable(GL_LIGHTING);

    // Move the critters (player and prey).
    for_each(prey.begin(), prey.end(), mem_fn(&Prey::move));
    player->move();

    // Handle collisions between player and prey.
    // Collisions between critters and intelligence are handled only
    // when crossing nodes in Critter::move().
    check_collisions();

    for_each(intelligence.begin(), intelligence.end(),
             mem_fn(&Intelligence::update));

    // Render scene objects.
    // Bind false (i.e., this is not a shadow pass) to the first
    // argument of function paint().
    for_each(prey.begin(), prey.end(),
             boost::bind(mem_fn(&Prey::paint), _1, false));

    for_each(intelligence.begin(), intelligence.end(),
             boost::bind(mem_fn(&Intelligence::paint), _1, false));

    player->paint(false);
    maze->paint(false); // false: not shadow pass

    // To do: replace fake shadows with stencil shadows.
#if 0
    // Render fake shadows
    GLdraw_E3d::begin_fake_shadows_on_Z_0(LIGHT0_POSITION); {
        maze->paint(true); // is shadow pass
        for_each(prey.begin(), prey.end(),
                 boost::bind(mem_fn(&Prey::paint), _1, true));
        player->paint(true);
    } GLdraw_E3d::end_fake_shadows();

    GLdraw_E3d::begin_fake_shadows_on_Z_0(LIGHT1_POSITION); {
        maze->paint(true); // is shadow pass
        for_each(prey.begin(), prey.end(),
                 boost::bind(mem_fn(&Prey::paint), _1, true));
        player->paint(true);
    } GLdraw_E3d::end_fake_shadows();
#endif

    if(is_in_debug_mode) {
        maze->paint_graph();    // Render graph of maze
    }
    if(display_normals)
        maze->paint_normals();
    if(display_axes)
        GLdraw_E3d::draw_axes(LENGTH_OF_AXES); // Render axis

    GLdraw_E3d::check_errors();
}

void Scene::stop_player()
{
    // player->stop_motion();
}

// If a turn is backwards, it is immediate, but otherwise it is
// scheduled and performed when the player reaches the next node.
// The player object also uses this notification to display the
// direction arrow.

void Scene::notify_player_of_turn(int key)
{
    player->notify_of_turn(key);
}

// Notice that the maze class operates in 2D, which is why we need to
// convert here to 3D.

const Segment_E3d
Scene::get_eye_coi() const
{
    const Point_E3d coi = player->get_current_coords();
    const Vector_E2d V2d = player->get_instantaneous_direction().getVector_E2();
    const Vector_E3d V3d = Two_D_to_Three_D::convert(V2d);
    const Point_E3d eye = coi - V3d * VIEWER_BEHIND + Vector_E3d(0.0, 0.0, VIEWER_ALTITUDE);
    return Segment_E3d(eye, coi);
}

void
Scene::add_to_score(int i)
{
    myQDelCapo_Window->add_to_score(i);
}
