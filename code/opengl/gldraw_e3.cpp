// Sherif Ghali

#include <vector>
#include <stdexcept>

#include "gldraw_e3.hpp"

#include "misc/conversions.hpp"
#include "raster_image/array2.hpp"

#include "../types/lighting_constants.hpp"

// #include "colorimage.hpp"
#include "misc/color_4.hpp"
#include "raster_image/image_color_4.hpp"

// const Color_4f FAKE_SHADOW = BACKGROUND;

//----------------------------------------------------------------
void
GLdraw_E3d::draw_axes(GLdouble len)
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES); {
        GLdraw::set_color(RED);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(len, 0.0, 0.0);
        GLdraw::set_color(GREEN);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, len, 0.0);
        GLdraw::set_color(BLUE);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, len);
    } glEnd();
}

//----------------------------------------------------------------
void
GLdraw_E3d::gen_polygon(const Polygon_E3d& poly)
{
    assert(poly.size() == 4);

    Vector_E3d N = polygon_normal_vector<double>(poly);
    glEnable(GL_NORMALIZE);

    glBegin(GL_POLYGON); {
        glNormal3d(N.x(), N.y(), N.z());
        glVertex3d(poly[0].x(), poly[0].y(), poly[0].z());
        glVertex3d(poly[1].x(), poly[1].y(), poly[1].z());
        glVertex3d(poly[2].x(), poly[2].y(), poly[2].z());
        glVertex3d(poly[3].x(), poly[3].y(), poly[3].z());
    } glEnd();
}

//----------------------------------------------------------------
void
GLdraw_E3d::gen_triangle_strips(const Polygon_E3d& poly, int divx, int divy)
// divx and divy are the numbers of the regions (triangle pairs), not vertices.
{
    assert(poly.size() == 4);

    Vector_E3d N = polygon_normal_vector<double>(poly);
    glEnable(GL_NORMALIZE);

    Array2<Point_E3d> A = quad_subdivide(poly, divx, divy); // in interpolation_e3.h

    for(int x = 0; x <= divx-1; ++x) {
        glBegin(GL_TRIANGLE_STRIP); {
            glNormal3d(N.x(), N.y(), N.z());
            for(int y = 0; y <= divy; ++y) {
                //std::cout << A(x,y) << "    ";
                glVertex3f( A(x  ,y).x(), A(x  ,y).y(), A(x  ,y).z() );
                glVertex3f( A(x+1,y).x(), A(x+1,y).y(), A(x+1,y).z() );
            }
        } glEnd();
        //std::cout << std::endl;
    }
}

//----------------------------------------------------------------
void
GLdraw_E3d::gen_cube(double half_dimension, int divx, int divy)
{
    const double h = half_dimension;
    Point_E3d P0(-h ,  h ,  h);
    Point_E3d P1( h ,  h ,  h);
    Point_E3d P2( h , -h ,  h);
    Point_E3d P3(-h , -h ,  h);
    Point_E3d P4(-h ,  h , -h);
    Point_E3d P5( h ,  h , -h);
    Point_E3d P6( h , -h , -h);
    Point_E3d P7(-h , -h , -h);

    Polygon_E3d q0(P3, P2, P1, P0);
    Polygon_E3d q1(P0, P1, P5, P4);
    Polygon_E3d q2(P4, P5, P6, P7);
    Polygon_E3d q3(P7, P6, P2, P3);
    Polygon_E3d q4(P2, P6, P5, P1);
    Polygon_E3d q5(P7, P3, P0, P4);

    gen_triangle_strips(q0, divx, divy);
    gen_triangle_strips(q1, divx, divy);
    gen_triangle_strips(q2, divx, divy);
    gen_triangle_strips(q3, divx, divy);
    gen_triangle_strips(q4, divx, divy);
    gen_triangle_strips(q5, divx, divy);
}

//----------------------------------------------------------------
#ifndef CALLBACK
#define CALLBACK
#endif

void CALLBACK error_callback()
{
    // a bit too aggressive:
    throw std::domain_error("Quadric error callback called.");
}

