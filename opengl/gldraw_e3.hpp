// Sherif Ghali

#ifndef GLDRAW_E3_H
#define GLDRAW_E3_H

#include <QtOpenGL>

#include "xplatform_glu.hpp"

#include "../types/color_types.hpp"
#include "../types/geometry_types.hpp"
#include "gldraw.hpp"

struct GLdraw_E3d : public GLdraw
{
    //----------------------------------------------------------------
    static void
    draw_axes(GLdouble len);

    //----------------------------------------------------------------
    static void
    gen_polygon(const Polygon_E3d& poly);

    //----------------------------------------------------------------
    static void
    gen_triangle_strips(const Polygon_E3d& poly, int divx = 5, int divy = 5);
    // divx and divy are the numbers of the regions (triangle pairs), not vertices.

    //----------------------------------------------------------------
    static void
    gen_cube(double half_dimension, int divx = 1, int divy = 1);
    // divx and divy are the # of the regions.

    //----------------------------------------------------------------
    static std::vector<Point_E2d>
    divide_circle(int divisions, double radius);

    template<typename Point_input_iterator>
    static
    void
    gen_Y_triangle_fan(Point_input_iterator P_begin,
                       Point_input_iterator P_end,
                       double Y_normal, // +/- 1.0
                       double Y);

    static void
    gen_Y_triangle_fan(int divisions, double radius);

    //----------------------------------------------------------------
    static void
    gen_Y_sphere(double radius, int longitudes, int latitudes);

    static void
    gen_Z_cylinder(GLdouble base,
                   GLdouble top, GLdouble height,
                   GLint slices, GLint stacks);

    static void
    gen_Z_cone(double base, double height, int slices, int stacks);

    static void
    gen_Z_disk(GLdouble inner,
               GLdouble outer,
               GLint slices,
               GLint loops,
               GLenum orientation); // GLU_OUTSIDE / GLU_INSIDE

    //----------------------------------------------------------------
    // static void
    // gen_Y_cylinder_with_caps(double radius, double half_height, 
    //                          int slices, int stacks);
    // Caps are parallel to the xz-plane.
    // y for top is +half_height. y for bottom is -half_height.

    //----------------------------------------------------------------
    static void
    set_static_light(const GLenum    & light,
                     const Color_3f  & ambient,
                     const Color_3f  & diffuse,
                     const Color_3f  & specular,
                     const Point_T3d & position);
#if 0
    static void
    begin_fake_shadows_on_Y_0(const Point_T3d & L);

    static void
    begin_fake_shadows_on_Z_0(const Point_T3d & L);

    static void
    end_fake_shadows();
#endif

    static GLuint
    initTexture(const char * imageFile);
};

#endif // GLDRAW_E3_H
