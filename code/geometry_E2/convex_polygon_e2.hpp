// Sherif Ghali

#ifndef CONVEX_POLYGON_E2_H
#define CONVEX_POLYGON_E2_H

#include <cassert>

#include "geometry_E2/point_e2.hpp"
#include "geometry_E2/line_e2.hpp"
#include "geometry_E2/triangle_e2.hpp"
#include "geometry_E2/bbox_e2.hpp"
#include "geometry_E2/predicates_e2.hpp"

// Convex_polygon_E2 is a model for BSP_geometry::BSP_convex_polytope

// The differences between Convex_polygon_E2 and Polygon_E2 are:
//   1- Convex_polygon_E2 stores the bounding halfspaces; vertices are implicit.
//   2- Convex_polygon_E2 may represent the empty set.
//   3- Convex_polygon_E2 stores whether each boundary is open or closed.


//----------------BSP_E2_convex_polygon_Begin----------------
template<typename NT>
class Convex_polygon_E2
{
    bool is_empty_var;

    std::vector<Segment_E2<NT> >  lines;
    std::vector<bool>  line_is_closed;
//----------------BSP_E2_convex_polygon_End----------------

    // For consistency with 3D solids:
    // Interior of polygon lies on the right (negative side) of boundary.
    // Exterior lies on the left (positive side).
    // The polygon is oriented clockwise.

public:
    Convex_polygon_E2() { is_empty_var = true; }

    Convex_polygon_E2(const Bbox_E2<NT>& box )
        : is_empty_var(false)
    {
        // UL     UR
        // 
        // LL     LR
        // 

        Point_E2<NT> LR(box.UR().x(), box.LL().y());
        Point_E2<NT> UL(box.LL().x(), box.UR().y());

        // clockwise:
        lines.push_back( Segment_E2<NT>(box.LL(), UL) );
        lines.push_back( Segment_E2<NT>(UL, box.UR()) );
        lines.push_back( Segment_E2<NT>(box.UR(), LR) );
        lines.push_back( Segment_E2<NT>(LR, box.LL()) );

        line_is_closed = std::vector<bool>(4, true);
    }

    Convex_polygon_E2(const std::vector<Segment_E2<NT> >& lines_in, 
                      const std::vector<bool>& line_is_closed_in)
        : is_empty_var(false),
          lines(lines_in), line_is_closed(line_is_closed_in)
    {}

    Convex_polygon_E2( const Triangle_E2<NT>& triangle )
        : is_empty_var(false)
    {
        const Point_E2<NT>& p0 = triangle.P0();
        const Point_E2<NT>& p1 = triangle.P1();
        const Point_E2<NT>& p2 = triangle.P2();

        lines.push_back(Segment_E2<NT>(p0, p1));
        lines.push_back(Segment_E2<NT>(p1, p2));
        lines.push_back(Segment_E2<NT>(p2, p0));

        line_is_closed = std::vector<bool>(3, true);

        // Confirm:
        //   p0, p1, p2 is a negative (right/clockwise) turn
        assert( oriented_side(p0, p1, p2) == ON_NEGATIVE_SIDE );
    }

    std::vector<Segment_E2<NT> >
    get_lines() const
    {
        return lines;
    }

    std::vector<Segment_E2<NT> >
    get_free_bounding_lines() const
    {
        std::vector<Segment_E2<NT> > returned_lines;

        typedef typename std::vector<Segment_E2<NT> >::const_iterator Line_it;
        typedef typename std::vector<bool>::const_iterator         Bool_it;

        Line_it line_it = lines.begin();
        Bool_it  bool_it = line_is_closed.begin();

        while(bool_it != line_is_closed.end()) {

            if(*bool_it)
                returned_lines.push_back(*line_it);

            ++line_it;
            ++bool_it;
        }
        return returned_lines;
    }

    bool is_empty() const { return is_empty_var; }

    Segment_E2<NT> get_line(int i) const { return lines[i]; }
    bool is_line_closed(int i) const { return line_is_closed[i]; }

    std::vector<bool>
    get_line_is_closed() const {
        return line_is_closed;
    }
};

#endif // CONVEX_POLYGON_E2_H
