// Sherif Ghali

#ifndef TRANSFORMATION_E2_H
#define TRANSFORMATION_E2_H

#include <cassert>
#include <cmath>
#include <stdexcept>
using std::cos;
using std::sin;

#include "geometry_E2/point_e2.h"
#include "geometry_E2/segment_e2.h"
#include "geometry_E2/vector_e2.h"
#include "geometry_E2/line_e2.h"
#include "geometry_E2/polygon_e2.h"

#include "geometry_lib/transformations.h"

extern Identity IDENTITY;
extern Scale SCALE;
extern Rotation ROTATION;
extern Translation TRANSLATION;

template<typename NT>
class Transformation_E2
{
protected:
    NT m00, m01, m02;
    NT m10, m11, m12;

    // m20 = 0 , m21 = 0 , m22 = 1
    void setToIdentity();
public:    
    Transformation_E2();
    Transformation_E2(const Identity);
    Transformation_E2(const Scale, const Vector_E2<NT>& v);
    Transformation_E2(const Translation, const Vector_E2<NT>& v);
    Transformation_E2(const Rotation, const NT& angle);

    Transformation_E2(const NT& m00, const NT& m01,
                      const NT& m10, const NT& m11)
        : m00(m00), m01(m01), m02(0),
          m10(m10), m11(m11), m12(0)
    {}
    Transformation_E2(const NT& m00, const NT& m01, const NT& m02,
                      const NT& m10, const NT& m11, const NT& m12)
        : m00(m00), m01(m01), m02(m02),
          m10(m10), m11(m11), m12(m12)
    {}

    NT get_m00() { return m00; }
    NT get_m01() { return m01; }
    NT get_m02() { return m02; }

    NT get_m10() { return m10; }
    NT get_m11() { return m11; }
    NT get_m12() { return m12; }

    Transformation_E2 inverse() const;

    Point_E2<NT>      transform(const Point_E2<NT>& p) const;
    Line_E2<NT>       transform(const Line_E2<NT>& l) const;
    Vector_E2<NT>     transform(const Vector_E2<NT>& p) const;
    Direction_E2<NT>  transform(const Direction_E2<NT>& p) const;
    Segment_E2<NT>    transform(const Segment_E2<NT>& s) const;
    Polygon_E2<NT>    transform(const Polygon_E2<NT>& s) const;
    Point_E2<NT>      operator()(const Point_E2<NT>& p) const;
    Line_E2<NT>       operator()(const Line_E2<NT>& l) const;
    Vector_E2<NT>     operator()(const Vector_E2<NT>& v) const;
    Direction_E2<NT>  operator()(const Direction_E2<NT>& v) const;
    Segment_E2<NT>    operator()(const Segment_E2<NT>& v) const;
    Polygon_E2<NT>    operator()(const Polygon_E2<NT>& v) const;

    Transformation_E2<NT> operator*(const Transformation_E2<NT>& TR) const;
};

template<typename NT>
void
Transformation_E2<NT>::setToIdentity()
{
    m00 = m11 = 1;
    m01 = m10 = m02 = m12 = 0;
}
template<typename NT>
Transformation_E2<NT>::Transformation_E2()
{
    setToIdentity();
}

template<typename NT>
Transformation_E2<NT>::Transformation_E2(const Identity)
{
    setToIdentity();
}

template<typename NT>
Transformation_E2<NT>::Transformation_E2(const Scale, const Vector_E2<NT>& v)
{
    setToIdentity();
    m00 = v.x();
    m11 = v.y();
}

template<typename NT>
Transformation_E2<NT>::Transformation_E2(const Rotation, const NT& angle)
{
    setToIdentity();
    const NT c = cos(angle);
    const NT s = sin(angle);
    m00 = c;
    m01 = -s;
    m10 = s;
    m11 = c;
}

