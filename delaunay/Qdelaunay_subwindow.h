// Sherif Ghali

#ifndef QDELAUNAY_SUBWINDOW_H
#define QDELAUNAY_SUBWINDOW_H

// Qt
#include <QGLWidget>
#include <QTime>

// QDelCapo
#include "../types/geometry_types.h"
#include "../delaunay/delaunay.h"
#include "../planar_graph/planar_graph.h"

class QDelaunay_Window;
class QDelCapo_Window;

// QDelaunay_Subwindow relies on
// template< typename Point_input_iterator > class Delaunay
// to compute the triangulation.

class QDelaunay_Subwindow : public QGLWidget
{
    Q_OBJECT

    public:
    QDelaunay_Subwindow(const Vector_E2i INITIAL_VERTEX_GRID,
                        QWidget *parent = 0);

    virtual ~QDelaunay_Subwindow() {}

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void new_graph();

    void XsizeChanged(int xsize);
    void YsizeChanged(int ysize);

    void compute_delaunay();
    void launch_game();
    void reveal_tree(bool b);

protected:                      // inherited
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();

    virtual void enterEvent(QEvent * event);
    virtual void leaveEvent(QEvent * event);

    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void keyPressEvent(QKeyEvent * event);

private:
    void draw_delaunay();

    Vector_E2i vertex_grid;
    Vector_E2i viewport;

    Planar_Graph PG;

    bool tree_is_revealed;
    bool mouse_is_inside_subwindow;
    Point_E2d mouse_location;

    QDelCapo_Window * qdelcapo_window;
};

#endif // QDELAUNAY_SUBWINDOW_H