void
GLdraw_E3d::gen_Y_sphere(double radius, int longitudes, int latitudes)
{
    GLUquadricObj * quadric = gluNewQuadric();
    if(!quadric)
        throw std::domain_error("No memory to allocate quadric sphere object.");

    gluQuadricCallback(quadric, GLU_ERROR, error_callback);
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluSphere(quadric, radius, longitudes, latitudes);

    gluDeleteQuadric(quadric);
}

void
GLdraw_E3d::gen_Z_cylinder(GLdouble base,
                           GLdouble top,
                           GLdouble height,
                           GLint slices,
                           GLint stacks)
{
    GLUquadricObj * quadric = gluNewQuadric();
    if(!quadric)
        throw std::domain_error("No memory to allocate quadric cylinder object.");

    gluQuadricCallback(quadric, GLU_ERROR, error_callback);
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluCylinder(quadric, base, top, height, slices, stacks);

    gluDeleteQuadric(quadric);
}

void
GLdraw_E3d::gen_Z_cone(double base, double height, int slices, int stacks)
{
    GLUquadricObj * quadric = gluNewQuadric();
    if(!quadric)
        throw std::domain_error("No memory to allocate quadric cone object.");

    gluQuadricCallback(quadric, GLU_ERROR, error_callback);
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluCylinder(quadric, base, 0.0, height, slices, stacks);

    gluDeleteQuadric(quadric);
}

void
GLdraw_E3d::gen_Z_disk(GLdouble inner,
                       GLdouble outer,
                       GLint slices,
                       GLint loops,
                       GLenum orientation) // GLU_OUTSIDE (+z) / GLU_INSIDE (-z)
{
    GLUquadricObj * quadric = gluNewQuadric();
    if(!quadric)
        throw std::domain_error("No memory to allocate quadric disk object.");

    gluQuadricCallback(quadric, GLU_ERROR, error_callback);
    gluQuadricDrawStyle(quadric, GLU_FILL);
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricOrientation(quadric, orientation);
    gluDisk(quadric, inner, outer, slices, loops);

    gluDeleteQuadric(quadric);
}

//----------------------------------------------------------------
std::vector<Point_E2d>
GLdraw_E3d::divide_circle(int divisions, double radius)
{
    std::vector<Point_E2d > vertices;

    const double first_angle = M_PI / double(divisions);
    const double cos_first_angle = cos(first_angle);

    for(int v=0; v<divisions; ++v) {
        double angle = 2.0 * M_PI / double(divisions) * double(v) + M_PI / double(divisions);
        double c = radius * cos(angle) / cos_first_angle;
        if(fabs(c) < 1e-10) c = 0;
        double s = radius * sin(angle) / cos_first_angle;
        if(fabs(s) < 1e-10) s = 0;
        vertices.push_back(Point_E2d(c,s));
    }

    return vertices;
}

//----------------------------------------------------------------
template<typename Point_input_iterator>
void
GLdraw_E3d::gen_Y_triangle_fan(Point_input_iterator P_begin,
                               Point_input_iterator P_end,
                               double Y_normal, // +/- 1.0
                               double Y)
{
    glBegin(GL_TRIANGLE_FAN); {
        glNormal3d(0.0, Y_normal, 0.0);
        glVertex3d(0, Y, 0);
        for(Point_input_iterator v=P_begin; v!=P_end; ++v)
            glVertex3d( v->x(), Y, v->y() );
        Point_input_iterator v = P_begin;
        glVertex3d( v->x(), Y, v->y() ); // close
    } glEnd();
}

//----------------------------------------------------------------
void
GLdraw_E3d::gen_Y_triangle_fan(int divisions, double radius)
{
    // Vertices
    std::vector<Point_E2d > vertices = divide_circle(divisions, radius);

    // Top: y = +half_height
    typedef std::vector<Point_E2d>::iterator Point_forward_iterator;
    gen_Y_triangle_fan<Point_forward_iterator>(vertices.begin(), vertices.end(), +1.0, 0.0);
}

