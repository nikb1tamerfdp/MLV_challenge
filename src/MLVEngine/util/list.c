#include "MLVEngine/util/list.h"
#include "MLVEngine/util/misc.h"
#include "MLVEngine/util/str.h"
#include "MLVEngine/util/error.h"

/* --- declaration of methods for ListItem */
ListItem _ListItem_new(Any content);
void _ListItem_init_methods(ListItem self);
void _ListItem_free(ListItem self, Boolean free_content, AnyConsumer custom_free);

/* --- some shortcuts for ListItem accessors */
#define CONTENT(self) LIST_ITEM_CONTENT(self)
#define NEXT(self) LIST_ITEM_NEXT(self)

/* --- declaration of methods for List */
List _List_new();
void _List_init_methods(List self);
ListItem _List_append(List self, Any content);
ListItem _List_append_at(List self, int position, Any content);
ListItem _List_push(List self, Any content);
void _List_for_each(List self, AnyConsumer action, AnyPredicate filter);
void _List_for_each_indexed(List self, BI_CONSUMER(int, Any, action), BI_PREDICATE(int, Any, filter));
Any _List_get(List self, int index);
Iterator _List_iterator(List self);
Any _List_remove(List self, int index);
Boolean _List_is_empty(List self);
void _List_clear(List self, Boolean free_content, AnyConsumer custom_free);
void _List_free(List self, Boolean free_content, AnyConsumer custom_free);
void _List_free_recursive(ListItem item, Boolean free_content, AnyConsumer custom_free);

/* --- some shortcuts for ListItem accessors */
#define HEAD(self) LIST_HEAD(self)
#define LAST(self) LIST_LAST(self)
#define SIZE(self) LIST_SIZE(self)
#define EMPTY(self) LIST_IS_EMPTY(self)

/* --- definition of global ListFactory */
const ListFactory Lists = {
    _List_new
};

/* --- declaration of private helpers */
Any _ListIterator_next(Iterator* it);
Boolean _ListIterator_has_next(Iterator* it);

/** ====================================== **
 *  Implementation of methods for ListItem  *
 ** ====================================== **/

ListItem _ListItem_new(Any content)
{

    ListItem self = NEW(_ListItem);
    CONTENT(self) = content;
    NEXT(self) = NULL;
    _ListItem_init_methods(self);

    return self;

}

void _ListItem_init_methods(ListItem self)
{

    FIELD(self, _free) = _ListItem_free;

}

void _ListItem_free(ListItem self, Boolean free_content, AnyConsumer custom_free)
{

    ENSURE_FREEABLE(self);

    if (free_content && NON_NULL(CONTENT(self)))
        IF_NULL_DEFAULT(custom_free, FREE)(CONTENT(self));
    
    FREE(self);

}

/** ================================== **
 *  Implementation of methods for List  *
 ** ================================== **/

List _List_new()
{
    
    List self = NEW(_List);
    HEAD(self) = NULL;
    LAST(self) = NULL;
    SIZE(self) = 0;
    _List_init_methods(self);

    return self;

}

void _List_init_methods(List self)
{

    FIELD(self, _append) = _List_append;
    FIELD(self, _append_at) = _List_append_at;
    FIELD(self, _push) = _List_push;
    FIELD(self, _for_each) = _List_for_each;
    FIELD(self, _for_each_indexed) = _List_for_each_indexed;
    FIELD(self, _get) = _List_get;
    FIELD(self, _iterator) = _List_iterator;
    FIELD(self, _is_empty) = _List_is_empty;
    FIELD(self, _remove) = _List_remove;
    FIELD(self, _clear) = _List_clear;
    FIELD(self, _free) = _List_free;

}

ListItem _List_append(List self, Any content)
{

    REQUIRE_NON_NULL(self);

    ListItem item = _ListItem_new(content);

    if (EMPTY(self))
        HEAD(self) = item;
    else
        NEXT(LAST(self)) = item;

    LAST(self) = item;
    SIZE(self)++;

    return item;

}

ListItem _List_append_at(List self, int position, Any content)
{

    REQUIRE_NON_NULL(self);

    position = MAX(0, MIN(SIZE(self), position));

    if (position == SIZE(self))
        return LIST_APPEND(self, content);

    if (position == 0)
        return LIST_PUSH(self, content);
    
    ListItem item = _ListItem_new(content);

    Iterator it = LIST_ITERATOR(self);
    ITER_CONSUME_UNTIL_POSITION(it, (position - 1));
    ListItem index = ITER_INDEX_AS(it, ListItem);
    NEXT(item) = NEXT(index);
    NEXT(index) = item;
    SIZE(self)++;

    return item;

}

