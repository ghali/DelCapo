// Sherif Ghali

#ifndef QDELCAPO_WINDOW_H
#define QDELCAPO_WINDOW_H

// Qt
#include <QWidget>
class QLCDNumber;
class QPushButton;
class QCheckBox;

#include "../misc/conversions.h"
#include "delcapo/Qdelcapo_subwindow.h"

class QDelCapo_Subwindow;

// QDelCapo_Window is the main game window.
// QDelCapo_Window is the decorated window (including widgets).
// QDelCapo_Subwindow, an instance of QGLWidget, is one of these widgets.

class QDelCapo_Window : public QWidget
{
    Q_OBJECT

public:
    QDelCapo_Window( Planar_Graph * pg );
    virtual ~QDelCapo_Window() {}
    void add_to_score(int i);
    void writeImage(const string& filename);

private slots:
    void wireframeStateChanged(bool wireframe);
    void debugModeStateChanged(bool debugMode);

private:
    QDelCapo_Subwindow * myQDelCapo_Subwindow;

    QLCDNumber  * qlcdnumber;
    QCheckBox   * wireframeCheckBox;
    QCheckBox   * debugModeCheckBox;
    QPushButton * quitButton;

    float lastSeconds;
};

#endif // QDELCAPO_WINDOW_H
