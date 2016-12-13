// Sherif Ghali

#ifndef PREDICATES_T3_H
#define PREDICATES_T3_H

#include <iostream>
using namespace std;

#include "misc/orientation.hpp"
#include "misc/determinant.hpp"

#include "geometry_T3/point_t3.hpp"
#include "geometry_T3/plane_t3.hpp"
#include "geometry_T3/line_t3.hpp"

template<typename T>
bool
are_coplanar(const Point_T3<T>& p1,
             const Point_T3<T>& p2,
             const Point_T3<T>& p3,
             const Point_T3<T>& p4)
{
    T d = determinant(p1.hx(), p1.hy(), p1.hz(), p1.hw(),
                      p2.hx(), p2.hy(), p2.hz(), p2.hw(),
                      p3.hx(), p3.hy(), p3.hz(), p3.hw(),
                      p4.hx(), p4.hy(), p4.hz(), p4.hw());

    T zero(static_cast<T>(0));
    return (d==zero);
}

template<typename T>
bool
are_cospherical(const Point_T3<T>& p0,
                const Point_T3<T>& p1,
                const Point_T3<T>& p2,
                const Point_T3<T>& p3,
                const Point_T3<T>& p4)
{
    T unity(static_cast<T>(1));
    T d1 = determinant(p0.x() * p0.x(), p0.x(), p0.y(), p0.z(), unity,
                       p1.x() * p1.x(), p1.x(), p1.y(), p1.z(), unity,
                       p2.x() * p2.x(), p2.x(), p2.y(), p2.z(), unity,
                       p3.x() * p3.x(), p3.x(), p3.y(), p3.z(), unity,
                       p4.x() * p4.x(), p4.x(), p4.y(), p4.z(), unity);

    T zero(static_cast<T>(0));
    return (d1==zero);
}

template<typename T>
Oriented_side
oriented_side(const Plane_T3<T>& pl,
              const Point_T3<T>& P)
{
    T d = inner_product( P.hx(), P.hy(), P.hz(), P.hw(),
                         pl.X(), pl.Y(), pl.Z(), pl.W() );

    return enum_Oriented_side(d);
}

#endif // PREDICATES_T3_H
