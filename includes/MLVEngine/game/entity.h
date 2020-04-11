#ifndef _ENTITY_
#define _ENTITY_

#include "MLVEngine/ui/drawables/drawable.h"

typedef struct _entity _Entity;
typedef _Entity* Entity;

struct _entity {

    Drawable drawable;
    Any data;

    FUNC(void, _update, Entity, long);
    FUNC(void, _free, Entity);

};

#define ENTITY_DRAWABLE(self) FIELD(self, drawable)
#define ENTITY_IS_DRAWABLE(self) NON_NULL(ENTITY_DRAWABLE(self))
#define ENTITY_DATA(self) FIELD(self, data)
#define ENTITY_DATA_AS(self, type) CAST(type, ENTITY_DATA(self))
#define ENTITY_UPDATE(self, deltaTime) METHOD(self, _update, deltaTime)
#define ENTITY_FREE(self) METHOD(self, _free)

#endif
