// Sherif Ghali

#ifndef INTERPOLATION_E2_H
#define INTERPOLATION_E2_H

#include "geometry_E2/point_e2.h"
#include "geometry_E2/vector_e2.h"
#include "geometry_E2/bbox_e2.h"
#include "geometry_E2/transformation_e2.h"
#include "geometry_E2/interpolation_e2.h"

template<typename NT>
Point_E2<NT>
interpolate(const Point_E2<NT>& P0, const Point_E2<NT>& P1, const NT& t)
{
    return Point_E2<NT>(P0 + (P1-P0) * t);
}

// Notice that we change the semantics for the next two functions.
// The t values are the relative weights of the points, whereas
// previously t was a parameterization of the line segment.

// Precondition: sum(t_i) = 1.
template<typename NT>
Point_E2<NT>
interpolate(const Point_E2<NT>& P0, const NT& t0,
            const Point_E2<NT>& P1, const NT& t1,
            const Point_E2<NT>& P2, const NT& t2)
{
    const Point_E2<NT> Origin(0.0, 0.0);
    // The following expression can be optimized by ignoring, say, t2.
    return Origin + ((P0-Origin) * t0 + (P1-Origin) * t1 + (P2-Origin) * t2);
}

template<typename NT>
Point_E2<NT>
interpolate(const Point_E2<NT>* P, const NT* t, int n)
{
    Vector_E2<NT> V(0,0);
    Point_E2<NT> Origin(0,0);
    for( int i = 0; i < n; ++i )
        V = V + (P[i] - Origin) * t[i];
    return Origin + V;
}

// Unlike Point_S1 interpolation, which interpolates along the segment
// defined by the two endpoints, the following interpolation between
// two Direction_E2 will take the shorter path.

template<typename NT>
Direction_E2<NT>
interpolate_shorter(const Direction_E2<NT>& P1, const Direction_E2<NT>& P2, const NT& u)
{
    NT costheta = dot_product(P1, P2);
    NT theta = std::acos(costheta);
    NT sintheta = std::sin(theta);
    Vector_E2<NT> v1 = Vector_E2<NT>(P1.x(), P1.y()) * (sin((1-u) * theta) / sintheta);
    Vector_E2<NT> v2 = Vector_E2<NT>(P2.x(), P2.y()) * (sin(u     * theta) / sintheta);
    Direction_E2<NT> ret = Direction_E2<NT>(v1 + v2);
    return ret;
}

template<typename NT>
Transformation_E2<NT>
interpolate(const Bbox_E2<NT>& from, const Bbox_E2<NT>& to)
{
    Point_E2<NT> ORIGIN(0,0);
    Transformation_E2<NT> translate_to_origin(TRANSLATION, Vector_E2<NT>(ORIGIN - from.LL()));
    Transformation_E2<NT> scale(SCALE, Vector_E2<NT>( to.get_width()  / from.get_width(),
                                                      to.get_height() / from.get_height() ));
    Transformation_E2<NT> translate_from_origin(TRANSLATION, Vector_E2<NT>(to.LL() - ORIGIN));

    Transformation_E2<NT> T = translate_from_origin * scale * translate_to_origin;

    return T;
}

// Return the Bbox with the same aspect ratio as window
// such that scene is centered when drawn in window.
template<typename NT>
Bbox_E2<NT>
center(const Bbox_E2<NT>& scene, const Bbox_E2<NT>& window)
{
    const NT scene_AR = scene.get_width() / scene.get_height();
    const NT window_AR = window.get_width() / window.get_height();
    Point_E2<NT> LL = scene.LL();
    Point_E2<NT> UR = scene.UR();
    Vector_E2<NT> V;

    if( scene_AR < window_AR ) {
        NT Xdiff = (scene.height() * window_AR - scene.width()) / (NT) 2.0;
        V = Vector_E2<NT>(Xdiff, (NT) 0);
    }
    else {
        NT Ydiff = (scene.width() / window_AR - scene.height()) / (NT) 2.0;
        V = Vector_E2<NT>((NT) 0, Ydiff);
    }
    LL -= V;
    UR += V;
    return Bbox_E2<NT>(LL, UR);
}

#endif // INTERPOLATION_E2_H
