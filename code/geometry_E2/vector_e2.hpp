// Sherif Ghali

#ifndef VECTOR_E2_H
#define VECTOR_E2_H

#include "geometry_E2/point_e2.hpp"
#include "misc/determinant.hpp"

template<typename NT>
class Vector_E2;

template<typename NT>
ostream& operator<< (ostream& os, const Vector_E2<NT>& v);

template<typename NT>
istream& operator>> (istream& is, Vector_E2<NT>& v);

template<typename NT>
class Vector_E2
{
private:
    NT _x;
    NT _y;
public:
    Vector_E2() : _x(), _y() {}
    Vector_E2(const NT& x, const NT& y) : _x(x), _y(y) {}

    NT x() const { return _x; }
    NT y() const { return _y; }

    bool operator==(const Vector_E2<NT>& v) const {
        return (this == &v) ||
            ((_x == v.x()) && (_y == v.y()));
    }

    bool operator!=(const Vector_E2<NT>& v) const {
        return ! operator==(v);
    }

    friend ostream& operator<< <>(ostream& os, const Vector_E2<NT>& p);

    friend istream& operator>> <>(istream& is, Vector_E2<NT>& p);
};

template<typename NT>
Vector_E2<NT>
operator*(const NT& d, const Vector_E2<NT>& v)
{
    return Vector_E2<NT>(d * v.x(), d * v.y());
}

template<typename NT>
Vector_E2<NT>
operator*(const Vector_E2<NT>& v, const NT & d)
{
    return Vector_E2<NT>(v.x() * d, v.y() * d);
}

template<typename NT>
Vector_E2<NT>
operator/(const Vector_E2<NT>& v, const NT & d)
{
    return Vector_E2<NT>(v.x() / d, v.y() / d);
}

template<typename NT>
Vector_E2<NT>
operator+(const Vector_E2<NT>& v1, const Vector_E2<NT>& v2)
{
    return Vector_E2<NT>(v1.x() + v2.x(), v1.y() + v2.y());
}

template<typename NT>
Vector_E2<NT>
operator-(const Vector_E2<NT>& v)
{
    return Vector_E2<NT>( -v.x(), -v.y() );
}

template<typename NT>
Vector_E2<NT>
operator-(const Vector_E2<NT>& v1, const Vector_E2<NT>& v2)
{
    return Vector_E2<NT>(v1.x() - v2.x(), v1.y() - v2.y());
}


template<typename NT>
Point_E2<NT>
operator+(const Point_E2<NT>& p, const Vector_E2<NT>&  v)
{
    return Point_E2<NT>(p.x()+v.x(), p.y()+v.y());
}

template<typename NT>
Point_E2<NT>
operator-(const Point_E2<NT>& p, const Vector_E2<NT>&  v)
{
    return Point_E2<NT>(p.x()-v.x(), p.y()-v.y());
}

template<typename NT>
Point_E2<NT>
Point_E2<NT>::operator+=(const Vector_E2<NT> & v)
{
    this->_x += v.x();
    this->_y += v.y();
    return *this;
}

template<typename NT>
Point_E2<NT>
Point_E2<NT>::operator-=(const Vector_E2<NT> & v)
{
    this->_x -= v.x();
    this->_y -= v.y();
    return *this;
}

template<typename NT>
Vector_E2<NT>
operator-(const Point_E2<NT>& p1, const Point_E2<NT>& p2)
{
    return Vector_E2<NT>(p1.x()-p2.x(), p1.y()-p2.y());
}

template<typename NT>
ostream& operator<< (ostream& os, const Vector_E2<NT>& v)
{
    return os << v._x << " " << v._y;
}

template<typename NT>
istream& operator>> (istream& is, Vector_E2<NT>& v)
{
    return is >> v._x >> v._y;
}

template<typename NT>
NT
dot_product(const Vector_E2<NT>& v1, const Vector_E2<NT>& v2)
{
    return v1.x() * v2.x() + v1.y() * v2.y();
}

template<typename NT>
NT
cross_product(const Vector_E2<NT>& v1, const Vector_E2<NT>& v2)
{
    return determinant(v1.x(), v1.y(), v2.x(), v2.y());
}

#endif // VECTOR_E2_H
