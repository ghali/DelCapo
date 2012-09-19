// Sherif Ghali

#ifndef SEGMENT_E3_H
#define SEGMENT_E3_H

#include "geometry_E3/point_e3.h"
#include "geometry_E3/line_e3.h"

template<typename T>
class Segment_E3;

template<typename T>
ostream& operator<<(ostream& os, const Segment_E3<T>& seg);

template<typename T>
class Segment_E3
{
private:
    Point_E3<T> src;         // first or source point of line segment
    Point_E3<T> tgt;         // second or target point of line segment
public:
    Segment_E3() : src(), tgt() {}

    Segment_E3(const Point_E3<T>& src, const Point_E3<T>& tgt): src(src), tgt(tgt)
    {}

    const Point_E3<T>& source() const { return src; }
    const Point_E3<T>& target() const { return tgt; }

    Line_E3<T> getLine_E3() const { return Line_E3<T>(src, tgt); }

    friend ostream& operator<< <>(ostream& os, const Segment_E3<T>& seg);
};

template<typename T>
ostream& operator<<(ostream& os, const Segment_E3<T>& seg)
{
    return os << seg.src << "--" << seg.tgt;
}

#endif // SEGMENT_E3_H
