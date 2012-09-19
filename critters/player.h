// Sherif Ghali

#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>

#include "../types/geometry_types.h"
#include "critter.h"

class Maze;

// The Player class is a sibling of the Prey, but the logic of
// turning, handling collisions, and the implementation of the
// intention direction distinguish Player from Prey. (In addition, of
// course, to replying to user events.

class Player : public Critter
{
    Q_OBJECT

public:
    Player(Maze * m);
    virtual ~Player();

    void notify_of_turn(int key);
    
    // class Prey (also a child of Critter) needs My_node_ptr
    virtual void handle_turn_at_node(const My_node_ptr &);
    virtual void check_collision_with_intelligence(const My_node_ptr & N);

    virtual void frame_by_frame() {} // used in Prey only.

    virtual void paint(bool is_shadow_pass);

public slots:
    void stop_showing_intention_direction();

protected:
    // GUI:
    QTimer     * intention_arrow_timer;
    bool         intention_arrow_show;
    GLuint       intention_arrow_list;
    MyDouble     intended_direction_Z_angle_in_degrees;
    My_edge_ptr  intended_edge;
};

#endif // PLAYER_H
