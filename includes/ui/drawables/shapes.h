#ifndef _SHAPES_
#define _SHAPES_

#include <MLV/MLV_all.h>
#include "../../util/lang.h"
#include "../point.h"

typedef struct _polygon _Polygon;
typedef _Polygon* Polygon;

struct _polygon {
    int* points[2];
    int* buffer[2];
    int n_points;
};

typedef enum {
    BORDER_INSIDE,
    BORDER_CENTER,
    BORDER_OUTSIDE
} BorderDirection;

typedef struct _border Border;

struct _border {

    double thickness;
    BorderDirection direction;
    MLV_Color color;

};

typedef struct _shape_display ShapeDisplay;

struct _shape_display {

    Boolean filled;
    MLV_Color fill_color;
    Border border;

};

#endif
