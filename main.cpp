// Sherif Ghali

// Qt
#include <QApplication>

// Qdelcapo
#include "delaunay/Qdelaunay_window.h"

// This is the launch point for the application.
// The secondary launch will be made by QDelaunay_Window to QDelCapo_Window.

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QDelaunay_Window window;
    window.show();
    return app.exec();
}
