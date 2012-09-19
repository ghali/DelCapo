// Sherif Ghali

#ifndef CIRCLE_E2_H
#define CIRCLE_E2_H

#include "geometry_E2/point_e2.h"

template<typename NT>
class Circle_E2
{
private:
    Point_E2<NT> _center;
    NT _radius;
public:
    Circle_E2() : _center(NT(0),NT(0)), _radius(NT(1)) {}

    Circle_E2(const Point_E2<NT> & center, const NT & radius)
        : _center(center), _radius(radius)
    {}

    Circle_E2<NT>& operator=(const Circle_E2<NT>& rs)
    {
        if(this != *rs) {
            _center = rs._center;
            _radius = rs._radius;
        }
        return *this;
    }

    Point_E2<NT> center() const { return _center; }
    NT radius() const { return _radius; }

    bool operator==(const Circle_E2<NT> & c) const {
        return (this == &c) ||
            (_center == c._center) && (_radius == c._radius);
    }

    bool operator!=(const Circle_E2<NT> & c) const {
        return ! operator==(c);
    }
};

#endif // CIRCLE_E2_H
