// Sherif Ghali

#include <cassert>
#include <cmath>
#include "geometry_E3/vector_e3.hpp"

template<typename NT>
void
normalize( const Vector_E3<NT>& v, Vector_E3<double>& vn )
{
    double dx = double(v.x());
    double dy = double(v.y());
    double dz = double(v.z());
    assert( dx != 0 || dy != 0 || dz != 0 );
    double dd = dx*dx + dy*dy + dz*dz;
    double d = std::sqrt(dd);

    vn = Vector_E3<double>(dx/d, dy/d, dz/d);
}

template<typename NT>
void
normalize( const Vector_E3<NT>& v, Vector_E3<float>& vn )
{
    float dx = float(v.x());
    float dy = float(v.y());
    float dz = float(v.z());
    assert( dx != 0 || dy != 0 || dz != 0 );
    float dd = dx*dx + dy*dy + dz*dz;
    float d = std::sqrt(dd);

    vn = Vector_E3<float>(dx/d, dy/d, dz/d);
}
