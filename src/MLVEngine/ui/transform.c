#include <math.h>
#include "MLVEngine/util/misc.h"
#include "MLVEngine/ui/transform.h"
#include "MLVEngine/util/contract.h"

void _Transform_init_methods(Transform self);
void _Transform_move_anchor(Transform self, double x, double y);
void _Transform_set_anchor(Transform self, double x, double y);
void _Transform_move(Transform self, double x, double y);
void _Transform_set_position(Transform self, double x, double y);
void _Transform_rotate(Transform self, double angle);
void _Transform_set_rotation(Transform self, double angle);
void _Transform_rotate_around(Transform self, double angle, double center_x, double center_y);
void _Transform_set_rotation_around(Transform self, double angle, double center_x, double center_y);
void _Transform_scale(Transform self, double x, double y);
void _Transform_set_scale(Transform self, double x, double y);
void _Transform_scale_from(Transform self, double x, double y, double from_x, double from_y);
void _Transform_set_scale_from(Transform self, double x, double y, double from_x, double from_y);
Matrix _Transform_get_matrix(Transform self);
Matrix _Transform_build_anchoring_matrix(Transform self);
Matrix _Transform_build_translation_matrix(Transform self);
Matrix _Transform_build_rotation_matrix(Transform self);
Matrix _Transform_build_scaling_matrix(Transform self);
void _Transform_compute_matrix(Transform self);
Point _Transform_apply(Transform self, double x, double y);
void _Transform_free(Transform self);

Transform _Transform_new();
Transform _Transform_translation(double x, double y);
Transform _Transform_scaling(double x, double y);
Transform _Transform_scaling_from(double x, double y, double from_x, double from_y);
Transform _Transform_rotation(double angle);
Transform _Transform_rotation_around(double angle, double center_x, double center_y);
Transform _Transform_combine(Transform a, Transform b);

const TransformFactory Transforms = {
    _Transform_new,
    _Transform_translation,
    _Transform_scaling,
    _Transform_scaling_from,
    _Transform_rotation,
    _Transform_rotation_around,
    _Transform_combine
};

#define ANCH(self) TRANSFORM_ANCH(self)
#define POS(self) TRANSFORM_POS(self)
#define SC(self) TRANSFORM_SC(self)
#define ROT(self) TRANSFORM_ROT(self)
#define MATRIX(self) FIELD(self, matrix)
#define NEEDS_UPDATE(self) FIELD(self, _matrix_needs_update)

void _Transform_init_methods(Transform self)
{

    FIELD(self, _move_anchor) = _Transform_move_anchor;
    FIELD(self, _set_anchor) = _Transform_set_anchor;
    FIELD(self, _move) = _Transform_move;
    FIELD(self, _set_position) = _Transform_set_position;
    FIELD(self, _rotate) = _Transform_rotate;
    FIELD(self, _set_rotation) = _Transform_set_rotation;
    FIELD(self, _rotate_around) = _Transform_rotate_around;
    FIELD(self, _set_rotation_around) = _Transform_set_rotation_around;
    FIELD(self, _scale) = _Transform_scale;
    FIELD(self, _set_scale) = _Transform_set_scale;
    FIELD(self, _scale_from) = _Transform_scale_from;
    FIELD(self, _set_scale_from) = _Transform_set_scale_from;
    FIELD(self, _apply) = _Transform_apply;
    FIELD(self, _get_matrix) = _Transform_get_matrix;
    FIELD(self, _free) = _Transform_free;

}

void _Transform_move_anchor(Transform self, double x, double y)
{
    POINT_ADD_XY(ANCH(self), x, y);
    NEEDS_UPDATE(self) = TRUE;
}

void _Transform_set_anchor(Transform self, double x, double y)
{
    POINT_SET_XY(ANCH(self), x, y);
    NEEDS_UPDATE(self) = TRUE;
}

void _Transform_move(Transform self, double x, double y)
{
    POINT_ADD_XY(POS(self), x, y);
    NEEDS_UPDATE(self) = TRUE;
}

