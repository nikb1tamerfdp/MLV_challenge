#ifndef _TRANSFORM_
#define _TRANSFORM_

#include "MLVEngine/util/matrix.h"
#include "MLVEngine/ui/point.h"

typedef struct _transform _Transform;
typedef _Transform* Transform;

struct _transform {
    
    Point anchor;
    Vector2 position;
    Vector2 scale;
    double rotation;
    Matrix matrix;
    Boolean _matrix_needs_update;

    FUNC(void, _move_anchor, Transform, double, double);
    FUNC(void, _set_anchor, Transform, double, double);
    FUNC(void, _move, Transform, double, double);
    FUNC(void, _set_position, Transform, double, double);
    FUNC(void, _rotate, Transform, double);
    FUNC(void, _set_rotation, Transform, double);
    FUNC(void, _rotate_around, Transform, double, double, double);
    FUNC(void, _set_rotation_around, Transform, double, double, double);
    FUNC(void, _scale, Transform, double, double);
    FUNC(void, _set_scale, Transform, double, double);
    FUNC(void, _scale_from, Transform, double, double, double, double);
    FUNC(void, _set_scale_from, Transform, double, double, double, double);
    FUNC(Point, _apply, Transform, double, double);
    FUNC(Matrix, _get_matrix, Transform);
    FUNC(void, _free, Transform);

};

#define TRANSFORM_ANCH(self) FIELD(self, anchor)
#define TRANSFORM_POS(self) FIELD(self, position)
#define TRANSFORM_SC(self) FIELD(self, scale)
#define TRANSFORM_ROT(self) FIELD(self, rotation)
#define TRANSFORM_MOVE_ANCHOR_XY(self, x, y) METHOD(self, _move_anchor, x, y)
#define TRANSFORM_MOVE_ANCHOR(self, delta) TRANSFORM_MOVE_ANCHOR_XY(self, delta.x, delta.y)
#define TRANSFORM_SET_ANCHOR_XY(self, x, y) METHOD(self, _set_anchor, x, y)
#define TRANSFORM_SET_ANCHOR(self, anchor) TRANSFORM_SET_ANCHOR_XY(self, anchor.x, anchor.y)
#define TRANSFORM_MOVE_XY(self, x, y) METHOD(self, _move, x, y)
#define TRANSFORM_MOVE(self, delta) TRANSFORM_MOVE_XY(self, delta.x, delta.y)
#define TRANSFORM_SET_POS_XY(self, x, y) METHOD(self, _set_position, x, y)
#define TRANSFORM_SET_POS(self, position) TRANSFORM_SET_POS_XY(self, position.x, position.y)
#define TRANSFORM_ROTATE(self, angle) METHOD(self, _rotate, angle)
#define TRANSFORM_SET_ROT(self, angle) METHOD(self, _set_rotation, angle)
#define TRANSFORM_ROTATE_AROUND_XY(self, angle, center_x, center_y) METHOD(self, _rotate_around, angle, center_x, center_y)
#define TRANSFORM_ROTATE_AROUND(self, angle, center) TRANSFORM_ROTATE_AROUND_XY(self, angle, center.x, center.y)
#define TRANSFORM_SET_ROT_AROUND_XY(self, angle, center_x, center_y) METHOD(self, _set_rotation_around, angle, center_x, center_y)
#define TRANSFORM_SET_ROT_AROUND(self, angle, center) TRANSFORM_SET_ROT_AROUND_XY(self, angle, center.x, center.y)
#define TRANSFORM_SCALE_XY(self, x, y) METHOD(self, _scale, x, y)
#define TRANSFORM_SCALE(self, factor) TRANSFORM_SCALE_XY(self, factor.x, factor.y)
#define TRANSFORM_SCALE_XY_FROM_XY(self, x, y, from_x, from_y) METHOD(self, _scale_from, x, y, from_x, from_y)
#define TRANSFORM_SCALE_XY_FROM(self, x, y, from) TRANSFORM_SCALE_XY_FROM_XY(self, x, y, from.x, from.y)
#define TRANSFORM_SCALE_FROM_XY(self, factor, from_x, from_y) TRANSFORM_SCALE_XY_FROM_XY(self, factor.x, factor.y, from_x, from_y)
#define TRANSFORM_SCALE_FROM(self, factor, from) TRANSFORM_SCALE_XY_FROM_XY(self, factor.x, factor.y, from.x, from.y)
#define TRANSFORM_SET_SCALE_XY(self, x, y) METHOD(self, _set_scale, x, y)
#define TRANSFORM_SET_SCALE(self, factor) TRANSFORM_SET_SCALE_XY(self, factor.x, factor.y)
#define TRANSFORM_SET_SCALE_XY_FROM_XY(self, x, y, from_x, from_y) METHOD(self, _set_scale_from, x, y, from_x, from_y)
#define TRANSFORM_SET_SCALE_XY_FROM(self, x, y, from) TRANSFORM_SET_SCALE_XY_FROM_XY(x, y, from.x, from.y)
#define TRANSFORM_SET_SCALE_FROM_XY(self, factor, from_x, from_y) TRANSFORM_SET_SCALE_XY_FROM_XY(factor.x, factor.y, from_x, from_y)
#define TRANSFORM_SET_SCALE_FROM(self, factor, from) TRANSFORM_SET_SCALE_XY_FROM_XY(factor.x, factor.y, from.x, from.y)
#define TRANSFORM_APPLY_XY(self, x, y) METHOD(self, _apply, x, y)
#define TRANSFORM_APPLY(self, point) TRANSFORM_APPLY_XY(self, point.x, point.y)
#define TRANSFORM_MATRIX(self) METHOD(self, _get_matrix)
#define TRANSFORM_FREE(self) METHOD(self, _free)

typedef struct _transform_factory TransformFactory;

struct _transform_factory {
    
    FUNC(Transform, _identity, void);
    FUNC(Transform, _translation, double, double);
    FUNC(Transform, _scaling, double, double);
    FUNC(Transform, _scaling_from, double, double, double, double);
    FUNC(Transform, _rotation, double);
    FUNC(Transform, _rotation_around, double, double, double);
    FUNC(Transform, _combine, Transform, Transform);

};

extern const TransformFactory Transforms;

#define TRANSFORM_IDENTITY() Transforms._identity()
#define TRANSLATION_XY(x, y) Transforms._translation(x, y)
#define TRANSLATION(along) TRANSLATION_XY(along.x, along.y)
#define SCALING_XY(x, y) Transforms._scaling(x, y)
#define SCALING(factor) SCALING_XY(factor.x, factor.y)
#define SCALING_XY_FROM_XY(x, y, from_x, from_y) Transforms._scaling_from(x, y, from_x, from_y)
#define SCALING_FROM(factor, from) SCALING_XY_FROM_XY(factor.x, factor.y, from.x, from.y)
#define SCALING_XY_FROM(x, y, from) SCALING_XY_FROM_XY(x, y, from.x, from.y)
#define SCALING_FROM_XY(factor, from_x, from_y) SCALING_XY_FROM_XY(factor.x, factor.y, from_x, from_y)
#define ROTATION_AROUND_XY(angle, center_x, center_y) Transforms._rotation_around(angle, center_x, center_y)
#define ROTATION(angle) Transforms._rotation(angle)
#define ROTATION_AROUND(angle, center) ROTATION_AROUND_XY(angle, center.x, center.y)
#define TRANSFORM_COMBINE(a, b) Transforms._combine(a, b)

#endif
