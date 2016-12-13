// Sherif Ghali

#ifndef QDELCAPO_SUBWINDOW_H
#define QDELCAPO_SUBWINDOW_H

// #define SAVING_FRAMES_FOR_ANIMATION

// Qt
#include <QGLWidget>
#include <QTime>

class QCheckBox;

// STL
#include <vector>

#include "../scene/scene.hpp"

// As can be seen in the UML class diagram, QDelCapo_Subwindow is
// referenced by an instance of QDelCapo_Window and references an
// instance of Scene.

class QDelCapo_Subwindow : public QGLWidget
{
    Q_OBJECT

public:
    QDelCapo_Subwindow(Planar_Graph * pg, QWidget *parent = 0);
    virtual ~QDelCapo_Subwindow();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void wireframeStateChanged(bool w);
    void debugModeStateChanged(bool g);

protected:                      // new in QDelCapo_Subwindow
    void
    set_up_projection_matrix(int width, int height);
    void set_up_modelview_matrix();

protected:                      // inherited from QGLWidget and its ancestors
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *event);

    void timerEvent(QTimerEvent*);

private:
    bool gl_is_initialized;
    bool wireframe;
    bool is_in_debug_mode;

    // Viewing
    Segment_E3d eye_coi;

    // Time & Timers
    QTime qtime;
    int timer_number;

    // Scene
    Scene * scene;
    Planar_Graph * PG;

#ifdef SAVING_FRAMES_FOR_ANIMATION
    std::vector<QImage> images;
#endif
};

#endif // QDELCAPO_SUBWINDOW_H
