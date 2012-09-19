// Sherif Ghali

#ifndef INTERSECTION_E2_H
#define INTERSECTION_E2_H

#include <cassert>

#include <limits>
#include <iterator>

#include "misc/determinant.h"

#include "geometry_E2/point_e2.h"
#include "geometry_E2/line_e2.h"
#include "geometry_E2/segment_e2.h"
#include "geometry_E2/bbox_e2.h"
#include "geometry_E2/predicates_e2.h"
#include "geometry_E2/projection_e2.h"

template<typename NT>
class Point_E2;

template<typename NT>
class Segment_E2;

template<typename NT>
class Line_E2;

//----------------------------------------------------------------
template<typename NT>
Point_E2<NT>
intersection(const Line_E2<NT>& l1, const Line_E2<NT>& l2)
// precondition: lines are not parallel
{
    NT denom = determinant(l1.a(), l1.b(), l2.a(), l2.b());
    assert(denom != 0);

    NT detx = + determinant(l1.b(), l1.c(), l2.b(), l2.c());
    NT dety = - determinant(l1.a(), l1.c(), l2.a(), l2.c());

    return Point_E2<NT>(detx/denom, dety/denom);
}
//----------------------------------------------------------------

template<typename NT>
Point_E2<NT>
intersection_of_lines(const Segment_E2<NT>& seg1, const Segment_E2<NT>& seg2)
{
    Line_E2<NT> l1(seg1);
    Line_E2<NT> l2(seg2);

    return intersection(l1, l2);
}

template<class NT>
bool
positive_half_space_clip(const Line_E2<NT> & splittingLine,
                         Segment_E2<NT> & mySegment)
{
    Oriented_side sourceSide = oriented_side( splittingLine, mySegment.source() );
    Oriented_side targetSide = oriented_side( splittingLine, mySegment.target() );

    if(sourceSide != ON_NEGATIVE_SIDE      && targetSide != ON_NEGATIVE_SIDE)
        return true; // no clipping needed: segment is entirely inside

    else if((sourceSide == ON_POSITIVE_SIDE  && targetSide == ON_NEGATIVE_SIDE) ||
            (sourceSide == ON_NEGATIVE_SIDE  && targetSide == ON_POSITIVE_SIDE))
    {
        Line_E2<NT> carrying_line(mySegment.source(), mySegment.target());
        Point_E2<NT> intersectionPoint = intersection(splittingLine, carrying_line);

        if(sourceSide == ON_POSITIVE_SIDE)
            mySegment = Segment_E2<NT>(mySegment.source(), intersectionPoint);
        else if(targetSide == ON_POSITIVE_SIDE)
            mySegment = Segment_E2<NT>(intersectionPoint, mySegment.target());

        return true;
    }
    else
        // mySegment is ON_NEGATIVE_SIDE, possibly with (at most)
        // one endpoint ON_ORIENTED_BOUNDARY
        return false;
}

template<typename NT>
bool
Brute_Bbox_clipping_of_segment(const Bbox_E2<NT>& B, Segment_E2<NT>& seg)
{
    const Line_E2<NT> E = B.getEastLine();
    const Line_E2<NT> N = B.getNorthLine();
    const Line_E2<NT> W = B.getWestLine();
    const Line_E2<NT> S = B.getSouthLine();

    return ( positive_half_space_clip(E, seg)
             && positive_half_space_clip(N, seg)
             && positive_half_space_clip(W, seg)
             && positive_half_space_clip(S, seg));
}

// first is true iff some segment is returned
// (i.e., if segment is not clipped out)

//----------------clip_line_to_convex_region_Begin----------------
template<typename NT>
std::pair<bool,Segment_E2<NT> >
clip_line_to_convex_region(
                           const Segment_E2<NT>& carrying_line,
                           const std::vector<Segment_E2<NT> >& region)
{
    const NT zero(0);
    NT min_largest  = + std::numeric_limits<NT>::max();
    NT max_smallest = - std::numeric_limits<NT>::max();

    Segment_E2<NT> s_min_largest, s_max_smallest;
    typedef typename std::vector<Segment_E2<NT> >::const_iterator Sci;
    for( Sci s = region.begin(); s != region.end(); ++s )
        if( !are_parallel(*s, carrying_line) )
        {
            bool plus = ( cross_product(carrying_line.get_Vector_E2(),
                                        s->get_Vector_E2()) > zero );

            NT lpd = find_larger_projected_difference(carrying_line, *s);
            if( plus ) {
                if( lpd < min_largest )
                { min_largest = lpd; s_min_largest = *s; }
            } else
                if( lpd > max_smallest )
                { max_smallest = lpd; s_max_smallest = *s; }
        }
        else                    // simulate T2
            if( oriented_side(*s, carrying_line.source()) == ON_NEGATIVE_SIDE )
                return std::make_pair(false, Segment_E2<NT>()); // clipped out

    if( min_largest <= max_smallest )
        return std::make_pair(false, Segment_E2<NT>()); // clipped out

    Point_E2<NT> s = intersection_of_lines(carrying_line, s_max_smallest);
    Point_E2<NT> t = intersection_of_lines(carrying_line, s_min_largest);
    return std::make_pair(true, Segment_E2<NT>(s,t));
}
//----------------clip_line_to_convex_region_End----------------


