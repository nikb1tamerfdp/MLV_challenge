#ifndef _DRAWABLE_
#define _DRAWABLE_

#include "../../util/lang.h"
#include "../../util/str.h"
#include "../transform.h"
#include "../scene.h"

typedef struct _drawable _Drawable;
typedef _Drawable* Drawable;

struct _drawable {
    
    String id;
    SceneNode node;
    Any data;

    FUNC(void, _draw, Drawable, Transform);
    FUNC(void, _free, Drawable);

};

#define DRAWABLE_ID(self) FIELD(self, id)
#define DRAWABLE_NODE(self) FIELD(self, node)
#define DRAWABLE_DATA(self) FIELD(self, data)
#define DRAWABLE_DATA_AS(self, type) CAST(type, DRAWABLE_DATA(self))
#define DRAWABLE_DRAW(self, transform) METHOD(self, _draw, transform)
#define DRAWABLE_FREE(self) METHOD(self, _free)

#endif
