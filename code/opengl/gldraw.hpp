// Sherif Ghali

#ifndef GLDRAW_H
#define GLDRAW_H

#include <QtGui>
#include <QtOpenGL>
#include "xplatform_glu.hpp"

#include "../types/color_types.hpp"

struct GLdraw
{
    static void
    check_errors();

    static void
    clear_screen();

    static void
    set_clear_color(const QColor & c);
    static void
    set_color(const QColor & c);
    static inline GLclampf
    qtcolor_to_glcolor(int i);

    static void
    set_clear_color(const Color_4f & c);
    static void
    set_color(const Color_4f & c);
    static void
    set_color(const Color_3f & c);
};

#endif // GLDRAW_H
