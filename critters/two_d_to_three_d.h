// Sherif Ghali

#ifndef TWO_D_TO_THREE_D_H
#define TWO_D_TO_THREE_D_H

#include <cmath>
#include "../types/geometry_types.h"

// The class Two_D_to_Three_D acts as a unified place for converting
// from the Maze and the Planar_Graph's 2D to the 3D expected by the
// other classes.

struct Two_D_to_Three_D
{
    static const Point_E3d
    convert(const Point_E2d & P2d)
    {
        return Point_E3d( P2d.x(), P2d.y(), 0.0 );
    }

    static const Direction_E3d
    convert(const Direction_E2d & D2d)
    {
        return Direction_E3d( D2d.x(), D2d.y(), 0.0 );
    }

    static const Vector_E3d
    convert(const Vector_E2d & V2d)
    {
        return Vector_E3d( V2d.x(), V2d.y(), 0.0 );
    }

    static MyDouble
    z_angle_in_radians(const Direction_E2d & direction)
    {
        return atan2(direction.y(), direction.x());
    }
};

#endif // TWO_D_TO_THREE_D_H
