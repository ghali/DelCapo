// Sherif Ghali

#ifndef GEOM_TYPES_H
#define GEOM_TYPES_H

#include <QtGui>

#include "number_type.h"

// All the concrete geometry classes, either Euclidean or projective,
// and in 2D or 3D, should be instantiated in this file.
// 
// Doing so slows down compilation slightly, but saves the
// considerable hassle of determining and updating which file needs
// which geometric objects. Incremental compilation should in any case
// make the extra compilation time unnoticeable.

// Geometry in E2
#include "../geometry_E2/point_e2.h"
#include "../geometry_E2/vector_e2.h"
#include "../geometry_E2/direction_e2.h"
#include "../geometry_E2/segment_e2.h"
#include "../geometry_E2/polygon_e2.h"
#include "../geometry_E2/bbox_e2.h"
#include "../geometry_E2/transformation_e2.h"

#include "../geometry_E2/predicates_e2.h"
#include "../geometry_E2/intersection_e2.h"

typedef Point_E2<MyDouble>                   Point_E2d;
typedef Vector_E2<MyDouble>                 Vector_E2d;
typedef Direction_E2<MyDouble>           Direction_E2d;
typedef Segment_E2<MyDouble>               Segment_E2d;
typedef Polygon_E2<MyDouble>               Polygon_E2d;
typedef Bbox_E2<MyDouble>                     Bbox_E2d;
typedef Transformation_E2<MyDouble> Transformation_E2d;

typedef Point_E2<int>           Point_E2i;
typedef Vector_E2<int>         Vector_E2i;

// Geometry in E3
#include "geometry_E3/point_e3.h"
#include "geometry_E3/segment_e3.h"
#include "geometry_E3/direction_e3.h"
#include "geometry_E3/polygon_e3.h"
#include "geometry_E3/polygon_normal_e3.h"
#include "geometry_E3/interpolation_e3.h"
#include "geometry_E3/projection_e3.h"
#include "geometry_E3/transformation_e3.h"

typedef Point_E3<MyDouble>                   Point_E3d;
typedef Segment_E3<MyDouble>               Segment_E3d;
typedef Direction_E3<MyDouble>           Direction_E3d;
typedef Polygon_E3<MyDouble>               Polygon_E3d;
typedef Vector_E3<MyDouble>                 Vector_E3d;
typedef Transformation_E3<MyDouble> Transformation_E3d;

// Geometry in T3
#include "geometry_T3/point_t3.h"

typedef Point_T3<MyDouble> Point_T3d;

const Direction_E3d X_axis(1.0,0.0,0.0);
const Direction_E3d Y_axis(0.0,1.0,0.0);
const Direction_E3d Z_axis(0.0,0.0,1.0);

// Delaunay
#include <vector>
typedef std::vector<Point_E2d>::iterator My_point_E2_input_iterator;

#endif // GEOM_TYPES_H
