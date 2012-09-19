// Sherif Ghali

#ifndef PROJECTION_E2_H
#define PROJECTION_E2_H

#include "geometry_E2/point_e2.h"
#include "geometry_E2/segment_e2.h"
#include "geometry_E2/predicates_e2.h"

// Find the (signed) larger projected distance between
// segment1.source() and segment1.target().
template<typename NT>
NT
find_larger_projected_difference(const Segment_E2<NT>& segment1)
{
    Dominant_E2 d = dominant( segment1 );

    NT diff = NT();             // initialization avoids warning

    switch(d) {
    case E2_POSX: diff = segment1.target().x() - segment1.source().x(); break;
    case E2_NEGX: diff = segment1.source().x() - segment1.target().x(); break;
    case E2_POSY: diff = segment1.target().y() - segment1.source().y(); break;
    case E2_NEGY: diff = segment1.source().y() - segment1.target().y(); break;
    }        

    return diff;
}


// Find the (signed) larger projected distance between
// segment1.source() and the intersection of segment1 and segment2.
template<typename NT>
NT
find_larger_projected_difference(const Segment_E2<NT>& segment1,
                                 const Segment_E2<NT>& segment2)
// precondition: segments are not parallel
{
    Point_E2<NT> i = intersection_of_lines(segment1, segment2);
    Dominant_E2 d = dominant( segment1 );

    NT diff = NT();             // initialization avoids warning

    switch(d) {
    case E2_POSX: diff = i.x() - segment1.source().x(); break;
    case E2_NEGX: diff = segment1.source().x() - i.x(); break;
    case E2_POSY: diff = i.y() - segment1.source().y(); break;
    case E2_NEGY: diff = segment1.source().y() - i.y(); break;
    }        

    return diff;
}

#endif // PROJECTION_E2_H
