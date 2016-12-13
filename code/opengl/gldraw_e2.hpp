// Sherif Ghali

#ifndef GLDRAW_E2_H
#define GLDRAW_E2_H

#include <vector>

#include <QtGui>
#include <QtOpenGL>
#include "xplatform_glu.hpp"

#include "gldraw.hpp"
#include "misc/conversions.hpp"

#include "../types/geometry_types.hpp"

struct GLdraw_E2 : public GLdraw
{
    static void
    draw(const std::vector<Point_E2d> & points,
         const QColor & pointColor = Qt::black,
         float pointSize = 7.0);

    static void
    draw(const std::vector<Segment_E2d> & segments,
         const QColor & lineColor = Qt::black,
         float lineWidth = 3.0);

    static void
    draw(const std::vector<Polygon_E2d> & polygons,
         const QColor & lineColor = Qt::black,
         float lineWidth = 3.0);

    static void
    draw(const Point_E2d & p,
         const QColor & pointColor = Qt::black,
         float pointSize = 7.0);

    static void
    draw(const Point_E2d & source, const Point_E2d & target,
         const QColor & segmentColor = Qt::cyan,
         float lineWidth = 3.0,
         bool drawEndpoints = true,
         const QColor & pointColor = Qt::yellow,
         float pointSize = 7.0);

    static void
    draw(const Segment_E2d & s,
         const QColor & segmentColor = Qt::cyan,
         float lineWidth = 3.0,
         bool drawEndpoints = true,
         const QColor & pointColor = Qt::yellow,
         float pointSize = 7.0);

    static void
    draw(const Polygon_E2d & polygon,
         const QColor & segmentColor = Qt::cyan,
         const QColor & pointColor = Qt::yellow,
         float lineWidth = 3.0,
         float pointSize = 7.0,
         bool filled = false,
         bool drawEndpoints = true);

    static void
    draw_polyline(const Polygon_E2d & polygon,
                  bool two_colors = false,
                  const QColor & segmentColor = Qt::cyan,
                  const QColor & pointColor1 = Qt::yellow,
                  float lineWidth = 3.0,
                  float pointSize = 7.0,
                  bool drawEndpoints = true,
                  int mod_for_first = 3,
                  const QColor & pointColor2 = Qt::red);

    static void
    setup_projection(const Bbox_E2d & bbox);
    static void
    setup_projection(const Point_E2d & wmin, const Point_E2d & wmax);

    static Point_E2d
    unproject(const Point_E2i& pin);
};

#endif // GLDRAW_E2_H
