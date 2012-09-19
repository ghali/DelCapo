// Sherif Ghali

#ifndef plane_T3_h
#define plane_T3_h

#include "geometry_T3/vector_t3.h"

template<typename T>
class Plane_T3;

template<typename T>
istream& operator>> (istream& is, Plane_T3<T>& p);

template<typename T>
class Plane_T3
{
    T _X, _Y, _Z, _W;

    void init_Xbcd(const Point_T3<T>& p1, const Point_T3<T>& p2, const Point_T3<T>& t3)
    {
        _X = + determinant(p1.hy(), p1.hz(), p1.hw(),
                           p2.hy(), p2.hz(), p2.hw(),
                           t3.hy(), t3.hz(), t3.hw());
        _Y = - determinant(p1.hx(), p1.hz(), p1.hw(),
                           p2.hx(), p2.hz(), p2.hw(),
                           t3.hx(), t3.hz(), t3.hw());
        _Z = + determinant(p1.hx(), p1.hy(), p1.hw(),
                           p2.hx(), p2.hy(), p2.hw(),
                           t3.hx(), t3.hy(), t3.hw());
        _W = - determinant(p1.hx(), p1.hy(), p1.hz(),
                           p2.hx(), p2.hy(), p2.hz(),
                           t3.hx(), t3.hy(), t3.hz());
    }
public:
    Plane_T3() {}
    Plane_T3(const T& X, const T& Y, const T& Z, const T& W) : _X(X), _Y(Y), _Z(Z), _W(W) {}
    Plane_T3(const Point_T3<T>& P1, const Point_T3<T>& P2, const Point_T3<T>& T3)
    {
        init_Xbcd(P1, P2, T3);
    }

    T X() const { return _X; }
    T Y() const { return _Y; }
    T Z() const { return _Z; }
    T W() const { return _W; }

    bool isIncident(const Point_T3<T>& p) const
    {
        return (_X * p.hx() + _Y * p.hy() + _Z * p.hz() + _W * p.hw()) == 0;
    }

    friend istream& operator>> <>(istream& is, Plane_T3<T>& p);
};

template<typename T>
ostream& operator<< (ostream& os, const Plane_T3<T>& p)
{
    return os << p.X() << " " << p.Y() << " " << p.Z() << " " << p.W();
}

template<typename T>
istream& operator>> (istream& is, Plane_T3<T>& p)
{
    return is >> p._X >> p._Y >> p._Z >> p._W;
}

#endif // plane_T3_h
