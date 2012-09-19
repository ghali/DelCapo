// Sherif Ghali

#ifndef PLANE_E3_H
#define PLANE_E3_H

#include "geometry_E3/point_e3.h"
#include "geometry_E3/vector_e3.h"

template<typename T>
class Plane_E3
{
    // a Point_E3<T> P lying on the plane satisfies the equation:
    // P.V + d = 0
    Vector_E3<T> V;
    T d;

public:
    Plane_E3() : V(), d() {}
    Plane_E3(const Vector_E3<T>& V, const T& d) : V(V), d(d) {}

    Plane_E3(const Point_E3& p1,
             const Point_E3& p2,
             const Point_E3& p3)
    {
        Vector_E3 v1 = p2 - p1;
        Vector_E3 v2 = p3 - p2;
        N = cross_product(v1, v2);
        d = dot_product((O - p1), N);
    }
};

#endif // PLANE_E3_H
