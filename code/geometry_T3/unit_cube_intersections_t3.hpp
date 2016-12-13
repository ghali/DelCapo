// Sherif Ghali

#ifndef UNIT_CUBE_INTERSECTIONS_T3_H
#define UNIT_CUBE_INTERSECTIONS_T3_H

#include <vector>
using std::vector;

#include "misc/determinant.hpp"
#include "geometry_T3/point_t3.hpp"
#include "geometry_T3/segment_t3.hpp"
#include "geometry_T3/plane_t3.hpp"
#include "geometry_T3/predicates_t3.hpp"
#include "geometry_T3/intersection_t3.hpp"

template<typename T>
class Point_T3;

template<typename T>
class Plane_T3;


// "private" function:
template<typename T>
void
process_segment(const Point_T3<T>& last_vertex, const Point_T3<T>& current_vertex,
                Oriented_side last_boundary_coordinates, Oriented_side current_boundary_coordinates,
                vector<Point_T3<T> >& V_clipped,
                const Plane_T3<T>& clipping_plane)
{
    if(current_boundary_coordinates != ON_NEGATIVE_SIDE) // inside box
        if(last_boundary_coordinates != ON_NEGATIVE_SIDE) // inside-inside
            V_clipped.push_back(current_vertex);
        else {                  // inside-outside
            Point_T3<T> p = intersection(clipping_plane, Segment_T3<T>(last_vertex, current_vertex));
            V_clipped.push_back(p);
        }
    else
        if(last_boundary_coordinates != ON_NEGATIVE_SIDE) { // outside-inside
            Point_T3<T> p = intersection(clipping_plane, Segment_T3<T>(last_vertex, current_vertex));
            V_clipped.push_back(p);
            V_clipped.push_back(current_vertex);
        }
        else
            ;                   // outside-outside: do nothing
}

template<typename T>
bool
plane_clip(const vector<Point_T3<T> >& V,
           vector<Point_T3<T> >& V_clipped,
           const Plane_T3<T>& clipping_plane)
{
    typedef typename vector<Point_T3<T> >::const_iterator Point_iterator;
    Point_iterator vit = V.begin();

    Point_T3<T> first_vertex = *vit;
    Oriented_side first_boundary_coordinates = oriented_side(clipping_plane, *vit);

    Point_T3<T> last_vertex = first_vertex;
    Oriented_side last_boundary_coordinates = first_boundary_coordinates;

    while(++vit != V.end()) {
        Point_T3<T> current_vertex = *vit;
        Oriented_side current_boundary_coordinates = oriented_side(clipping_plane, *vit);

        process_segment(last_vertex, current_vertex,
                        last_boundary_coordinates, current_boundary_coordinates,
                        V_clipped, clipping_plane);

        last_vertex = current_vertex;
        last_boundary_coordinates = current_boundary_coordinates;
    }

    process_segment(last_vertex, first_vertex,
                    last_boundary_coordinates, first_boundary_coordinates,
                    V_clipped, clipping_plane);

    return !V_clipped.empty();
}

template<typename T>
bool
unit_cube_clip(Segment_T3<T>& seg)
{
    const Plane_T3<T> Xpos(-1,0,0,1); // X =  1
    const Plane_T3<T> Xneg( 1,0,0,1); // X = -1
    const Plane_T3<T> Ypos(0,-1,0,1); // Y =  1
    const Plane_T3<T> Yneg(0, 1,0,1); // Y = -1
    const Plane_T3<T> Zpos(0,0,-1,1); // Z =  1
    const Plane_T3<T> Zneg(0,0, 1,1); // Z = -1

    return ( positive_half_space_clip(Xpos, seg)
             && positive_half_space_clip(Xneg, seg)
             && positive_half_space_clip(Ypos, seg)
             && positive_half_space_clip(Yneg, seg)
             && positive_half_space_clip(Zpos, seg)
             && positive_half_space_clip(Zneg, seg) );
}

#endif // UNIT_CUBE_INTERSECTIONS_T3_H
