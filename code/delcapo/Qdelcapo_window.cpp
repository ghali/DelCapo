// Sherif Ghali

#include <QLCDNumber>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>

#include "delcapo/Qdelcapo_window.hpp"
#include "delcapo/Qdelcapo_subwindow.hpp"

const QPoint qdelcapo_hint_to_window_manager(50,20);

QDelCapo_Window::QDelCapo_Window( Planar_Graph * pg )
{
    qlcdnumber = new QLCDNumber();
    qlcdnumber->setMode(QLCDNumber::Dec);
    qlcdnumber->setSegmentStyle(QLCDNumber::Flat);

    wireframeCheckBox = new QCheckBox(tr("Wireframe"));
    debugModeCheckBox = new QCheckBox(tr("Show Graph")); // "Debug Mode"
    quitButton = new QPushButton("Quit");

    myQDelCapo_Subwindow = new QDelCapo_Subwindow( pg, this );

    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;

    mainLayout->setContentsMargins(5,5,5,5);
    hLayout->setContentsMargins(5,5,5,5);

    mainLayout->addWidget(myQDelCapo_Subwindow);

    mainLayout->addLayout(hLayout);
    hLayout->addWidget(qlcdnumber);
    hLayout->addStretch();
    hLayout->addWidget(wireframeCheckBox);
    hLayout->addWidget(debugModeCheckBox);
    hLayout->addWidget(quitButton);
    setLayout(mainLayout);

    setWindowTitle(tr("QDelCapo"));

    connect(wireframeCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(wireframeStateChanged(bool)));
    wireframeCheckBox->setChecked(false);
    wireframeStateChanged(false);

    connect(debugModeCheckBox, SIGNAL(toggled(bool)),
            this, SLOT(debugModeStateChanged(bool)));
    debugModeCheckBox->setChecked(true);
    debugModeStateChanged(true);

    move(qdelcapo_hint_to_window_manager);
    // No GL commands can precede QDelCapo_Subwindow::initializeGL().
}

// Notify myQDelCapo_Subwindow if a button has been clicked.
void
QDelCapo_Window::wireframeStateChanged(bool wireframe)
{
    myQDelCapo_Subwindow->wireframeStateChanged(wireframe);
}

void
QDelCapo_Window::debugModeStateChanged(bool debugMode)
{
    myQDelCapo_Subwindow->debugModeStateChanged(debugMode);
}

void QDelCapo_Window::add_to_score(int i)
{
    qlcdnumber->display(qlcdnumber->intValue() + i);
}