void _Transform_set_position(Transform self, double x, double y)
{
    POINT_SET_XY(POS(self), x, y);
    NEEDS_UPDATE(self) = TRUE;
}

void _Transform_rotate(Transform self, double angle)
{
    ROT(self) += angle;
    NEEDS_UPDATE(self) = TRUE;
}

void _Transform_set_rotation(Transform self, double angle)
{
    ROT(self) = angle;
    NEEDS_UPDATE(self) = TRUE;
}

Vector2 _Transform_compute_t_for_angle_around(double angle, double center_x, double center_y)
{
    angle = DEG_TO_RAD(angle);
    double c = cos(angle);
    double s = sin(angle);

    Vector2 t = {
        (-center_x * c) + (center_y * s) + center_x,
        (-center_x * s) - (center_y * c) + center_y
    };

    return t;

}

void _Transform_rotate_around(Transform self, double angle, double center_x, double center_y)
{
    ROT(self) += angle;
    Vector2 t = _Transform_compute_t_for_angle_around(angle, center_x, center_y);
    POINT_ADD_XY(POS(self), t.x, t.y);
    NEEDS_UPDATE(self) = TRUE;

}

void _Transform_set_rotation_around(Transform self, double angle, double center_x, double center_y)
{

    ROT(self) = angle;
    Vector2 t = _Transform_compute_t_for_angle_around(angle, center_x, center_y);
    POINT_ADD_XY(POS(self), t.x, t.y);
    NEEDS_UPDATE(self) = TRUE;

}

void _Transform_scale(Transform self, double x, double y)
{
    VECTOR_MUL_XY(SC(self), x, y);
    NEEDS_UPDATE(self) = TRUE;
}

void _Transform_set_scale(Transform self, double x, double y)
{
    VECTOR_SET_XY(SC(self), x, y);
    NEEDS_UPDATE(self) = TRUE;
}

void _Transform_scale_from(Transform self, double x, double y, double from_x, double from_y)
{
    VECTOR_ADD_XY(SC(self), x, y);
    POINT_ADD_XY(POS(self), from_x * (1. - x), from_y * (1. - y));
    NEEDS_UPDATE(self) = TRUE;
}

void _Transform_set_scale_from(Transform self, double x, double y, double from_x, double from_y)
{
    VECTOR_SET_XY(SC(self), x, y);
    POINT_ADD_XY(POS(self), from_x * (1. - x), from_y * (1. - y));
    NEEDS_UPDATE(self) = TRUE;
}

Point _Transform_apply(Transform self, double x, double y)
{

    REQUIRE_NON_NULL(self);

    Matrix m = _Transform_get_matrix(self);
    double a1 = TERM(m, 0, 0), a2 = TERM(m, 0, 1), b1 = TERM(m, 0, 2);
    double a3 = TERM(m, 1, 0), a4 = TERM(m, 1, 1), b2 = TERM(m, 1, 2);

    Point transformed = {
        (a1 * x) + (a2 * y) + b1,
        (a3 * x) + (a4 * y) + b2
    };

    return transformed;

}

Matrix _Transform_get_matrix(Transform self)
{
    if (NEEDS_UPDATE(self))
    {
        _Transform_compute_matrix(self);
        NEEDS_UPDATE(self) = FALSE;
    }

    return MATRIX(self);
}

Matrix _Transform_build_anchoring_matrix(Transform self)
{
    
    Matrix m = Matrix_new_identity(3);
    Matrix_set(m, 1, 3, ANCH(self).x);
    Matrix_set(m, 2, 3, ANCH(self).y);

    return m;

}

Matrix _Transform_build_translation_matrix(Transform self)
{
    
    Matrix m = Matrix_new_identity(3);
    Matrix_set(m, 1, 3, POS(self).x);
    Matrix_set(m, 2, 3, POS(self).y);

    return m;

}

