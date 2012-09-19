// Sherif Ghali

#ifndef BBOX_E3_H
#define BBOX_E3_H

#include "geometry_E3/point_e3.h"

template<typename T>
class Bbox_E3;

template<class S>
std::ostream&
operator<<(std::ostream& os, const Bbox_E3<S>& B);

template<typename T>
class Bbox_E3
{
    Point_E3<T> _LL;
    Point_E3<T> _UR;
public:
    Bbox_E3() : _LL(), _UR() {}

    Bbox_E3(const Point_E3<T>& p) : _LL(p), _UR(p) {}

    Bbox_E3(const Point_E3<T>& pLL, const Point_E3<T>& pUR)
        : _LL(pLL), _UR(pUR) {}

    Bbox_E3<T>&
    operator+=(const Point_E3<T>& p)
    {
        T xll = _LL.x();
        T yll = _LL.y();
        T zll = _LL.z();
        T xur = _UR.x();
        T yur = _UR.y();
        T zur = _UR.z();

        if(p.x() < xll)
            xll = p.x();
        if(p.y() < yll)
            yll = p.y();
        if(p.z() < zll)
            zll = p.z();

        if(p.x() > xur)
            xur = p.x();
        if(p.y() > yur)
            yur = p.y();
        if(p.z() > zur)
            zur = p.z();

        _LL = Point_E3<T>(xll,yll,zll);
        _UR = Point_E3<T>(xur,yur,zur);

        return *this;
    }

    bool operator==(const Bbox_E3<T>& box) const
    {
        return box._LL == _LL && box._UR == _UR;
    }

    Point_E3<T> LL() const { return _LL; }
    Point_E3<T> UR() const { return _UR; }

    Point_E3<T> center() const { return Point_E3<T>((_LL.x() + _UR.x())/2.0,
                                                    (_LL.y() + _UR.y())/2.0,
                                                    (_LL.z() + _UR.z())/2.0); }
};

template<class S>
std::ostream&
operator<<(std::ostream& os, const Bbox_E3<S>& B)
{
    return os << B.LL() << " --- " << B.UR();
}

#endif // BBOX_E3_H
