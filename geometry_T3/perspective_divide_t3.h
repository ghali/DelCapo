// Sherif Ghali

#ifndef PERSPECTIVE_DIVIDE_T3_H
#define PERSPECTIVE_DIVIDE_T3_H

#include "geometry_T3/point_t3.h"
#include "geometry_E3/point_e3.h"

template<typename NT>
struct Perspective_divide
{
    Point_E3<NT> operator() (const Point_T3<NT>& p) const
    {
        return Point_E3<NT>(p.hx()/p.hw(),
                            p.hy()/p.hw(),
                            p.hz()/p.hw());
    }
};

#endif // PERSPECTIVE_DIVIDE_T3_H
