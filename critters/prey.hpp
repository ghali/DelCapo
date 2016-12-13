// Sherif Ghali

#ifndef PREY_H
#define PREY_H

#include "../types/geometry_types.hpp"
#include "critter.hpp"

class Maze;

// The Prey class is a specialization of Critter, a parenthood shared
// with the Player class.

class Prey : public Critter
{
public:
    Prey(Maze * m,
         My_node_ptr start_node,
         int prey_number);
    virtual ~Prey() {}

    virtual void handle_turn_at_node(const My_node_ptr & N);
    virtual void check_collision_with_intelligence(const My_node_ptr & N);

    virtual void frame_by_frame();

    virtual void paint(bool is_shadow_pass);

private:
    bool is_intelligent; // did this prey find an Intelligence object?
    QTime prey_is_intelligent_time; // Intelligence expires.
};

#endif // PREY_H
