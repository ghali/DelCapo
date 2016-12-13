// Sherif Ghali

#ifndef PREDICATES_E2_H
#define PREDICATES_E2_H

#include <iostream>
using namespace std;

#include "geometry_E2/point_e2.hpp"
#include "geometry_E2/segment_e2.hpp"
#include "geometry_E2/line_e2.hpp"
#include "geometry_E2/polygon_e2.hpp"
#include "geometry_E2/dominant_e2.hpp"

#include "misc/orientation.hpp"
#include "misc/determinant.hpp"

template<typename NT>
Oriented_side
oriented_side(const Point_E2<NT>& p1,
              const Point_E2<NT>& p2,
              const Point_E2<NT>& p3)
{
    NT unity(static_cast<NT>(1));
    NT d = determinant<NT>(p1.x(), p1.y(), unity,
                           p2.x(), p2.y(), unity,
                           p3.x(), p3.y(), unity);

    return enum_Oriented_side(d);
}

template<typename NT>
Oriented_side
oriented_side(const Line_E2<NT>& L,
              const Point_E2<NT>& p)
{
    NT d = L.a() * p.x() + L.b() * p.y() + L.c();

    return enum_Oriented_side(d);
}

template<typename NT>
Oriented_side
oriented_side(const Segment_E2<NT>& seg,
              const Point_E2<NT>& p)
{
    return oriented_side<NT>(seg.source(), seg.target(), p);
}

template<typename NT>
bool
are_linearly_dependent(const Vector_E2<NT>& v1,
                       const Vector_E2<NT>& v2)
{
    return (determinant<NT>(v1.x(), v1.y(),
                            v2.x(), v2.y()) == NT(0));
}

template<typename NT>
bool
lex_xy_compare(const Point_E2<NT>& p1,
               const Point_E2<NT>& p2)
{
    return p1.x() < p2.x() || (p1.x() == p2.x() && p1.y() < p2.y());
}

template<typename NT>
bool
are_parallel(const Segment_E2<NT>& seg1,
             const Segment_E2<NT>& seg2)
{
    const Vector_E2<NT> v1 = seg1.get_Vector_E2();
    const Vector_E2<NT> v2 = seg2.get_Vector_E2();
    return are_linearly_dependent<NT>(v1, v2);
}

template<typename NT>
Orientation_to_circle
inside_circle(const Point_E2<NT>& p0,
              const Point_E2<NT>& p1,
              const Point_E2<NT>& p2,
              const Point_E2<NT>& p3)
{
    NT unity(static_cast<NT>(1));
    NT d1 = determinant<NT>(p0.x() * p0.x() + p0.y() * p0.y(), p0.x(), p0.y(), unity,
                            p1.x() * p1.x() + p1.y() * p1.y(), p1.x(), p1.y(), unity,
                            p2.x() * p2.x() + p2.y() * p2.y(), p2.x(), p2.y(), unity,
                            p3.x() * p3.x() + p3.y() * p3.y(), p3.x(), p3.y(), unity);
    NT d2 = determinant<NT>(p1.x(), p1.y(), unity, 
                            p2.x(), p2.y(), unity, 
                            p3.x(), p3.y(), unity);
    NT d = d1 * d2;

    if(d < 0)
        return INSIDE_CIRCLE;
    else if(d > 0)
        return OUTSIDE_CIRCLE;
    else
        return COCIRCULAR;
}

template<typename NT>
bool
is_inside(const Polygon_E2<NT>& polygon,
          const Point_E2<NT>& point)
{
    Point_E2<NT> inf(99999, point.y());
    int count = 0;
    for(unsigned int i=0; i<polygon.vertices.size(); ++i) {
        Point_E2<NT> source(polygon.vertices[i]);
        Point_E2<NT> target(polygon.vertices[(i==polygon.vertices.size()-1) ? 0 : i+1]);

        Oriented_side o1 = oriented_side(source, target, point);
        Oriented_side o2 = oriented_side(source, target, inf);

        Oriented_side o3 = oriented_side(point, inf, source);            
        Oriented_side o4 = oriented_side(point, inf, target);            

        if(o1 * o2 == -1 && o3 * o4 == -1)
            count++;
    }
    return (count%2 == 1);
}

//----------------------------------------------------------------
template<typename NT>
Dominant_E2
dominant(const Segment_E2<NT>& segment)
{
    NT dx = segment.target().x() - segment.source().x();
    NT dy = segment.target().y() - segment.source().y();

    return dominant(dx, dy);
}
//----------------------------------------------------------------

#endif // PREDICATES_E2_H
