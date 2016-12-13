// Sherif Ghali

#ifndef DOMINANT_E2_H
#define DOMINANT_E2_H

#include "geometry_E2/vector_e2.hpp"

enum Dominant_E2 { E2_POSX, E2_NEGX, E2_POSY, E2_NEGY };
//----------------------------------------------------------------
template<typename T>
Dominant_E2
dominant(const T& dx, const T& dy)
{
    const T zero(0);
    T dxabs = dx >= zero ? dx : (-dx);
    T dyabs = dy >= zero ? dy : (-dy);

    if(dxabs >= dyabs)
        return ((dx > zero) ? E2_POSX : E2_NEGX);
    else
        return ((dy > zero) ? E2_POSY : E2_NEGY);
}

template<typename T>
Vector_E2<T>
get_Vector_E2(const Dominant_E2& d)
{
    switch( d )
    {
    case E2_POSX: return Vector_E2<T>( 1, 0);
    case E2_NEGX: return Vector_E2<T>(-1, 0);
    case E2_POSY: return Vector_E2<T>( 0, 1);
    case E2_NEGY: return Vector_E2<T>( 0,-1);
    };
}

#endif // DOMINANT_E2_H
