// Sherif Ghali

#ifndef SCENE_H
#define SCENE_H

// QDelCapo
#include "../types/ptr_types.h"

#include "../critters/player.h"
#include "../critters/prey.h"
#include "../critters/intelligence.h"

#include "maze.h"

// The following are defined here rather than in scene.cpp because
// they are needed by prey.cpp.
const int NUMBER_OF_PREY         = 4;
const int NUMBER_OF_INTELLIGENCE = 6;

class QDelCapo_Window;

// Scene captures the maze as well as the critters and intelligences.
// It stores references to these objects.

class Scene
{
public:
    Scene(Planar_Graph * pg,
          QDelCapo_Window * myQDelCapo_Window,
          Segment_E3d & eye_coi);
    virtual ~Scene();

    void paint(bool is_in_debug_mode);

    void check_collisions();

    void notify_player_of_turn(int key);
    void stop_player();

    const Segment_E3d get_eye_coi() const;

    void toggle_displaying_normals() { display_normals = !display_normals; }
    void toggle_displaying_axes() { display_axes = !display_axes; }

    void add_to_score(int i);

private:
    QDelCapo_Window * myQDelCapo_Window;

    Maze                                * maze;
    Player                            * player;
    std::vector<Prey_ptr>                 prey;
    std::vector<Intelligence_ptr> intelligence;

    bool display_normals;
    bool display_axes;
};

#endif // SCENE_H
