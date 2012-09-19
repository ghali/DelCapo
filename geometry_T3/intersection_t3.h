// Sherif Ghali

#ifndef INTERSECTION_T3_H
#define INTERSECTION_T3_H

#include <cassert>

#include "misc/determinant.h"
#include "geometry_T3/point_t3.h"
#include "geometry_T3/segment_t3.h"
#include "geometry_T3/plane_t3.h"
#include "geometry_T3/predicates_t3.h"

template<typename T>
class Point_T3;

template<typename T>
class Plane_T3;

template<typename T>
Point_T3<T>
intersection(const Plane_T3<T>& l1, const Plane_T3<T>& l2, const Plane_T3<T>& l3)
{
    T detx = + determinant(l1.Y(), l1.Z(), l1.W(),
                           l2.Y(), l2.Z(), l2.W(),
                           l3.Y(), l3.Z(), l3.W());
    T dety = - determinant(l1.X(), l1.Z(), l1.W(),
                           l2.X(), l2.Z(), l2.W(),
                           l3.X(), l3.Z(), l3.W());
    T detz = + determinant(l1.X(), l1.Y(), l1.W(),
                           l2.X(), l2.Y(), l2.W(),
                           l3.X(), l3.Y(), l3.W());
    T detw = - determinant(l1.X(), l1.Y(), l1.Z(),
                           l2.X(), l2.Y(), l2.Z(),
                           l3.X(), l3.Y(), l3.Z());

    return Point_T3<T>(detx, dety, detz, detw);
}

template<typename T>
Point_T3<T>
intersection(const Plane_T3<T>& pl, const Segment_T3<T>& seg)
{
    T num = pl.X() * seg.source().hx()
        +   pl.Y() * seg.source().hy()
        +   pl.Z() * seg.source().hz()
        +   pl.W() * seg.source().hw();
    T denom = pl.X() * (seg.source().hx() - seg.target().hx())
        +     pl.Y() * (seg.source().hy() - seg.target().hy())
        +     pl.Z() * (seg.source().hz() - seg.target().hz())
        +     pl.W() * (seg.source().hw() - seg.target().hw());
    T t = num / denom;
    Point_T3<T> intp(seg.source().hx() + t * (seg.target().hx() - seg.source().hx()),
                     seg.source().hy() + t * (seg.target().hy() - seg.source().hy()),
                     seg.source().hz() + t * (seg.target().hz() - seg.source().hz()),
                     seg.source().hw() + t * (seg.target().hw() - seg.source().hw()));
    return intp;
}

template<typename T>
bool
positive_half_space_clip(const Plane_T3<T>& plane, Segment_T3<T>& mySegment)
{
    int sourceSide = oriented_side( plane, mySegment.source() );
    int targetSide = oriented_side( plane, mySegment.target() );

    if(sourceSide != ON_NEGATIVE_SIDE      && targetSide != ON_NEGATIVE_SIDE)
        return true; // no clipping needed: segment is entirely inside

    else if((sourceSide == ON_POSITIVE_SIDE    && targetSide == ON_NEGATIVE_SIDE) ||
            (sourceSide == ON_NEGATIVE_SIDE    && targetSide == ON_POSITIVE_SIDE))
    {
        Point_T3<T> intersectionPoint = intersection(plane, mySegment);

        if(sourceSide == ON_POSITIVE_SIDE)
            mySegment = Segment_T3<T>(mySegment.source(), intersectionPoint);
        else if(targetSide == ON_POSITIVE_SIDE)
            mySegment = Segment_T3<T>(intersectionPoint, mySegment.target());

        return true;
    }
    else
        return false; // mySegment is ON_NEGATIVE_SIDE, possibly ON_ORIENTED_BOUNDARY
}

#endif // INTERSECTION_T3_H