template<typename NT>
std::pair<bool,Segment_E2<NT> >
clip_segment_to_ccw_convex_region(
                                  const Segment_E2<NT>& carrying_line,
                                  const std::vector<Segment_E2<NT> >& region)
{
    const NT zero(0);
    NT initial_largest, min_largest;
    initial_largest = min_largest  = find_larger_projected_difference(carrying_line);
    NT initial_smallest, max_smallest;
    initial_smallest = max_smallest = zero;

    Segment_E2<NT> s_min_largest, s_max_smallest;
    typedef typename std::vector<Segment_E2<NT> >::const_iterator Sci;
    for( Sci s = region.begin(); s != region.end(); ++s )
        if( !are_parallel(*s, carrying_line) )
        {
            bool plus = ( cross_product(carrying_line.get_Vector_E2(),
                                        s->get_Vector_E2()) > zero );

            NT lpd = find_larger_projected_difference(carrying_line, *s);
            if( plus ) {
                if( lpd < min_largest )
                { min_largest = lpd; s_min_largest = *s; }
            } else
                if( lpd > max_smallest )
                { max_smallest = lpd; s_max_smallest = *s; }
        }
        else                    // simulate T2
            if( oriented_side(*s, carrying_line.source()) == ON_NEGATIVE_SIDE )
                return std::make_pair(false, Segment_E2<NT>()); // clipped out

    if( min_largest <= max_smallest )
        return std::make_pair(false, Segment_E2<NT>()); // clipped out

    Point_E2<NT> s = carrying_line.source();
    Point_E2<NT> t = carrying_line.target();
    if ( initial_smallest != max_smallest )
        s = intersection_of_lines(carrying_line, s_max_smallest);
    if( initial_largest != min_largest )
        t = intersection_of_lines(carrying_line, s_min_largest);
    return std::make_pair(true, Segment_E2<NT>(s,t));
}

template<typename NT>
std::pair<bool,Segment_E2<NT> >
clip_segment_to_cw_convex_region(
                                 const Segment_E2<NT>& carrying_line,
                                 const std::vector<Segment_E2<NT> >& region)
{
    const NT zero(0);
    NT initial_largest, min_largest;
    initial_largest = min_largest  = find_larger_projected_difference(carrying_line);
    NT initial_smallest, max_smallest;
    initial_smallest = max_smallest = zero;

    Segment_E2<NT> s_min_largest, s_max_smallest;
    typedef typename std::vector<Segment_E2<NT> >::const_iterator Sci;
    for( Sci s = region.begin(); s != region.end(); ++s )
        if( !are_parallel(*s, carrying_line) )
        {
            bool plus = ( cross_product(carrying_line.get_Vector_E2(),
                                        s->get_Vector_E2()) > zero );

            NT lpd = find_larger_projected_difference(carrying_line, *s);
            if( !plus ) {
                if( lpd < min_largest )
                { min_largest = lpd; s_min_largest = *s; }
            } else
                if( lpd > max_smallest )
                { max_smallest = lpd; s_max_smallest = *s; }
        }
        else                    // simulate T2
            if( oriented_side(*s, carrying_line.source()) == ON_POSITIVE_SIDE )
                return std::make_pair(false, Segment_E2<NT>()); // clipped out

    if( min_largest <= max_smallest )
        return std::make_pair(false, Segment_E2<NT>()); // clipped out

    Point_E2<NT> s = carrying_line.source();
    Point_E2<NT> t = carrying_line.target();
    if ( initial_smallest != max_smallest )
        s = intersection_of_lines(carrying_line, s_max_smallest);
    if( initial_largest != min_largest )
        t = intersection_of_lines(carrying_line, s_min_largest);
    return std::make_pair(true, Segment_E2<NT>(s,t));
}

#endif // INTERSECTION_E2_H