//----------------------------------------------------------------
//void
//GLdraw_E3d::gen_Y_cylinder_with_caps(double radius, double half_height, 
//                                     int slices, int stacks)
//// Caps are parallel to the xz-plane.
//// y for top is +half_height. y for bottom is -half_height.
//{
//    // Vertices
//    std::vector<Point_E2d > vertices = divide_circle(slices, radius);
//
//    // Top: y = +half_height
//    typedef std::vector<Point_E2d>::iterator Point_forward_iterator;
//    gen_Y_triangle_fan<Point_forward_iterator>(vertices.begin(), vertices.end(), +1.0, +half_height);
//
//    // Bottom: y = -half_height
//    typedef std::vector<Point_E2d>::reverse_iterator Point_reverse_iterator;
//    gen_Y_triangle_fan<Point_reverse_iterator>(vertices.rbegin(), vertices.rend(), -1.0, +half_height);
//
//    gen_Z_cylinder(radius, radius, half_height * 2.0, slices, stacks);
// }

//----------------------------------------------------------------
void
GLdraw_E3d::set_static_light(const GLenum    & light,
                             const Color_3f  & ambient,
                             const Color_3f  & diffuse,
                             const Color_3f  & specular,
                             const Point_T3d & position)
{
    const GLfloat a[] = { ambient.r() , ambient.g() , ambient.b(), 1.0 };
    const GLfloat d[] = { diffuse.r() , diffuse.g() , diffuse.b(), 1.0 };
    const GLfloat s[] = { specular.r(), specular.g(), specular.b(), 1.0 };

    const GLfloat p[] = { float(position.hx()), float(position.hy()), float(position.hz()), float(position.hw()) };

    glEnable(light);

    glLightfv(light, GL_AMBIENT , a);
    glLightfv(light, GL_DIFFUSE , d);
    glLightfv(light, GL_SPECULAR, s);

    glLightfv(light, GL_POSITION, p);
}

//----------------------------------------------------------------
#if 0
void
GLdraw_E3d::begin_fake_shadows_on_Y_0(const Point_T3d & L)
{
    assert(L.hw() == 1.0);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    GLdraw::set_color(FAKE_SHADOW);

    // For readability, we store the matrix in row-order...
    GLdouble shadowMatrix[16] =
    {
        1, -L.hx()/L.hy(), 0,     0,
        0,              0, 0, 0.001,
        0, -L.hz()/L.hy(), 1,     0,
        0,    -1.0/L.hy(), 0,     1
    };

    // but then multiply by the transpose (OpenGL expects column order).
    glMatrixMode(GL_MODELVIEW);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    { } glPushMatrix();
    {     } glMultMatrixd(shadowMatrix);
}

void
GLdraw_E3d::begin_fake_shadows_on_Z_0(const Point_T3d & L)
{
    assert(L.hw() == 1.0);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    GLdraw::set_color(FAKE_SHADOW);

    // For readability, we store the matrix in row-order...
    GLdouble shadowMatrix[16] =
    {
        1, 0, -L.hx()/L.hz(),     0,
        0, 1, -L.hy()/L.hz(),     0,
        0, 0,              0, 0.001,
        0, 0,    -1.0/L.hz(),     1
    };

    // but then multiply by the transpose (OpenGL expects column order).
    glMatrixMode(GL_MODELVIEW);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    { } glPushMatrix();
    {     } glMultTransposeMatrixd(shadowMatrix);
}

void
GLdraw_E3d::end_fake_shadows()
{
    { } glPopMatrix();
    glPopAttrib();

    glEnable(GL_LIGHTING);
}
#endif

GLuint
GLdraw_E3d::initTexture(const char * imageFile)
{
    // ColorImage<GLubyte> textureImage(imageFile);
    Image<Color_4<GLubyte> > textureImage(imageFile);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLuint texName;
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 textureImage.nrows(), textureImage.ncols(),
                 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 textureImage.getData());

    return texName;
}
