#ifndef _SPRITE_
#define _SPRITE_

#include <MLV/MLV_all.h>
#include "MLVEngine/ui/drawables/shapes.h"
#include "MLVEngine/ui/drawables/drawable.h"

typedef struct _sprite _Sprite;
typedef _Sprite* Sprite;

struct _sprite {
    MLV_Image* texture;
    MLV_Image* partial;
    Rect rect;
};

Drawable _Sprite_as_drawable(MLV_Image* texture, Rect rect);

#endif
