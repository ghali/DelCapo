// Sherif Ghali

#ifndef DIRECTION_E2_H
#define DIRECTION_E2_H

#include <cmath>
using std::sqrt;
#include "misc/determinant.hpp"
#include "geometry_E2/vector_e2.hpp"
#include "geometry_E2/segment_e2.hpp"

template<typename NT>
class Direction_E2;

template<class NT>
std::ostream&
operator<<(std::ostream& os, const Direction_E2<NT>& d);

template<class NT>
std::istream&
operator>>(std::istream& is, Direction_E2<NT>& d);

template<typename NT>
class Direction_E2
{
    NT _x, _y;

    void
    normalize() {
        NT d = sqrt( _x*_x + _y*_y );
        _x = _x/d;
        _y = _y/d;
    }
public:
    Direction_E2() : _x(1), _y(0) {}
    Direction_E2(const NT& x, const NT& y) : _x(x), _y(y) {
        normalize();
    }
    Direction_E2(const Vector_E2<NT>& v) : _x(v.x()), _y(v.y()) {
        normalize();
    }
    Direction_E2(const Point_E2<NT>& source,
                 const Point_E2<NT>& target)
    {
        _x = target.x() - source.x();
        _y = target.y() - source.y();
        normalize();
    }
    Direction_E2(const Segment_E2<NT>& seg)
    {
        _x = seg.target().x() - seg.source().x();
        _y = seg.target().y() - seg.source().y();
        normalize();
    }

    NT x() const { return _x; }
    NT y() const { return _y; }

    Vector_E2<NT> getVector_E2() const {
        return Vector_E2<NT>(_x,_y);
    }

    bool
    operator==(const Direction_E2<NT>& d) const {
        return (this == &d) ||
            (determinant(_x, _y, d._x, d._y) == 0 &&
             inner_product(_x, _y, d._x, d._y) > 0);
    }

    Direction_E2<NT>
    operator-() const {
        return Direction_E2<NT>(- _x, - _y);
    }

    template<class S>
    friend std::ostream&
    operator<<(std::ostream& os, const Direction_E2<S>& d);

    template<class S>
    friend std::istream&
    operator>>(std::istream& is, Direction_E2<S>& d);

};

template<class S>
std::ostream&
operator<<(std::ostream& os, const Direction_E2<S>& d)
{
    return os << d._x << " " << d._y;
}

template<class S>
std::istream&
operator>>(std::istream& is, Direction_E2<S>& d)
{
    return is >> d._x >> d._y;
}

template<typename NT>
Point_E2<NT>
operator+(const Point_E2<NT>& p, const Direction_E2<NT>& d)
{
    return Point_E2<NT>(p.x() + d.x(), p.y() + d.y());
}

template<typename NT>
NT
dot_product(const Direction_E2<NT>& d1, const Direction_E2<NT>& d2)
{
    return d1.x() * d2.x() + d1.y() * d2.y();
}

template<typename NT>
NT
cross_product(const Direction_E2<NT>& d1, const Direction_E2<NT>& d2)
{
    return determinant(d1.x(), d1.y(), d2.x(), d2.y());
}

#endif // DIRECTION_E2_H
