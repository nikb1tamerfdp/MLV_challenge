#ifndef _ELLIPSE_
#define _ELLIPSE_

#include "MLVEngine/util/lang.h"
#include "MLVEngine/ui/drawables/shapes.h"
#include "MLVEngine/ui/drawables/drawable.h"

#define ELLIPSE_DEFAULT_RESOLUTION 49

typedef struct _ellipse _Ellipse;
typedef _Ellipse* Ellipse;

struct _ellipse {
    
    Vector2 radius;
    ShapeDisplay display;
    Polygon polygon;

};

Drawable _Ellipse_as_drawable(Vector2 radius, ShapeDisplay display);

typedef Ellipse Circle;

Drawable _Circle_as_drawable(double radius, ShapeDisplay display);

#endif
