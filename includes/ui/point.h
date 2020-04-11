#ifndef _POINT_
#define _POINT_

#include <math.h>

typedef struct _point Point;

struct _point {
    double x;
    double y;
};

#define POINT_SET_XY(self, _x, _y) { self.x = (_x); self.y = (_y); }
#define POINT_ADD_XY(self, _x, _y) { self.x += (_x); self.y += (_y); }
#define POINT_ADD(self, other) POINT_ADD_XY(self, (other).x, (other).y)
#define POINT_SUB_XY(self, _x, _y) POINT_ADD_XY(self, -(_x), -(_y))
#define POINT_SUB(self, other) POINT_SUB(self, (other).x, (other).y)
#define POINT_MUL_XY(self, _x, _y) { self.x *= (_x); self.y *= (_y); }
#define POINT_MUL(self, other) POINT_MUL_XY(self, (other).x, (other).y)
#define POINT_DIV_XY(self, _x, _y) { self.x /= (_x); self.y /= (_y); }
#define POINT_DIV(self, other) POINT_DIV_XY(self, (other).x, (other).y)
#define POINT_DIV_VALUE(self, value) POINT_DIV_XY(self, value, value)

extern const Point ORIGIN;

typedef Point Vector2;

#define VECTOR_SET_XY(self, x, y) POINT_SET_XY(self, x, y)
#define VECTOR_ADD_XY(self, x, y) POINT_ADD_XY(self, x, y)
#define VECTOR_ADD(self, other) POINT_ADD(self, other)
#define VECTOR_SUB_XY(self, x, y) POINT_SUB_XY(self, x, y)
#define VECTOR_SUB(self, other) POINT_SUB(self, other)
#define VECTOR_MUL_XY(self, x, y) POINT_MUL_XY(self, x, y)
#define VECTOR_MUL(self, other) POINT_MUL(self, other)
#define VECTOR_DIV_XY(self, x, y) POINT_DIV_XY(self, x, y)
#define VECTOR_DIV_VALUE(self, value) POINT_DIV_VALUE(self, value)
#define VECTOR_DIV(self, other) POINT_DIV(self, other)
#define VECTOR_NORM(self) sqrt((self.x * self.x) + (self.y * self.y))
#define VECTOR_DOT(a, b) ((a.x * b.x) + (a.y * b.y))
#define VECTOR_DET(a, b) ((a.x * b.y) - (a.y * b.x))
#define VECTOR_NORMALIZE(self) ({ \
        double _len = VECTOR_NORM(self); \
        if (!FLOAT_EQUALS(_len, 0.)) \
            VECTOR_DIV_VALUE(self, _len); \
    })

extern const Vector2 VEC_UNIT;
extern const Vector2 VEC_NIL;

typedef struct _rect Rect;

struct _rect {
    Point top_left;
    Vector2 size;
};

#define RECT_X(self) (self.top_left.x)
#define RECT_Y(self) (self.top_left.y)
#define RECT_W(self) (self.size.x)
#define RECT_H(self) (self.size.y)

#endif
