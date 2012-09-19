// Sherif Ghali

#ifndef PREDICATES_E3_H
#define PREDICATES_E3_H

#include "geometry_E3/segment_e3.h"

#include "misc/dominant.h"
#include "misc/determinant.h"

//----------------------------------------------------------------
template<typename T>
Dominant_E3
dominant(const Vector_E3<T>& V)
{
    return dominant(V.x(), V.y(), V.z());
}
//----------------------------------------------------------------
template<typename T>
Dominant_E3
dominant(const Segment_E3<T>& segment)
{
    T dx = segment.target().x() - segment.source().x();
    T dy = segment.target().y() - segment.source().y();
    T dz = segment.target().z() - segment.source().z();

    return dominant(dx, dy, dz);
}

template<typename T>
Dominant_E3
least_dominant(const Vector_E3<T>& v)
{
    return least_dominant(v.x(), v.y(), v.z());
}

template<typename NT>
bool
are_linearly_dependent(const Vector_E3<NT>& v1,
                       const Vector_E3<NT>& v2)
{
    return are_dependent(v1.x(), v1.y(), v1.z(),
                         v2.x(), v2.y(), v2.z());
}

#endif // PREDICATES_E3_H
