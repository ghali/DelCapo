// Sherif Ghali

#ifndef POINT_E2_H
#define POINT_E2_H

#include <iostream>
using std::ostream;
using std::istream;

template<typename NT>
class Point_E2;
template<typename NT>
class Vector_E2;

template<typename NT>
ostream& operator<< (ostream& os, const Point_E2<NT>& p);

template<typename NT>
istream& operator>> (istream& is, Point_E2<NT>& p);

template<typename NT>
class Point_E2
{
private:
    NT _x, _y;
public:
    typedef NT NumberType;
    typedef NT number_type;
    typedef Point_E2<NT> value_type;

    Point_E2() : _x(), _y()
    {
    }

    Point_E2(const NT& x, const NT& y) : _x(x), _y(y) {}

    NT x() const { return _x; }
    NT y() const { return _y; }

    bool operator==(const Point_E2<NT>& p) const {
        return (this == &p) ||
            ((_x == p._x) && (_y == p._y));
    }

    bool operator!=(const Point_E2<NT>& p) const {
        return ! operator==(p);
    }

    Point_E2<NT> operator+=(const Vector_E2<NT> & v);
    Point_E2<NT> operator-=(const Vector_E2<NT> & v);

    friend ostream& operator<< <>(ostream& os, const Point_E2<NT>& p);

    friend istream& operator>> <>(istream& is, Point_E2<NT>& p);
};

template<typename NT>
ostream& operator<< (ostream& os, const Point_E2<NT>& p)
{
    return os << p._x << " " << p._y;
}

template<typename NT>
istream& operator>> (istream& is, Point_E2<NT>& p)
{
    return is >> p._x >> p._y;
}

template<typename NT>
NT
squared_distance(const Point_E2<NT>& P, const Point_E2<NT>& Q)
{
    NT x = P.x() - Q.x();
    NT y = P.y() - Q.y();
    return x*x + y*y;
}

#endif // POINT_E2_H
