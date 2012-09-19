######################################################################
# (Mostly) Automatically generated by qmake (2.01a) Mon Mar 30 13:56:13 2009
######################################################################

TEMPLATE = app
QT      += opengl

win32:INCLUDEPATH += C:/boost_1_50_0
mac:INCLUDEPATH   += /opt/local/include

# On Windows, we must set the PATH to:
#     C:/QtSDK/mingw/bin/:C:/QtSDK/Desktop/Qt/4.8.1/mingw/bin
# no need: win32:LIBS += ...

TARGET = 
DEPENDPATH += . \
              critters \
              delaunay \
              delcapo \
              geometry_E2 \
              geometry_E3 \
              geometry_lib \
              geometry_T3 \
              misc \
              opengl \
              planar_graph \
              raster_image \
              scene \
              solid_heds \
              types
INCLUDEPATH += . \
               delaunay \
               misc \
               types \
               geometry_E2 \
               geometry_lib \
               geometry_E3 \
               raster_image \
               geometry_T3 \
               planar_graph \
               solid_heds \
               critters \
               scene \
               opengl \
               delcapo

# Input
HEADERS += critters/critter.h \
           critters/intelligence.h \
           critters/player.h \
           critters/prey.h \
           critters/random.h \
           critters/two_d_to_three_d.h \
           delaunay/delaunay.h \
           delaunay/Qdelaunay_subwindow.h \
           delaunay/Qdelaunay_window.h \
           delcapo/Qdelcapo_subwindow.h \
           delcapo/Qdelcapo_window.h \
           geometry_E2/bbox_e2.h \
           geometry_E2/circle_e2.h \
           geometry_E2/convex_polygon_e2.h \
           geometry_E2/direction_e2.h \
           geometry_E2/dominant_e2.h \
           geometry_E2/geometry_e2.h \
           geometry_E2/interpolation_e2.h \
           geometry_E2/intersection_e2.h \
           geometry_E2/line_e2.h \
           geometry_E2/point_e2.h \
           geometry_E2/polygon_e2.h \
           geometry_E2/predicates_e2.h \
           geometry_E2/projection_e2.h \
           geometry_E2/ray_e2.h \
           geometry_E2/segment_e2.h \
           geometry_E2/transformation_e2.h \
           geometry_E2/triangle_e2.h \
           geometry_E2/vector_e2.h \
           geometry_E3/bbox_e3.h \
           geometry_E3/direction_e3.h \
           geometry_E3/dominant_e3.h \
           geometry_E3/interpolation_e3.h \
           geometry_E3/line_e3.h \
           geometry_E3/normalize_e3.h \
           geometry_E3/plane_e3.h \
           geometry_E3/point_e3.h \
           geometry_E3/polygon_e3.h \
           geometry_E3/polygon_normal_e3.h \
           geometry_E3/predicates_e3.h \
           geometry_E3/projection_e3.h \
           geometry_E3/ray_e3.h \
           geometry_E3/segment_e3.h \
           geometry_E3/transformation_e3.h \
           geometry_E3/triangle_e3.h \
           geometry_E3/vector_e3.h \
           geometry_lib/transformations.h \
           geometry_T3/direction_t3.h \
           geometry_T3/intersection_t3.h \
           geometry_T3/line_t3.h \
           geometry_T3/perspective_divide_t3.h \
           geometry_T3/plane_t3.h \
           geometry_T3/point_t3.h \
           geometry_T3/predicates_t3.h \
           geometry_T3/segment_t3.h \
           geometry_T3/transformation_t3.h \
           geometry_T3/unit_cube_intersections_t3.h \
           geometry_T3/vector_t3.h \
           misc/color_3.h \
           misc/color_4.h \
           misc/conversions.h \
           misc/determinant.h \
           misc/epsilon_double.h \
           misc/orientation.h \
           misc/random_e2.h \
           misc/random_e3.h \
           opengl/gldraw.h \
           opengl/gldraw_e2.h \
           opengl/gldraw_e3.h \
           opengl/glimagewriter.h \
           planar_graph/planar_graph.h \
           raster_image/array2.h \
           raster_image/image.h \
           raster_image/image_color_4.h \
           scene/maze.h \
           scene/scene.h \
           solid_heds/face.h \
           solid_heds/halfedge.h \
           solid_heds/heds.h \
           solid_heds/heds_reader.h \
           solid_heds/node.h \
           types/color_types.h \
           types/geometry_types.h \
           types/heds_types.h \
           types/lighting_constants.h \
           types/number_type.h \
           types/ptr_types.h
SOURCES += main.cpp \
           critters/critter.cpp \
           critters/intelligence.cpp \
           critters/player.cpp \
           critters/prey.cpp \
           delaunay/Qdelaunay_subwindow.cpp \
           delaunay/Qdelaunay_window.cpp \
           delcapo/Qdelcapo_subwindow.cpp \
           delcapo/Qdelcapo_window.cpp \
           geometry_lib/transformations.cpp \
           opengl/gldraw.cpp \
           opengl/gldraw_e2.cpp \
           opengl/gldraw_e3.cpp \
           planar_graph/planar_graph.cpp \
           raster_image/image.cpp \
           scene/maze.cpp \
           scene/scene.cpp \
           types/heds_types.cpp

unix:OBJECTS_DIR = ./objs
win32:OBJECTS_DIR = c:/objs

unix:MOC_DIR = ./objs
win32:MOC_DIR = c:/objs

