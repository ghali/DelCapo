
#ifndef POLYGON_NORMAL_E3_H
#define POLYGON_NORMAL_E3_H

#include "vector_e3.h"
#include "polygon_e3.h"

// The following implementation depends on Cartesian coordinates.
template<typename NT>
Vector_E3<NT>
polygon_normal_vector(const Polygon_E3<NT>& P)
{
    NT mx=0, my=0, mz=0;
    for(int i = 0; i < int(P.size()); ++i) {
        int in = (i+1) % int(P.size());
        mx += ( P[i].y() - P[in].y() )  *  ( P[i].z() + P[in].z() );
        my += ( P[i].z() - P[in].z() )  *  ( P[i].x() + P[in].x() );
        mz += ( P[i].x() - P[in].x() )  *  ( P[i].y() + P[in].y() );
    }
    return Vector_E3<NT>(mx, my, mz);
}

#endif // POLYGON_NORMAL_E3_H
