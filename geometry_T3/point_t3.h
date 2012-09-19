// Sherif Ghali

#ifndef POINT_T3_H
#define POINT_T3_H

#include <iostream>
using std::ostream;
using std::istream;

#include "misc/determinant.h"
#include "geometry_E3/point_e3.h"

template<typename NT>
class Point_T3;

template<typename NT>
ostream& operator<< (ostream& os, const Point_T3<NT>& p);

template<typename NT>
istream& operator>> (istream& is, Point_T3<NT>& p);

template<typename NT>
class Point_T3
{
private:
    NT _x, _y, _z, _w;
public:
    typedef NT NumberType;
    typedef Point_T3<NT> value_type;

    Point_T3() : _x(), _y(), _z(), _w(1)
    {
    }

    Point_T3(const NT& x, const NT& y, const NT& z, const NT& w = 1)
        : _x(x), _y(y), _z(z), _w(w) {}

    Point_T3(const Point_E3<NT>& P)
        : _x(P.x()), _y(P.y()), _z(P.z()), _w(1) {}

    NT hx() const { return _x; }
    NT hy() const { return _y; }
    NT hz() const { return _z; }
    NT hw() const { return _w; }

    bool operator==(const Point_T3<NT>& p) const {
        return (this == &p) ||
            are_dependent( _x, _y, _z, _w, p._x, p._y, p._z, p._w )
            && inner_product(_x, _y, _z, _w, p._x, p._y, p._z, p._w) > 0;
    }

    bool operator!=(const Point_T3<NT>& p) const {
        return ! operator==(p);
    }

    friend ostream& operator<< <>(ostream& os, const Point_T3<NT>& p);

    friend istream& operator>> <>(istream& is, Point_T3<NT>& p);
};

template<typename NT>
ostream& operator<< (ostream& os, const Point_T3<NT>& p)
{
    return os << p._x << " " << p._y << " " << p._z << " " << p._w;
}

template<typename NT>
istream& operator>> (istream& is, Point_T3<NT>& p)
{
    return is >> p._x >> p._y >> p._z >> p._w;
}

#endif // POINT_T3_H
