// Sherif Ghali

#ifndef LINE_E2_H
#define LINE_E2_H

#include "geometry_E2/point_e2.hpp"
#include "geometry_E2/segment_e2.hpp"
#include "misc/determinant.hpp"

template<typename NT>
class Line_E2;

template<typename NT>
ostream& operator<<(ostream& os, const Line_E2<NT>& seg);

template<typename NT>
class Line_E2
{
private:
    NT _a, _b, _c;              // ax+by+c=0

    void init_abc(const Point_E2<NT>& src, const Point_E2<NT>& tgt)
    {
        _a = + determinant(src.y(), NT(1.0), tgt.y(), NT(1.0));
        _b = - determinant(src.x(), NT(1.0), tgt.x(), NT(1.0));
        _c = + determinant(src.x(), src.y(), tgt.x(), tgt.y());
    }
public:
    Line_E2() : _a(0), _b(1), _c(0) {}
    Line_E2(const Point_E2<NT>& src, const Point_E2<NT>& tgt)
    {
        init_abc(src, tgt);
    }
    Line_E2(const Segment_E2<NT>& seg)
    {
        init_abc(seg.source(), seg.target());
    }

    NT a() const { return _a; }
    NT b() const { return _b; }
    NT c() const { return _c; }

    bool operator==(const Line_E2<NT>& L) const {
        return (this == &L) ||
            (are_dependent(_a, _b, _c, L._a, L._b, L._c) &&
             inner_product(_a, _b, _c, L._a, L._b, L._c) > 0);
    }

    bool operator!=(const Line_E2<NT>& L) const {
        return ! operator==(L);
    }

    NT slope() const {
        return _b / _a;
    }

    bool is_parallel(const Line_E2<NT>& L) const
    {
        return slope() == L.slope();
    }

    friend ostream& operator<< <>(ostream& os, const Line_E2<NT>& seg);
};

template<typename NT>
ostream& operator<<(ostream& os, const Line_E2<NT>& seg)
{
    return os << seg._a << " " << seg._b << " " << seg._c;
}

#endif // LINE_E2_H
