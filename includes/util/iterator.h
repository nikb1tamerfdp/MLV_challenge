#ifndef _ITERATOR_
#define _ITERATOR_

#include "lang.h"

typedef struct _iterator Iterator;

struct _iterator {

    Any source;
    Any index;
    int position;

    FUNC(Boolean, _has_next, Iterator*);
    FUNC(Any, _next, Iterator*);

};

#define ITER_HAS_NEXT(self) ((self)._has_next(REF(self)))
#define ITER_SOURCE(self) ((self).source)
#define ITER_SOURCE_AS(self, type) CAST(type, ITER_SOURCE(self))
#define ITER_INDEX(self) ((self).index)
#define ITER_INDEX_AS(self, type) CAST(type, ITER_INDEX(self))
#define ITER_POSITION(self) ((self).position)
#define ITER_NEXT(self) ((self)._next(REF(self)))
#define ITER_NEXT_AS(self, type) CAST(type, ITER_NEXT(self))
#define ITER_CONSUME_UNTIL_POSITION(self, position) { \
        int target = position; \
        while (ITER_HAS_NEXT(it) && ITER_POSITION(it) < target) \
            ITER_NEXT(it); \
    }

#endif
