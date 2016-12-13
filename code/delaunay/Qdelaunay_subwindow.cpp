// Sherif Ghali

// std
#include <cmath>

// Qt
#include <QtGui>
#include <QtOpenGL>
#include <QCheckBox>

// STL
#include <iterator>
#include <algorithm>
#include <functional>

// QDelCapo
#include "../types/geometry_types.hpp"
#include "../geometry_E2/interpolation_e2.hpp"
#include "../opengl/gldraw_e2.hpp"

#include "Qdelaunay_window.hpp"
#include "Qdelaunay_subwindow.hpp"

#include "../delcapo/Qdelcapo_window.hpp"

const QSize MY_SIZEHINT(640, 480); // 800, 850

const MyDouble HALF_CORRIDOR_WIDTH = 0.2;

const QColor qBACKGROUND = Qt::white;
const QColor qFOREGROUND = Qt::black;

QDelaunay_Subwindow::QDelaunay_Subwindow(Vector_E2i initial_vertex_grid,
                                         QWidget *parent)
    : QGLWidget(parent),
      vertex_grid(initial_vertex_grid),
      tree_is_revealed(false),
      mouse_is_inside_subwindow(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setFormat(QGLFormat(QGL::DoubleBuffer));
    setMouseTracking(true);

    new_graph();
}

QSize QDelaunay_Subwindow::minimumSizeHint() const
{
    return QSize(100, 150);
}
QSize QDelaunay_Subwindow::sizeHint() const
{
    return MY_SIZEHINT;
}

void QDelaunay_Subwindow::new_graph()
{
    PG = Planar_Graph(vertex_grid, HALF_CORRIDOR_WIDTH);
    resizeGL( viewport.x() /*width*/, viewport.y() /*height*/ );
    update();
}

void QDelaunay_Subwindow:: XsizeChanged(int xsize)
{
    vertex_grid = Vector_E2i(xsize, vertex_grid.y());
    new_graph();
}
void QDelaunay_Subwindow:: YsizeChanged(int ysize)
{
    vertex_grid = Vector_E2i(vertex_grid.x(), ysize);
    new_graph();
}

void QDelaunay_Subwindow::compute_delaunay()
{
    update();
}

// The instance of QDelCapo_Window is not created until the game is
// launched. It is not created along with an instance of
// QDelaunay_Subwindow. This is (partly) because a
// QGLWidget::initializeGL() would run when a QGLWidget is constructed
// and not when show() runs, so the actual planar graph must be known.

void QDelaunay_Subwindow::launch_game()
{
    qdelcapo_window = new QDelCapo_Window(&PG);
    qdelcapo_window->show();

    static_cast<QDelaunay_Window*>(parent())->hide();

    update();
}

void QDelaunay_Subwindow::reveal_tree(bool b)
{
    tree_is_revealed = b;
    update();
}

void QDelaunay_Subwindow::initializeGL()
{
    GLdraw_E2::set_clear_color(qBACKGROUND);
    GLdraw_E2::check_errors();
}

void QDelaunay_Subwindow::resizeGL(int width, int height)
{
    viewport = Vector_E2i(width, height);

    glViewport(0, 0, (GLint)width, (GLint)height);
    const Bbox_E2d window(Point_E2d(0,0), Point_E2d(width, height));

    const Point_E2d wmin(-2.0,-2.0);
    const Point_E2d wmax( (MyDouble) vertex_grid.x()+1,
                          (MyDouble) vertex_grid.y()+1 );
    const Bbox_E2d scene(wmin, wmax);

    const Bbox_E2d centered_scene = center(scene, window);
    GLdraw_E2::setup_projection(centered_scene);
    GLdraw_E2::check_errors();
}

void QDelaunay_Subwindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    PG.draw(mouse_is_inside_subwindow, mouse_location, tree_is_revealed);
    GLdraw_E2::check_errors();
}

void QDelaunay_Subwindow::enterEvent(QEvent *)
{
    mouse_is_inside_subwindow = true;
    // std::cout << "entering subwindow\n";
    update();
}

void QDelaunay_Subwindow::leaveEvent(QEvent *)
{
    mouse_is_inside_subwindow = false;
    // std::cout << "leaving subwindow\n\n";
    update();
}

void QDelaunay_Subwindow::mouseMoveEvent(QMouseEvent *event)
{
    const Point_E2i pe2i( event->x(), event->y() );
    mouse_location = GLdraw_E2::unproject(pe2i);
    update();
}

void QDelaunay_Subwindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q: 
    case Qt::Key_Escape: 
        exit(0);
	break;
    default:
        QWidget::keyPressEvent(event);
    }
}
