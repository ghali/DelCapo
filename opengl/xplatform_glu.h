// xplatform_glu.h

#ifndef XPLATFORM_GLU_H
#define XPLATFORM_GLU_H

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glu.h>
#endif

#endif // XPLATFORM_GLU_H
