// Sherif Ghali

#ifndef LIGHTING_CONSTANTS_H
#define LIGHTING_CONSTANTS_H

#include <QtOpenGL>

#include "color_types.hpp"
#include "geometry_types.hpp"

// Introducing a file for lighting constants rather than including
// them directly in the one file they are needed risks
// increasing/wasting compilation time, but it is convenient to have
// them in one place. They also probably will not be updated too
// frequently. If they are, consider reading them from a file.

// BACKGROUND is needed in both GLdraw_E3 and in QDelCapo_Subwindow
// (hence it has to remain here).

const Color_4f BACKGROUND(0.2, 0.2, 0.25, 1.0);

// Lighting
const Point_T3d LIGHT0_POSITION( 4.0, 5.0, 7.0, 1.0 );
const Point_T3d LIGHT1_POSITION( -4.0, 12.0, 7.0, 1.0 );

// All scene objects
const GLfloat SCENE_AMBIENT[]  = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat SCENE_SPECULAR[] = { 0.7, 0.7, 0.7, 1.0 };
const GLfloat SCENE_SHININESS[] = { 2.0 };

// Convenience
const Color_3f RED   (1.0, 0.0, 0.0);
const Color_3f GREEN (0.0, 1.0, 0.0);
const Color_3f BLUE  (0.0, 0.0, 1.0);

#endif // LIGHTING_CONSTANTS_H
