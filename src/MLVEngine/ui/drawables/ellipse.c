#include "MLVEngine/ui/drawables/ellipse.h"
#include "MLVEngine/util/misc.h"
#include "MLVEngine/ui/drawables/drawable.h"

Ellipse _Ellipse_new(Vector2 radius, ShapeDisplay display);
Polygon _Ellipse_generate_polygon(Vector2 radius, int resolution);
void _Ellipse_draw(Drawable self, Transform transform);

Drawable _Ellipse_as_drawable(Vector2 radius, ShapeDisplay display)
{

    Drawable self = NEW(_Drawable);
    self -> data = _Ellipse_new(radius, display);
    self -> _draw = _Ellipse_draw;

    return self;

}

Ellipse _Ellipse_new(Vector2 radius, ShapeDisplay display)
{

    Ellipse self = NEW(_Ellipse);
    self -> radius = radius;
    self -> display = display;
    self -> polygon = _Ellipse_generate_polygon(radius, ELLIPSE_DEFAULT_RESOLUTION);

    return self;

}

Circle _Circle_new(double radius, ShapeDisplay display)
{

    Ellipse self = NEW(_Ellipse);
    Vector2 r = { radius, radius };
    self -> radius = r;
    self -> display = display;
    self -> polygon = _Ellipse_generate_polygon(r, ELLIPSE_DEFAULT_RESOLUTION);

    return self;

}

Drawable _Circle_as_drawable(double radius, ShapeDisplay display)
{

    Drawable self = NEW(_Drawable);
    self -> data = _Circle_new(radius, display);
    self -> _draw = _Ellipse_draw;

    return self;

}

Polygon _Ellipse_generate_polygon(Vector2 radius, int resolution)
{

    Polygon poly = NEW(_Polygon);
    poly -> n_points = resolution;
    (poly -> points)[0] = NEW_MULTIPLE(int, resolution);
    (poly -> points)[1] = NEW_MULTIPLE(int, resolution);
    (poly -> buffer)[0] = NEW_MULTIPLE(int, resolution);
    (poly -> buffer)[1] = NEW_MULTIPLE(int, resolution);

    /* int angle = 0; */
    /* double rot_angle = 0. */ /* angle * PI / 180; */;
    double c_angle = 1. /* cos(rot_angle) */;
    double s_angle = 0. /* sin(rot_angle) */;
    Point center = { 0., 0. };
    Point p;
    double theta;
    double kf = (360 * PI / 180) / resolution;
    int x, y;
    
    RANGE(i, 0, resolution, 1)
    {

        theta = i * kf;
        p.x = center.x + (radius.x * cos(theta));
        p.y = center.y + (radius.y * sin(theta));

        x = (center.x + ((p.x - center.x) * c_angle) - ((p.y - center.y) * s_angle));
        y = (center.y + ((p.x - center.x) * s_angle) + ((p.y - center.y) * c_angle));
        
        (poly -> buffer)[0][i] = (poly -> points)[0][i] = x;
        (poly -> buffer)[1][i] = (poly -> points)[1][i] = y;

    }

    return poly;

}

void _Ellipse_draw(Drawable self, Transform transform)
{

    Ellipse ellipse = DRAWABLE_DATA_AS(self, Ellipse);
    Polygon poly = ellipse -> polygon;

    RANGE(i, 0, poly -> n_points, 1)
    {
        Point original = { (poly -> points)[0][i], (poly -> points)[1][i] };
        Point transformed = TRANSFORM_APPLY(transform, original);
        (poly -> buffer)[0][i] = transformed.x;
        (poly -> buffer)[1][i] = transformed.y;
    }
    
    ShapeDisplay display = ellipse -> display;

    /*if (NON_NULL(display -> border))
    {

        Ellipse inside = ellipse;
        inside.rx += inside.border_size;
        inside.ry += inside.border_size;
        inside.fill_color = inside.border_color;
        inside.border_size = -1;
        draw_ellipse(inside, xcenter, ycenter, angle);

    }*/

    MLV_draw_filled_polygon(
        (poly -> buffer)[0],
        (poly -> buffer)[1],
        poly -> n_points,
        display.fill_color
    );

}