ListItem _List_push(List self, Any content)
{

    REQUIRE_NON_NULL(self);

    if (EMPTY(self))
        return LIST_APPEND(self, content);

    ListItem item = _ListItem_new(content);
    NEXT(item) = HEAD(self);
    HEAD(self) = item;
    SIZE(self)++;

    return item;

}

void _List_for_each(List self, AnyConsumer action, AnyPredicate filter)
{

    REQUIRE_NON_NULL(self);
    REQUIRE_NON_NULL(action);

    Iterator it = LIST_ITERATOR(self);
    while (ITER_HAS_NEXT(it))
    {
        Any current = ITER_NEXT(it);
        if (IS_NULL(filter) || filter(current))
            action(current);
    }

}

void _List_for_each_indexed(List self, BI_CONSUMER(int, Any, action), BI_PREDICATE(int, Any, filter))
{

    REQUIRE_NON_NULL(self);
    REQUIRE_NON_NULL(action);

    Iterator it = LIST_ITERATOR(self);
    while (ITER_HAS_NEXT(it))
    {
        int i = ITER_POSITION(it);
        Any current = ITER_NEXT(it);
        if (IS_NULL(filter) || filter(i, current))
            action(i, current);
    }

}

Any _List_get(List self, int index)
{
    
    REQUIRE_NON_NULL(self);

    if (EMPTY(self) || index < 0 || SIZE(self) <= index)
        THROW_AND_KILL("Index out of bounds !", "Cannot get item %d of list with only %d items.", index, SIZE(self));

    if (index == 0)
        return CONTENT(HEAD(self));

    if ((index + 1) == SIZE(self))
        return CONTENT(LAST(self));

    Iterator it = LIST_ITERATOR(self);
    ITER_CONSUME_UNTIL_POSITION(it, index);
    ListItem item = ITER_INDEX_AS(it, ListItem);

    return CONTENT(item);

}

Iterator _List_iterator(List self)
{

    REQUIRE_NON_NULL(self);

    Iterator it;
    ITER_SOURCE(it) = self;
    ITER_INDEX(it) = HEAD(self);
    ITER_POSITION(it) = 0;
    it._next = _ListIterator_next;
    it._has_next = _ListIterator_has_next;

    return it;

}

Any _List_remove(List self, int index)
{

    REQUIRE_NON_NULL(self);

    if (EMPTY(self) || index < 0 || SIZE(self) <= index)
        THROW_AND_KILL("Index out of bounds !", "Cannot remove item %d of list with only %d items.", index, SIZE(self));

    ListItem removed;

    if (index == 0)
    {
        removed = HEAD(self);
        HEAD(self) = NEXT(removed);
        if (IS_NULL(HEAD(self))) LAST(self) = NULL;
    }
    else
    {
        ListItem current = HEAD(self);
        for (; index > 1; current = NEXT(current), index--);
        removed = NEXT(current);
        NEXT(current) = NEXT(removed);
        if (IS_NULL(NEXT(current))) LAST(self) = current;
    }
    
    SIZE(self)--;

    Any content = CONTENT(removed);
    LIST_ITEM_FREE(removed, FALSE, NULL);

    return content;

}

Boolean _List_is_empty(List self)
{
    
    REQUIRE_NON_NULL(self);

    return EMPTY(self);

}

void _List_clear(List self, Boolean free_content, AnyConsumer custom_free)
{

    REQUIRE_NON_NULL(self);

    _List_free_recursive(HEAD(self), free_content, custom_free);
    HEAD(self) = NULL;
    LAST(self) = NULL;
    SIZE(self) = 0;

}

void _List_free(List self, Boolean free_content, AnyConsumer custom_free)
{

    ENSURE_FREEABLE(self);

    _List_free_recursive(HEAD(self), free_content, custom_free);
    FREE(self);

}

void _List_free_recursive(ListItem item, Boolean free_content, AnyConsumer custom_free)
{

    ENSURE_FREEABLE(item);

    if (NON_NULL(NEXT(item)))
        _List_free_recursive(NEXT(item), free_content, custom_free);
    
    LIST_ITEM_FREE(item, free_content, custom_free);

}

/* --- private helpers */

Any _ListIterator_next(Iterator* it)
{

    ListItem index = ITER_INDEX_AS(*it, ListItem);
    ITER_INDEX(*it) = NEXT(index);
    ITER_POSITION(*it)++;

    return CONTENT(index);

}

Boolean _ListIterator_has_next(Iterator* it)
{
    return NON_NULL(ITER_INDEX(*it));
}
