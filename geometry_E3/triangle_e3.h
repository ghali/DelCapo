// Sherif Ghali

#ifndef TRIANGLE_E3_H
#define TRIANGLE_E3_H

#include "geometry_E3/point_e3.h"

template<typename T>
class Triangle_E3
{
    Point_E3<T> p0;
    Point_E3<T> p1;
    Point_E3<T> p2;
public:
    Triangle_E3(const Point_E3<T>& p0, const Point_E3<T>& p1, const Point_E3<T>& p2) :
        p0(p0), p1(p1), p2(p2)
    {}

    Point_E3<T> P0() const { return p0; }
    Point_E3<T> P1() const { return p1; }
    Point_E3<T> P2() const { return p2; }

};

#endif // TRIANGLE_E3_H
