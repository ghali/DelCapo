// Sherif Ghali

#ifndef PROJECTION_E3_H
#define PROJECTION_E3_H

#include "geometry_E3/point_e3.h"
#include "geometry_E2/point_e2.h"

//----------------E3_Project_on_xy_Begin----------------
template<typename NT>
struct Project_on_xy
{
    Point_E2<NT> operator() (const Point_E3<NT>& p) const
    {
        return Point_E2<NT>(p.x(), p.y());
    }
};
//----------------E3_Project_on_xy_End----------------

#endif // PROJECTION_E3_H