Matrix _Transform_build_rotation_matrix(Transform self)
{

    Matrix m = Matrix_new_identity(3);

    double angle = DEG_TO_RAD(ROT(self));
    double c = cos(angle);
    double s = sin(angle);

    Matrix_set(m, 1, 1, c);
    Matrix_set(m, 1, 2, -s);
    Matrix_set(m, 2, 1, s);
    Matrix_set(m, 2, 2, c);

    return m;

}

Matrix _Transform_build_scaling_matrix(Transform self)
{
    
    Matrix m = Matrix_new_identity(3);
    Matrix_set(m, 1, 1, SC(self).x);
    Matrix_set(m, 2, 2, SC(self).y);

    return m;

}

void _Transform_compute_matrix(Transform self)
{

    Matrix combined;
    Matrix t = _Transform_build_translation_matrix(self);
    Matrix s = _Transform_build_scaling_matrix(self);
    Matrix r = _Transform_build_rotation_matrix(self);
    Matrix a = _Transform_build_anchoring_matrix(self);

    combined = t;

    Matrix_multiply(combined, r);
    Matrix_free(r);
    Matrix_multiply(combined, s);
    Matrix_free(s);

    Matrix_multiply(combined, a);
    Matrix_free(a);

    if (NON_NULL(MATRIX(self)))
        Matrix_free(MATRIX(self));
    
    MATRIX(self) = combined;

}

void _Transform_free(Transform self)
{

    ENSURE_FREEABLE(self);

    if (NON_NULL(MATRIX(self))) Matrix_free(MATRIX(self));
    FREE(self);

}

Transform _Transform_new()
{
    
    Transform self = NEW(_Transform);
    ANCH(self) = ORIGIN;
    POS(self) = ORIGIN;
    SC(self) = VEC_UNIT;
    ROT(self) = 0.;
    MATRIX(self) = NULL;
    NEEDS_UPDATE(self) = TRUE;
    _Transform_init_methods(self);

    return self;

}

Transform _Transform_translation(double x, double y)
{
    
    Transform translation = _Transform_new();
    Matrix_set(translation -> matrix, 1, 3, x);
    Matrix_set(translation -> matrix, 2, 3, y);

    return translation;

}

Transform _Transform_scaling(double x, double y)
{
    return _Transform_scaling_from(x, y, ORIGIN.x, ORIGIN.y);
}

Transform _Transform_scaling_from(double x, double y, double from_x, double from_y)
{
    
    Transform scaling = _Transform_new();
    Matrix_set(scaling -> matrix, 1, 1, x);
    Matrix_set(scaling -> matrix, 1, 3, from_x * (1. - x));
    Matrix_set(scaling -> matrix, 2, 2, y);
    Matrix_set(scaling -> matrix, 2, 3, from_y * (1. - y));

    return scaling;

}

Transform _Transform_rotation(double angle)
{
    return _Transform_rotation_around(angle, ORIGIN.x, ORIGIN.y);
}

Transform _Transform_rotation_around(double angle, double center_x, double center_y)
{

    angle = DEG_TO_RAD(angle);
    double c = cos(angle);
    double s = sin(angle);

    Transform rotation = _Transform_new();
    Matrix_set(rotation -> matrix, 1, 1, c);
    Matrix_set(rotation -> matrix, 1, 2, -s);
    Matrix_set(rotation -> matrix, 1, 3, (-center_x * c) + (center_y * s) + center_x);
    Matrix_set(rotation -> matrix, 2, 1, s);
    Matrix_set(rotation -> matrix, 2, 2, c);
    Matrix_set(rotation -> matrix, 2, 3, (-center_x * s) - (center_y * c) + center_y);

    return rotation;

}

Transform _Transform_combine(Transform a, Transform b)
{

    REQUIRE_NON_NULL(a);
    REQUIRE_NON_NULL(b);

    Transform combined = _Transform_new();
    ANCH(combined) = ANCH(a);
    POINT_ADD(ANCH(combined), ANCH(b));
    POS(combined) = POS(a);
    POINT_ADD(POS(combined), POS(b));
    SC(combined) = SC(a);
    VECTOR_MUL(SC(combined), SC(b));
    ROT(combined) = ROT(a) + ROT(b);

    return combined;

}
