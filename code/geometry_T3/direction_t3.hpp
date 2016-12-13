// Sherif Ghali

#ifndef DIRECTION_T3_H
#define DIRECTION_T3_H

#include <cmath>
using std::sqrt;
#include "geometry_T3/vector_t3.hpp"

template<typename NT>
class Direction_T3
{
    Vector_T3<NT> V;

    void
    normalize() {
        NT d = static_cast<NT>( sqrt( V.length2() ) );
        V = Vector_T3<NT>(V.hx()/d, V.hy()/d, V.hz()/d);
    }

public:
    Direction_T3() : V() {}
    Direction_T3(const NT& x, const NT& y, const NT& z, const NT& w = 1) : V(x,y,z,w) {
        normalize();
    }
    Direction_T3(const Vector_T3<NT>& V) : V(V) {
        normalize();
    }

    NT hx() const { return V.hx(); }
    NT hy() const { return V.hy(); }
    NT hz() const { return V.hz(); }
    NT hw() const { return V.hw(); }

    Vector_T3<NT> getVector_T3() const { return V; }

    Direction_T3<NT> operator+(const Direction_T3<NT>& D2)
    {
        return Direction_T3<NT>(V + D2.V);
    }

    bool operator==(const Direction_T3<NT>& d) const {
        return (V == d.V);
    }
};

template<class S>
S
dot_product(const Direction_T3<S>& d1, const Direction_T3<S>& d2)
{
    return dot_product(d1.getVector_T3(), d2.getVector_T3());
}

template<class S>
Direction_T3<S>
cross_product(const Direction_T3<S>& v1, const Direction_T3<S>& v2)
{
    return Direction_T3<S>(+ determinant(v1.hy(), v2.hy(),
                                         v1.hz(), v2.hz()),
                           - determinant(v1.hx(), v2.hx(),
                                         v1.hz(), v2.hz()),
                           + determinant(v1.hx(), v2.hx(),
                                         v1.hy(), v2.hy()),
                           v1.hw() * v2.hw());
}

#endif // DIRECTION_T3_H
