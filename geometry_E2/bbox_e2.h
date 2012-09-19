// Sherif Ghali

#ifndef BBOX_E2_H
#define BBOX_E2_H

#include "geometry_E2/point_e2.h"
#include "geometry_E2/line_e2.h"

template<typename NT>
class Bbox_E2
{
    Point_E2<NT> _LL;
    Point_E2<NT> _UR;
public:
    Bbox_E2() : _LL(), _UR() {}

    Bbox_E2(const Point_E2<NT>& p) : _LL(p), _UR(p) {}

    Bbox_E2(const Point_E2<NT>& pLL, const Point_E2<NT>& pUR)
        : _LL(pLL), _UR(pUR) {}

    NT width()  const { return _UR.x() - _LL.x(); }
    NT height() const { return _UR.y() - _LL.y(); }
    // remove:
    NT get_width()  const { return _UR.x() - _LL.x(); }
    NT get_height() const { return _UR.y() - _LL.y(); }

    Bbox_E2<NT>&
    operator+=(const Point_E2<NT>& p)
    {
        NT xll = _LL.x();
        NT yll = _LL.y();
        NT xur = _UR.x();
        NT yur = _UR.y();
        if(p.x() < xll)
            xll = p.x();
        if(p.y() < yll)
            yll = p.y();
        if(p.x() > xur)
            xur = p.x();
        if(p.y() > yur)
            yur = p.y();

        _LL = Point_E2<NT>(xll,yll);
        _UR = Point_E2<NT>(xur,yur);

        return *this;
    }

    bool operator==(const Bbox_E2<NT>& box) const
    {
        return box._LL == _LL && box._UR == _UR;
    }

    Point_E2<NT> LL() const { return _LL; }
    Point_E2<NT> UR() const { return _UR; }

    Point_E2<NT> center() const { return Point_E2<NT>((_LL.x() + _UR.x())/2.0,
                                                      (_LL.y() + _UR.y())/2.0); }

    // Bbox is to the left of the four oriented lines
    Line_E2<NT> getEastLine() const
    {
        return Line_E2<NT>(Point_E2<NT>(_UR.x(), _LL.y()),
                           _UR);
    }
    Line_E2<NT> getNorthLine() const
    {
        return Line_E2<NT>(_UR,
                           Point_E2<NT>(_LL.x(), _UR.y()));
    }
    Line_E2<NT> getWestLine() const
    {
        return Line_E2<NT>(Point_E2<NT>(_LL.x(), _UR.y()),
                           _LL);
    }
    Line_E2<NT> getSouthLine() const
    {
        return Line_E2<NT>(_LL,
                           Point_E2<NT>(_UR.x(), _LL.y()));
    }
    
};

template<class S>
std::ostream&
operator<<(std::ostream& os, const Bbox_E2<S>& B)
{
    return os << B.LL() << " --- " << B.UR();
}

#endif // BBOX_E2_H
