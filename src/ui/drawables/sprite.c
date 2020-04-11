#include "../../../includes/ui/drawables/sprite.h"
#include "../../../includes/util/contract.h"
#include "../../../includes/util/misc.h"

Drawable _Sprite_as_drawable(MLV_Image* texture, Rect rect);
Sprite _Sprite_new(MLV_Image* texture, Rect rect);
void _Sprite_draw(Drawable self, Transform transform);

Drawable _Sprite_as_drawable(MLV_Image* texture, Rect rect)
{

    Drawable self = NEW(_Drawable);
    self -> data = _Sprite_new(texture, rect);
    self -> _draw = _Sprite_draw;

    return self;

}

Sprite _Sprite_new(MLV_Image* texture, Rect rect)
{

    REQUIRE_NON_NULL(texture);

    Sprite self = NEW(_Sprite);
    self -> texture = texture;
    self -> partial = MLV_copy_partial_image(texture, RECT_X(rect), RECT_Y(rect), RECT_W(rect), RECT_H(rect));
    self -> rect = rect;

    return self;

}

void _Sprite_draw(Drawable self, Transform transform)
{

    Sprite sprite = DRAWABLE_DATA_AS(self, Sprite);

    Point top_left = { 0., 0. };
    Point top_right = { (sprite -> rect).size.x, 0. };
    Point bottom_left = { 0., (sprite -> rect).size.y };

    Vector2 top_ori = top_right;
    MLV_draw_line(100, 100, 100 + top_ori.x, 100 + top_ori.y, MLV_COLOR_GREEN1);
    double len_top_ori = VECTOR_NORM(top_ori);
    top_ori.x /= len_top_ori;
    top_ori.y /= len_top_ori;

    Vector2 left_ori = { bottom_left.x, bottom_left.y };
    MLV_draw_line(100, 100, 100 + left_ori.x, 100 + left_ori.y, MLV_COLOR_GREEN1);
    double len_left_ori = VECTOR_NORM(left_ori);

    top_left = TRANSFORM_APPLY(transform, top_left);
    top_right = TRANSFORM_APPLY(transform, top_right);
    bottom_left = TRANSFORM_APPLY(transform, bottom_left);

    Vector2 top_tra = { top_right.x - top_left.x, top_right.y - top_left.y};
    MLV_draw_line(100, 100, 100 + top_tra.x, 100 + top_tra.y, MLV_COLOR_RED);
    double len_top_tra = VECTOR_NORM(top_tra);
    top_tra.x /= len_top_tra;
    top_tra.y /= len_top_tra;
    
    Vector2 left_tra = { bottom_left.x - top_left.x, bottom_left.y - top_left.y};
    MLV_draw_line(100, 100, 100 + left_tra.x, 100 + left_tra.y, MLV_COLOR_RED);
    double len_left_tra = VECTOR_NORM(left_tra);

    double dot = VECTOR_DOT(top_ori, top_tra);
    double det = VECTOR_DET(top_ori, top_tra);
    double angle = RAD_TO_DEG(atan2(det, dot));
    if (angle < 0) angle += 360;
    angle *= -1;
    
    MLV_Image* transformed = MLV_copy_partial_image(
        sprite -> texture,
        RECT_X(sprite -> rect), RECT_Y(sprite -> rect),
        RECT_W(sprite -> rect), RECT_H(sprite -> rect)
    );

    
    
    Vector2 scale = { len_top_tra / len_top_ori, len_left_tra / len_left_ori };
    MLV_scale_xy_image(transformed, scale.x, scale.y);


    MLV_rotate_image(transformed, angle);

    Point anchor = {
        top_left.x - (MLV_get_image_width(transformed) / 2.),
        top_left.y - (MLV_get_image_height(transformed) / 2.)
    };
    
    MLV_draw_image(transformed, anchor.x, anchor.y);

}
