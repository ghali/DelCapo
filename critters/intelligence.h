// Sherif Ghali

#ifndef INTELLIGENCE_H
#define INTELLIGENCE_H

// Qt
#include <QtOpenGL>
#include <QTime>

#include "../types/heds_types.h"

enum Intelligence_State
{
    ALIVE,
    EATEN
};

class Maze;

// An intelligence object resides at a node in the Planar_Graph.

class Intelligence
{
public:
    Intelligence(Maze * m,
                 My_node_ptr set_node,
                 int intelli_number);

    void initialize_to_alive(My_node_ptr set_node);
    void initialize_to_eaten();

    void update();

    void paint(bool shadow_pass);

protected:
    Maze             * maze;
    My_node_ptr        node;
    QTime              qtime;
    Intelligence_State state;
    Point_E3d          coords;
    GLuint             intelli_list;
    Direction_E3d      tumble; // Unlike critters, intelligence always tumbles.
};

#endif // INTELLIGENCE_H
