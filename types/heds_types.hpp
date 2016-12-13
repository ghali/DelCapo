// Sherif Ghali

#ifndef HEDS_TYPES_H
#define HEDS_TYPES_H

// STL/Boost
#include "boost/shared_ptr.hpp"

// HEDS
#include "solid_heds/heds.hpp"
#include "../types/geometry_types.hpp"
#include "../critters/two_d_to_three_d.hpp"

// The parameter classes for the graph are called Junction and Route.
// There is also a myPolygon class that is unused (but needed by the
// HEDS Face).
// 
// Do NOT scatter Node*, Edge*, and Face* thoughout the program.
// Use instead the typedefs below for
//     My_node_ptr,
//     My_edge_ptr,
//     My_face_ptr.
// This way if the shared pointers are later removed, only these
// aliases would need to be modified.

//----------------------------------------------------------------
struct Junction
{
    Point_E2d    coords;
    Junction(const Point_E2d & c) : coords(c) {}
};

struct Route
{
    MyDouble      length;
    Direction_E2d direction;
    MyDouble      z_angle_in_degrees;

    Route(const Point_E2d & source,
          const Point_E2d & target)
    {
        length = std::sqrt(squared_distance(source, target));
        direction = Direction_E2d(source, target);
        z_angle_in_degrees = Two_D_to_Three_D::z_angle_in_radians(direction) * 180.0 / M_PI;
    }
};

struct myPolygon
{};                             // unused

//----------------------------------------------------------------
std::ostream&
operator<< (std::ostream& os, const Junction & J);

std::ostream&
operator<< (std::ostream& os, const Route & R);

std::ostream&
operator<< (std::ostream& os, const myPolygon);


//----------------------------------------------------------------
struct My_heds_traits
{
    typedef Junction    Node_type;
    typedef Route       Edge_type;
    typedef myPolygon   Face_type;
};


typedef HEDS<My_heds_traits>     My_heds;

typedef My_heds::My_node  My_node;
typedef My_heds::My_edge  My_edge;
typedef My_heds::My_face  My_face;

typedef boost::shared_ptr<My_node> My_node_ptr;
typedef boost::shared_ptr<My_edge> My_edge_ptr;
typedef boost::shared_ptr<My_face> My_face_ptr;

typedef My_heds::My_node_container  My_node_container;
typedef My_heds::My_edge_container  My_edge_container;
typedef My_heds::My_face_container  My_face_container;

#endif // HEDS_TYPES_H
