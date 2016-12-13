// Sherif Ghali

#ifndef POLYGON_E3_H
#define POLYGON_E3_H

#include <vector>
#include "geometry_E3/point_e3.hpp"
#include "geometry_E3/bbox_e3.hpp"

template<typename NT>
class Transformation_E3;

template<typename NT>
struct Polygon_E3
{
    std::vector<Point_E3<NT> > vertices;
public:
    friend class Transformation_E3<NT>;

    Polygon_E3() : vertices() {}
    Polygon_E3(int n) : vertices(n) {}
    template<typename iterator>
    Polygon_E3(iterator b, iterator e) : vertices(b,e) {}
    Polygon_E3(const Point_E3<NT> P0,
               const Point_E3<NT> P1,
               const Point_E3<NT> P2,
               const Point_E3<NT> P3)
    {
        vertices.push_back(P0);
        vertices.push_back(P1);
        vertices.push_back(P2);
        vertices.push_back(P3);
    }

    void clear() { vertices.clear(); }
    bool isEmpty() const { return !vertices.size(); }
    unsigned int size() const { return vertices.size(); }
    void push_back(const Point_E3<NT>& p) { vertices.push_back(p); }
    void pop_back() { vertices.pop_back(); }

    Point_E3<NT>& operator[](int i) { return vertices[i]; }
    const Point_E3<NT>& operator[](int i) const { return vertices[i]; }

    void update_last_point(const Point_E3<NT>& p)
    {
        vertices[vertices.size()-1] = p;
    }

    Bbox_E3<NT> get_Bbox() const
    {
        // precondition: polygon has at least one vertex
        Bbox_E3<NT> box = vertices[0];
        for(unsigned int i=1; i<vertices.size(); ++i)
            box += vertices[i];
        return box;
    }
};

#endif // POLYGON_E3_H
