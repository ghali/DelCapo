// geometry_e2.h

#ifndef GEOMETRY_E2_H
#define GEOMETRY_E2_H

template<typename NT> class Point_E2;
template<typename NT> class Segment_E2;
template<typename NT> class Convex_polygon_E2;

template<typename NT>
class Geometry_E2
{
public:
    typedef Point_E2<NT>           Point;
    typedef Segment_E2<NT>         Hyperplane;
    typedef Convex_polygon_E2<NT>  Convex_polytope;
};

#endif // GEOMETRY_E2_H
