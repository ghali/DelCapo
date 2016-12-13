// xplatform_glut.h

#ifndef XPLATFORM_GLUT_H
#define XPLATFORM_GLUT_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glut.h>
#endif

#endif // XPLATFORM_GLUT_H
