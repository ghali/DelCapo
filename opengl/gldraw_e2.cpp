// Sherif Ghali

#include "gldraw_e2.h"

#include <vector>

#include "misc/conversions.h"
#include "geometry_E2/point_e2.h"
#include "geometry_E2/segment_e2.h"
#include "geometry_E2/polygon_e2.h"

void
GLdraw_E2::draw(const std::vector<Point_E2d> & points,
                const QColor & pointColor,
                float pointSize)
{
    glPointSize(pointSize);
    set_color(pointColor);
    glBegin(GL_POINTS); {
        typedef std::vector<Point_E2d>::const_iterator PI;
        for(PI i=points.begin(); i!=points.end(); ++i)
            glVertex2f( to_float(i->x()), to_float(i->y()) );
    } glEnd();
}

void
GLdraw_E2::draw(const std::vector<Segment_E2d> & segments,
                const QColor & lineColor,
                float lineWidth)
{
    glLineWidth(lineWidth);
    set_color(lineColor);

    glBegin(GL_LINES); {
        typedef std::vector<Segment_E2d>::const_iterator SI;
        for(SI i=segments.begin(); i!=segments.end(); ++i) {
            glVertex2f( to_float(i->source().x()), to_float(i->source().y()) );
            glVertex2f( to_float(i->target().x()), to_float(i->target().y()) );
        }
    } glEnd();
}

void
GLdraw_E2::draw(const std::vector<Polygon_E2d> & polygons,
                const QColor & lineColor,
                float lineWidth)
{
    glLineWidth(lineWidth);
    set_color(lineColor);

    typedef std::vector<Polygon_E2d>::const_iterator PI;
    for(PI pi=polygons.begin(); pi!=polygons.end(); ++pi) {
        glBegin(GL_LINE_LOOP); {
            typedef Polygon_E2d::Point_const_iterator PCI;
            for(PCI pci = pi->begin(); pci != pi->end(); ++pci)
                glVertex2f( to_float(pci->x()),
                            to_float(pci->y()) );
        } glEnd();
    }
}

void
GLdraw_E2::draw(const Point_E2d & p,
                const QColor & pointColor,
                float pointSize)
{
    glPointSize(pointSize);
    set_color(pointColor);
    glBegin(GL_POINTS); {
        glVertex2f( to_float(p.x()), to_float(p.y()) );
    } glEnd();
}

void
GLdraw_E2::draw(const Point_E2d & source, const Point_E2d & target,
                const QColor & segmentColor,
                float lineWidth,
                bool drawEndpoints,
                const QColor & pointColor,
                float pointSize)
{
    glLineWidth(lineWidth);

    set_color(segmentColor);
    glBegin(GL_LINES); {
        glVertex2f( to_float(source.x()), to_float(source.y()) );
        glVertex2f( to_float(target.x()), to_float(target.y()) );
    } glEnd();

    if(drawEndpoints) {
        draw(source, pointColor, pointSize);
        draw(target, pointColor, pointSize);
    }
}

void
GLdraw_E2::draw(const Segment_E2d & s,
                const QColor & segmentColor,
                float lineWidth,
                bool drawEndpoints,
                const QColor & pointColor,
                float pointSize)
{
    draw(s.source(), s.target(),
         segmentColor, lineWidth, drawEndpoints,
         pointColor, pointSize);
}

void
GLdraw_E2::draw(const Polygon_E2d & polygon,
                const QColor & segmentColor,
                const QColor & pointColor,
                float lineWidth,
                float pointSize,
                bool filled,
                bool drawEndpoints)
{
    glLineWidth(lineWidth);
    set_color(segmentColor);
    glBegin(filled ? GL_POLYGON : GL_LINE_LOOP); {
        for(unsigned int i = 0; i != polygon.size(); ++i)
            glVertex2f( to_float(polygon[i].x()), to_float(polygon[i].y()) );
    } glEnd();

    glPointSize(pointSize);
    set_color(pointColor);
    if(drawEndpoints) {
        for(unsigned int i = 0; i != polygon.size(); ++i) {
            glBegin(GL_POINTS); {
                glVertex2f( to_float(polygon[i].x()), to_float(polygon[i].y()) );
            } glEnd();
        }
    }
}

void
GLdraw_E2::draw_polyline(const Polygon_E2d & polygon,
                         bool two_colors,
                         const QColor & segmentColor,
                         const QColor & pointColor1,
                         float lineWidth,
                         float pointSize,
                         bool drawEndpoints,
                         int mod_for_first,
                         const QColor & pointColor2)
{
    glLineWidth(lineWidth);
    set_color(segmentColor);
    glBegin(GL_LINE_STRIP); {
        for(unsigned int i = 0; i != polygon.size(); ++i)
            glVertex2f( to_float(polygon[i].x()), to_float(polygon[i].y()) );
    } glEnd();

    glPointSize(pointSize);
    if(!two_colors)
        set_color(pointColor1);
    if(drawEndpoints) {
        for(unsigned int i = 0; i != polygon.size(); ++i) {
            glBegin(GL_POINTS); {
                if(two_colors)
                    if(i%mod_for_first == 0)
                        set_color(pointColor1);
                    else
                        set_color(pointColor2);
                glVertex2f( to_float(polygon[i].x()), to_float(polygon[i].y()) );
            } glEnd();
        }
    }
}

void
GLdraw_E2::setup_projection(const Bbox_E2d & bbox)
{
    GLdraw_E2::setup_projection(bbox.LL(), bbox.UR());
}

void
GLdraw_E2::setup_projection(const Point_E2d & wmin, const Point_E2d & wmax)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(to_double(wmin.x()), to_double(wmax.x()),
               to_double(wmin.y()), to_double(wmax.y()));

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0);
    glPointSize(7.0);
}

Point_E2d
GLdraw_E2::unproject(const Point_E2i & pin)
{
    GLint viewport[4];
    GLdouble mvmatrix[16], projmatrix[16];
    GLint realy;            //  OpenGL y coordinate position
    GLdouble wx, wy, wz;    // returned world x, y, z coords

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

    // viewport[3] is the height of the window in pixels.
    realy = viewport[3] - (GLint) pin.y() - 1;

    gluUnProject((GLdouble) pin.x(), (GLdouble) realy,
                 0.0,         // Ortho2D means any z-value will do
                 mvmatrix, projmatrix, viewport, &wx, &wy, &wz); 

    return Point_E2d(wx, wy);
}
