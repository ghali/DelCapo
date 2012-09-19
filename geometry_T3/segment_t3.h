// Sherif Ghali

#ifndef SEGMENT_T3_H
#define SEGMENT_T3_H

#include "geometry_T3/point_t3.h"

template<typename T>
class Segment_T3;

template<typename T>
ostream& operator<<(ostream& os, const Segment_T3<T>& seg);

template<typename T>
class Segment_T3
{
private:
    Point_T3<T> src;         // first or source point of line segment
    Point_T3<T> tgt;         // second or target point of line segment
public:
    Segment_T3() : src(), tgt() {}

    Segment_T3(const Point_T3<T>& src, const Point_T3<T>& tgt): src(src), tgt(tgt)
    {}

    const Point_T3<T>& source() const { return src; }
    const Point_T3<T>& target() const { return tgt; }

    bool operator==(const Segment_T3<T>& seg) const
    {
        return src == seg.src && tgt == seg.tgt;
    }

    bool operator!=(const Segment_T3<T>& seg) const
    {
        return !operator==(seg);
    }

    friend ostream& operator<< <>(ostream& os, const Segment_T3<T>& seg);
};

template<typename T>
ostream& operator<<(ostream& os, const Segment_T3<T>& seg)
{
    return os << seg.src << "--" << seg.tgt;
}

#endif // SEGMENT_T3_H
