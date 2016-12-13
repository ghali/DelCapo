// Sherif Ghali

// Qt
#include <QLCDNumber>
#include <QtWidgets>
#include <QVBoxLayout>
#include <QHBoxLayout>

// QDelCapo
#include "Qdelaunay_window.hpp"
#include "Qdelaunay_subwindow.hpp"

const QPoint delaunay_hint_to_window_manager(5,25);

const Vector_E2i INITIAL_VERTEX_GRID(5, 5); 

const int RANGE_MIN =  1; // converted to an odd number below:
const int RANGE_MAX = 40; // 2*x + 1, 2*y + 1

QDelaunay_Window::QDelaunay_Window()
{
    myQDelaunay_Subwindow = new QDelaunay_Subwindow(INITIAL_VERTEX_GRID, this);
    //----------------Main----------------
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(5,5,5,5);
    mainLayout->addWidget(myQDelaunay_Subwindow);

    QHBoxLayout *uiLayout = new QHBoxLayout;
    uiLayout->setContentsMargins(5,5,5,5);
    uiLayout->setSizeConstraint (QLayout::SetFixedSize);
    {
        //----------------XY----------------
        QGroupBox *xyGroupBox = new QGroupBox(QObject::tr("Size"));
        {
            QLabel *Xlabel = new QLabel(QObject::tr("X"));
            QLabel *Ylabel = new QLabel(QObject::tr("Y"));

            Xslider = new QSlider(Qt::Horizontal);
            Xslider->setRange(RANGE_MIN, RANGE_MAX);
            Xslider->setValue((INITIAL_VERTEX_GRID.x()-1)/2);
            Xslider->setTickInterval(5);
            Xslider->setTickPosition(QSlider::TicksBelow);
            connect(Xslider, SIGNAL(valueChanged(int)),
                    this, SLOT(XsizeChanged(int)));

            Yslider = new QSlider(Qt::Horizontal);
            Yslider->setRange(RANGE_MIN, RANGE_MAX);
            Yslider->setValue((INITIAL_VERTEX_GRID.y()-1)/2);
            Yslider->setTickInterval(5);
            Yslider->setTickPosition(QSlider::TicksBelow);
            connect(Yslider, SIGNAL(valueChanged(int)),
                    this, SLOT(YsizeChanged(int)));

            QGridLayout *grid_layout = new QGridLayout;
            grid_layout->addWidget(Xlabel, 0, 0, Qt::AlignTop);
            grid_layout->addWidget(Xslider, 0, 1);
            grid_layout->addWidget(Ylabel, 1, 0, Qt::AlignTop);
            grid_layout->addWidget(Yslider, 1, 1);

            xyGroupBox->setLayout(grid_layout);
        }
        //----------------Launch----------------
        QGroupBox *launchGroupBox = new QGroupBox();
        {
            QVBoxLayout *buttonsLayout = new QVBoxLayout;

            launchButton  = new QPushButton(tr("Launch"));
            connect(launchButton, SIGNAL(clicked()), this, SLOT(launch_game()));

            buttonsLayout->addWidget(launchButton);
            launchGroupBox->setLayout(buttonsLayout);
        }
        //----------------Admin----------------
        QGroupBox *adminGroupBox = new QGroupBox();
        {
            QVBoxLayout *buttonsLayout = new QVBoxLayout;

            revealTreeCheckBox = new QCheckBox(tr("Reveal Tree"));
            quitButton  = new QPushButton(tr("Quit"));
            connect(revealTreeCheckBox, SIGNAL(clicked()), this, SLOT(reveal_tree(void)));
            connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

            buttonsLayout->addWidget(revealTreeCheckBox);
            buttonsLayout->addWidget(quitButton);
            adminGroupBox->setLayout(buttonsLayout);
        }
        //----------------ui----------------
        uiLayout->addWidget(xyGroupBox);
        uiLayout->addStretch();
        uiLayout->addWidget(launchGroupBox);
        uiLayout->addStretch();
        uiLayout->addWidget(adminGroupBox);
    }
    //----------------Main----------------
    mainLayout->addLayout(uiLayout);

    setLayout(mainLayout);
    setWindowTitle(tr("DelCapo - Maze"));
    move(delaunay_hint_to_window_manager);
    // No GL commands can precede QDelaunay_Subwindow::initializeGL().
}

// Notify myQDelaunay_Subwindow that either slider has been changed.

void QDelaunay_Window::XsizeChanged(int x)
{
    myQDelaunay_Subwindow->XsizeChanged(2*x+1);
}
void QDelaunay_Window::YsizeChanged(int y)
{
    myQDelaunay_Subwindow->YsizeChanged(2*y+1);
}

// Since the planar graph is stored by the subwindow,
// QDelaunay_Window::launch_game merely acts to delegate the
// launch_game request to the subwindow.

void QDelaunay_Window::launch_game()
{
    myQDelaunay_Subwindow->launch_game();
}

void QDelaunay_Window::reveal_tree()
{
    myQDelaunay_Subwindow->reveal_tree(revealTreeCheckBox->isChecked());
}
