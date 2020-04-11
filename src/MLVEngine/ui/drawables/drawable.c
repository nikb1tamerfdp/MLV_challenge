#include "MLVEngine/ui/drawables/drawable.h"
#include <MLV/MLV_all.h>

void _Circle_draw(Drawable self, Transform transform);

Drawable Circle_create(int r)
{
    
    int* _r = NEW(int);
    *_r = r;

    Drawable self = NEW(_Drawable);
    FIELD(self, _draw) = _Circle_draw;
    self -> data = _r;

    return self;

}

void _Circle_draw(Drawable self, Transform transform)
{

    int r = AT(CAST(int*, self -> data));
    Point center = { 0., 0.};
    Point radius = { r, r };

    center = TRANSFORM_APPLY(transform, center);
    radius = TRANSFORM_APPLY(transform, radius);
    radius.x -= center.x;
    radius.y -= center.y;
    
    MLV_draw_ellipse(center.x, center.y, radius.x, radius.y, MLV_COLOR_RED);

}
