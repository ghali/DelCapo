// Sherif Ghali

#ifndef QDELAUNAY_WINDOW_H
#define QDELAUNAY_WINDOW_H

// Qt
#include <QWidget>

class QLCDNumber;
class QPushButton;
class QCheckBox;

// QDelCapo
#include "../misc/conversions.h"
#include "Qdelaunay_subwindow.h"

// QDelaunay_Window is the decorated window (including widgets).
// QDelaunay_Subwindow, an instance of QGLWidget, is one of these widgets.

class QDelaunay_Window : public QWidget
{
    Q_OBJECT

public:
    QDelaunay_Window();
    virtual ~QDelaunay_Window() {} // Qt cleans up the buttons

private slots:
    void launch_game();
    void reveal_tree();
    void XsizeChanged(int x);
    void YsizeChanged(int y);

private:
    QDelaunay_Subwindow * myQDelaunay_Subwindow;

    QSlider     * Xslider;
    QSlider     * Yslider;
    QPushButton * launchButton;
    QCheckBox   * revealTreeCheckBox;
    QPushButton * quitButton;
};

#endif // QDELAUNAY_WINDOW_H