template<typename NT>
Transformation_E2<NT>::Transformation_E2(const Translation, const Vector_E2<NT>& v)
{
    setToIdentity();
    m02 = v.x();
    m12 = v.y();
}

template<typename NT>
Transformation_E2<NT>
Transformation_E2<NT>::inverse() const
{
    typedef const NT CT;
    CT det = determinant(m00, m01, m10, m11);
    if(det == NT(0))
        throw std::domain_error("Attempting to invert a singular matrix.");
    CT d = CT(1) / det;

    CT n00 =  m11;
    CT n01 = -m01;
    CT n02 =  determinant(m01, m02, m11, m12);

    CT n10 = -m10;
    CT n11 =  m00;   
    CT n12 = -determinant(m00, m10, m02, m12);

    return Transformation_E2<NT>(d * n00, d * n01, d * n02,
                                 d * n10, d * n11, d * n12);
}

template<typename NT>
Point_E2<NT>
Transformation_E2<NT>::transform(const Point_E2<NT>& p) const
{
    return Point_E2<NT>(m00 * p.x() + m01 * p.y() + m02 ,
                        m10 * p.x() + m11 * p.y() + m12 );
}

template<typename NT>
Line_E2<NT>
Transformation_E2<NT>::transform(const Line_E2<NT>& l) const
{
    // ...
    assert(false);
}

template<typename NT>
Vector_E2<NT>
Transformation_E2<NT>::transform(const Vector_E2<NT>& v) const
{
    return Vector_E2<NT>(m00 * v.x() + m01 * v.y(),
                         m10 * v.x() + m11 * v.y());
}

template<typename NT>
Direction_E2<NT>
Transformation_E2<NT>::transform(const Direction_E2<NT>& d) const
{
    return Vector_E2<NT>(m00 * d.x() + m01 * d.y(),
                         m10 * d.x() + m11 * d.y());
}

template<typename NT>
Segment_E2<NT>
Transformation_E2<NT>::transform(const Segment_E2<NT>& seg) const
{
    Point_E2<NT> s = transform(seg.source());
    Point_E2<NT> t = transform(seg.target());

    return Segment_E2<NT>(s,t);
}

template<typename NT>
Polygon_E2<NT>
Transformation_E2<NT>::transform(const Polygon_E2<NT>& polygon) const
{
    Polygon_E2<NT> rp(polygon.size());
    std::transform(polygon.vertices.begin(), polygon.vertices.end(), rp.vertices.begin(), *this);

    return rp;
}

template<typename NT>
Point_E2<NT>
Transformation_E2<NT>::operator()(const Point_E2<NT>& p) const
{
    return transform(p);
}

template<typename NT>
Line_E2<NT>
Transformation_E2<NT>::operator()(const Line_E2<NT>& l) const
{
    return transform(l);
}

template<typename NT>
Vector_E2<NT>
Transformation_E2<NT>::operator()(const Vector_E2<NT>& v) const
{
    return transform(v);
}

template<typename NT>
Direction_E2<NT>
Transformation_E2<NT>::operator()(const Direction_E2<NT>& d) const
{
    return transform(d);
}

template<typename NT>
Segment_E2<NT>
Transformation_E2<NT>::operator()(const Segment_E2<NT>& s) const
{
    return transform(s);
}

template<typename NT>
Transformation_E2<NT>
Transformation_E2<NT>::operator*(const Transformation_E2<NT>& TR) const
{
    // Notice the order of composition:
    //      *this is applied second and TR is applied first
    return Transformation_E2<NT>(m00 * TR.m00 + m01 * TR.m10         ,
                                 m00 * TR.m01 + m01 * TR.m11         ,
                                 m00 * TR.m02 + m01 * TR.m12  +  m02 ,

                                 m10 * TR.m00 + m11 * TR.m10         ,
                                 m10 * TR.m01 + m11 * TR.m11         ,
                                 m10 * TR.m02 + m11 * TR.m12  +  m12 );
}

#endif // TRANSFORMATION_E2_H
