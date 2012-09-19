// Sherif Ghali

#include <stdexcept>
#include <iostream>

#include <QtOpenGL>

// #include "xplatform_glut.h"

#include "gldraw.h"

void
GLdraw::check_errors()
{
    GLenum errorCode = 0;
    bool errorFound = false;
    while((errorCode = glGetError()) != GL_NO_ERROR) {
        errorFound = true;
        std::cerr << "ERROR: " << errorCode
                  << "       " << gluErrorString(errorCode) << std::endl;
    }
    if(errorFound)
        throw std::domain_error("glGetError senses trouble brewing.");
}

void
GLdraw::clear_screen()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void
GLdraw::set_clear_color(const QColor & c)
{
    glClearColor(qtcolor_to_glcolor(c.red()  ),
                 qtcolor_to_glcolor(c.green()),
                 qtcolor_to_glcolor(c.blue() ),
                 qtcolor_to_glcolor(c.alpha()));
}

void
GLdraw::set_color(const QColor & c)
{
    glColor3f(qtcolor_to_glcolor(c.red()  ),
              qtcolor_to_glcolor(c.green()),
              qtcolor_to_glcolor(c.blue() ));
}

inline GLclampf
GLdraw::qtcolor_to_glcolor(int i)
{
    return double(i) / 255.0;
}


void
GLdraw::set_clear_color(const Color_4f & c)
{
    glClearColor(c.r(),c.g(),c.b(),c.a());
}

void
GLdraw::set_color(const Color_3f & c)
{
    glColor3f(c.r(),c.g(),c.b());
}

void
GLdraw::set_color(const Color_4f & c)
{
    glColor4f(c.r(),c.g(),c.b(),c.a());
}
