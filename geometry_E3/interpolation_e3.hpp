// Sherif Ghali

#ifndef INTERPOLATION_E3_H
#define INTERPOLATION_E3_H

#include "raster_image/array2.hpp"

#include "geometry_E3/point_e3.hpp"
#include "geometry_E3/vector_e3.hpp"
#include "geometry_E3/polygon_e3.hpp"

template<typename NT>
Point_E3<NT>
interpolate(const Point_E3<NT>& P0, const Point_E3<NT>& P1, const NT& t)
{
    return Point_E3<NT>(P0 + (P1-P0) * t);
}

// Notice that we change the semantics for the next two functions.
// The t values are the relative weights of the points, whereas
// previously t was a parameterization of the line segment.
template<typename NT>
Point_E3<NT>
interpolate(const Point_E3<NT>& P0, const NT& t0,
            const Point_E3<NT>& P1, const NT& t1,
            const Point_E3<NT>& P2, const NT& t2)
{
    const Point_E3<NT> Origin(0.0, 0.0, 0.0);
    // The following expression can be optimized by ignoring, say, t2.
    return Origin + ((P0-Origin) * t0 + (P1-Origin) * t1 + (P2-Origin) * t2);
}

template<typename NT>
Point_E3<NT>
interpolate(const Point_E3<NT>* P, const NT* t, int n)
{
    Vector_E3<NT> V(0,0,0);
    Point_E3<NT> Origin(0,0,0);
    for( int i = 0; i < n; ++i )
        V = V + (P[i] - Origin) * t[i];
    return Origin + V;
}

template<typename NT>
Point_E3<NT>
bilinear_interpolate(const Point_E3<NT>& P0,
                     const Point_E3<NT>& P1,
                     const Point_E3<NT>& P2,
                     const Point_E3<NT>& P3,
                     const NT& t_P0P1,
                     const NT& t_P0P3)
{
    //         P3 -- P2
    //         |     |
    // t_P0P3  |     |
    //         P0 -- P1
    //            ^
    //            t_P0P1

    Point_E3<NT> P0P1 = interpolate(P0, P1, t_P0P1);
    Point_E3<NT> P3P2 = interpolate(P3, P2, t_P0P1);

    Point_E3<NT> P = interpolate(P0P1, P3P2, t_P0P3);
    return P;
}

template<typename NT>
Array2<Point_E3<NT> >
quad_subdivide(const Polygon_E3<NT>& poly, int divx, int divy)
// divx and divy are the numbers of the regions (triangle pairs), not vertices.
{
    assert(poly.size() == 4);
    Array2<Point_E3<NT> > S(divx + 1, divy + 1);
    for(int y = 0; y <= divy; ++y)
        for(int x = 0; x <= divx; ++x)
            S(x,y) = bilinear_interpolate(poly[0], poly[1], poly[2], poly[3],
                                          NT(x) / NT(divx), NT(y) / NT(divy));
    return S;
}

#endif // INTERPOLATION_E3_H
