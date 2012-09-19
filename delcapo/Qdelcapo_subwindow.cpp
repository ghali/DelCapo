// Sherif Ghali

// Qt
#include <QtGui>
#include <QtOpenGL>
#include <QCheckBox>

// STL
#include <unistd.h>

#include <cmath>
#include <cassert>

// QDelCapo
#include "../types/lighting_constants.h"

#include "delcapo/Qdelcapo_window.h"
#include "delcapo/Qdelcapo_subwindow.h"

#include "../types/geometry_types.h"
#include "../opengl/gldraw_e3.h"

// Window size
QSize qdelcapo_size_hint = QSize(640, 480);

// Redraw/process events are coming in every TIMER_INTERVAL.
const int TIMER_INTERVAL = 16;  // msec. -- normal machine
//const int TIMER_INTERVAL = 48;  // msec. -- ancient machine

// View
const GLdouble FIELD_OF_VIEW = 45.0;

// Lighting

// The LIGHT_POSITIONx constants are in lighting_constants
// (because they are also needed by GLdraw_E3 to draw shadows).

const Color_3f LIGHT0_AMBIENT (0.0, 0.0, 0.0);
//const Color_3f LIGHT0_DIFFUSE (0.0, 0.0, 0.0);
const Color_3f LIGHT0_DIFFUSE (0.2, 0.2, 0.2);
//const Color_3f LIGHT0_SPECULAR(0.1, 0.1, 0.1);
//0.0, 0.0, 0.0);
const Color_3f LIGHT0_SPECULAR(0.5, 0.5, 0.5);

const Color_3f LIGHT1_AMBIENT (0.0, 0.0, 0.0);
//const Color_3f LIGHT1_DIFFUSE (0.0, 0.0, 0.0);
const Color_3f LIGHT1_DIFFUSE (0.2, 0.2, 0.2);
//const Color_3f LIGHT1_SPECULAR(0.1, 0.1, 0.1);
//0.0, 0.0, 0.0);
const Color_3f LIGHT1_SPECULAR(0.5, 0.5, 0.5);


// QDelCapo_Subwindow launches the game by starting the timer.
// Whenever a timer click is generated, timerEvent() updates
// the GL display, which generates a call to updateGL().

QDelCapo_Subwindow::QDelCapo_Subwindow(Planar_Graph * pg,
                                       QWidget *parent)
    : QGLWidget(parent),
      gl_is_initialized(false),
      qtime(/* h: */ 0, /* m: */ 0)
{
    setFocusPolicy(Qt::StrongFocus);

    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));

    timer_number = startTimer(TIMER_INTERVAL);

    PG = pg;

    // initializeGL() is (logically, yet not actually) part of the constructor.
}

QDelCapo_Subwindow::~QDelCapo_Subwindow()
{ 
    killTimer(timer_number);
    delete scene;
}

QSize QDelCapo_Subwindow::minimumSizeHint() const      { return QSize(100, 150); }
QSize QDelCapo_Subwindow::sizeHint() const             { return qdelcapo_size_hint; }
void QDelCapo_Subwindow::wireframeStateChanged(bool w) { wireframe = w; if(gl_is_initialized) updateGL(); }
void QDelCapo_Subwindow::debugModeStateChanged(bool g) { is_in_debug_mode = g; if(gl_is_initialized) updateGL(); }
void QDelCapo_Subwindow::timerEvent(QTimerEvent*)      { updateGL(); }

void QDelCapo_Subwindow::initializeGL()
{
    gl_is_initialized = true;

    // Lighting
    glEnable(GL_LIGHTING);

    GLdraw_E3d::set_static_light(GL_LIGHT0,
                                 LIGHT0_AMBIENT ,
                                 LIGHT0_DIFFUSE ,
                                 LIGHT0_SPECULAR,
                                 LIGHT0_POSITION);

    GLdraw_E3d::set_static_light(GL_LIGHT1,
                                 LIGHT1_AMBIENT ,
                                 LIGHT1_DIFFUSE ,
                                 LIGHT1_SPECULAR,
                                 LIGHT1_POSITION);

    // Shading
    glShadeModel(GL_SMOOTH);
    // glShadeModel(GL_FLAT);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    // glLightModelfv(GL_LIGHT_MODEL_AMBIENT, SCENE_AMBIENT);

    // Misc
    glEnable(GL_DEPTH_TEST);
    GLdraw_E3d::set_clear_color(BACKGROUND);

    // Scene
    assert(PG != NULL);
    scene = new Scene(PG,
                      dynamic_cast<QDelCapo_Window *>(parent()),
                      eye_coi); // eye_coi is set by the maze.

    GLdraw_E3d::check_errors();
}

void
QDelCapo_Subwindow::set_up_projection_matrix(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW, double(width)/double(height), 1.0, 100.0);
}

void
QDelCapo_Subwindow::set_up_modelview_matrix()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    const Point_E3d eye = eye_coi.source();
    const Point_E3d coi = eye_coi.target();
    gluLookAt(eye.x(), eye.y(), eye.z(),
              coi.x(), coi.y(), coi.z(),
              0.0, 0.0, 1.0);
}

void QDelCapo_Subwindow::resizeGL(int width, int height)
{
    glViewport (0, 0, (GLsizei) width, (GLsizei) height);

    set_up_projection_matrix(width, height);
    set_up_modelview_matrix();

    GLdraw_E3d::check_errors();
}

void QDelCapo_Subwindow::paintGL()
{
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    eye_coi = scene->get_eye_coi();
    set_up_modelview_matrix();

    scene->paint(is_in_debug_mode);
    GLdraw_E3d::check_errors();

    glFlush();
#ifdef SAVING_FRAMES_FOR_ANIMATION
    images.push_back( QGLWidget::grabFrameBuffer() );
#endif
}

// Scene is the main class handling user input.
// keyPressEvent() just delegates the events.
void QDelCapo_Subwindow::keyPressEvent(QKeyEvent *event)
{
    // We do nothing if Qt::Key_Up is pressed.
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Down:
        scene->notify_player_of_turn( event->key() );
        updateGL();
        break;
    case Qt::Key_Space:
        scene->stop_player();
        break;
    case Qt::Key_A:
        scene->toggle_displaying_axes();
        break;
    case Qt::Key_N:
        scene->toggle_displaying_normals();
        break;
    case Qt::Key_Q: 
    case Qt::Key_Escape: 
        killTimer(timer_number);
#ifdef SAVING_FRAMES_FOR_ANIMATION
        for(unsigned int frame_number = 0; frame_number < images.size(); ++frame_number)
        {
            std::string f("imgs/f_");
            f += integer_to_zero_padded_string(frame_number) + ".png";
            QString filename(f.c_str());
            images[frame_number].save(filename);            
        }
#endif
        exit(0);
	break;
    default:
        QWidget::keyPressEvent(event);
    }
}
