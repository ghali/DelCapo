// Sherif Ghali

#ifndef TRIANGLE_E2_H
#define TRIANGLE_E2_H

#include "geometry_E2/point_e2.hpp"
#include "geometry_E2/predicates_e2.hpp"
#include "misc/determinant.hpp"

template<typename NT>
class Triangle_E2
{
    Point_E2<NT> p0;
    Point_E2<NT> p1;
    Point_E2<NT> p2;
public:
    Triangle_E2(const Point_E2<NT>& q0, const Point_E2<NT>& q1, const Point_E2<NT>& q2)
    {
        p0 = q0; p1 = q1; p2 = q2;
    }

    Point_E2<NT> P0() const { return p0; }
    Point_E2<NT> P1() const { return p1; }
    Point_E2<NT> P2() const { return p2; }

    NT get_signed_area() const
    {
        NT unity(1.0);
        NT half(1.0/2.0);
        NT area = half * determinant(p0.x(), p0.y(), unity,
                                     p1.x(), p1.y(), unity,
                                     p2.x(), p2.y(), unity);
        return area;
    }
};

#endif // TRIANGLE_E2_H
