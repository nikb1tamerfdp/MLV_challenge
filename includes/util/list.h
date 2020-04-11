#ifndef _LIST_
#define _LIST_

#include "lang.h"
#include "func.h"
#include "iterator.h"

/* Alias for a list cell (for internal usage) */
typedef struct _list_item _ListItem;

/* Alias for a pointer on a list cell (for public usage,
   assuming that all list cells will be allocated dynamically) */
typedef _ListItem* ListItem;

/* List cell containing a generic object. */
struct _list_item {

    Any content;
    ListItem next;

    /** void free(ListItem self, Boolean free_content, AnyConsumer custom_free)
        -----------------------------------------------------------------------
        Deletes the given list item, freeing all allocated memory.
        Content can be deallocated if needed.
        You can provide a custom deallocator (will use simple free by default) */
    FUNC(void, _free, ListItem, Boolean, AnyConsumer);

};

#define LIST_ITEM_CONTENT(self) FIELD(self, content)
#define LIST_ITEM_CONTENT_AS(self, type) CAST(type, LIST_ITEM_CONTENT(self))
#define LIST_ITEM_NEXT(self) FIELD(self, next)
#define LIST_ITEM_FREE(self, free_content, custom_free) METHOD(self, _free, free_content, custom_free)

/* Alias for a list (for internal usage) */
typedef struct _list _List;

/* Alias for a pointer on a list (for public usage,
   assuming that all lists will be allocated dynamically) */
typedef struct _list* List;

/* List with size caching and O(1) appending. */
struct _list {

    ListItem head;
    ListItem last;
    int size;

    /** ListItem append(List self, Any content)
        ---------------------------------------
        Appends an element to the given list. */
    FUNC(ListItem, _append, List, Any);

    /** ListItem append_at(List self, int position, Any content)
        --------------------------------------------------------
        Appends an element to the given list, at a given position.
        Please note that position cannot be out of bounds :
        - if zero or negative, the element is inserted as new head.
        - if greater than the list size, the element is inserted at the end. */
    FUNC(ListItem, _append_at, List, int, Any);

    /** ListItem push(List self, Any content)
        -------------------------------------
        Inserts an element as new head of given list. */
    FUNC(ListItem, _push, List, Any);

    /** void for_each(List self, AnyConsumer action, AnyPredicate filter)
        -----------------------------------------------------------------
        Sends each element of the given list to a generic consumer, with
        an optional filter if you want to select only particular elements. */
    FUNC(void, _for_each, List, AnyConsumer, AnyPredicate);

    /** void for_each_indexed(List self, BiConsumer(int, Any) action, BiPredicate(int, Any) filter)
        -------------------------------------------------------------------------------------------
        Sends each element of the given list and its index to a generic
        bi-consumer, with an optional filter if you want to select only
        particular elements and/or indexes. */
    FUNC(void, _for_each_indexed, List, BI_CONSUMER(int, Any, action), BI_PREDICATE(int, Any, filter));
    
    /** Any get(List self, int index)
        -----------------------------
        Returns the element in the list stored at given index or 
        throws a fatal error if it doesn't exist. */
    FUNC(Any, _get, List, int);

    /** ListIterator iterator(List self)
        --------------------------------
        Creates an iterator for the given list. */
    FUNC(Iterator, _iterator, List);

    /** Boolean is_empty(List self)
        ---------------------------
        Checks if the list is empty. */
    FUNC(Boolean, _is_empty, List);

    /** Any remove(List self, int index)
        --------------------------------
        Removes the element in the list stored at given index or 
        throws a fatal error if it doesn't exist. Content of the
        removed cell is returned. */
    FUNC(Any, _remove, List, int);

    /** void clear(List self, Boolean free_content, AnyConsumer custom_free)
        --------------------------------------------------------------------
        Clears the given list, removing all element.
        Elements in each cell can be deallocated if needed.
        You can provide a custom deallocator (will use
        simple free by default) */
    FUNC(void, _clear, List, Boolean, AnyConsumer);

    /*  void free(List self, Boolean free_content, AnyConsumer custom_free)
        -------------------------------------------------------------------
        Deletes the given list, freeing all allocated memory.
        Elements in each cell can be deallocated if needed.
        You can provide a custom deallocator (will use simple free by default) */
    FUNC(void, _free, List, Boolean, AnyConsumer);

};

#define LIST_OF(type) List
#define LIST_HEAD(self) FIELD(self, head)
#define LIST_LAST(self) FIELD(self, last)
#define LIST_SIZE(self) FIELD(self, size)
#define LIST_APPEND(self, content) METHOD(self, _append, content)
#define LIST_APPEND_AT(self, position, content) METHOD(self, _append_at, position, content)
#define LIST_PUSH(self, content) METHOD(self, _push, content)
#define LIST_FOR_EACH(self, action, filter) METHOD(self, _for_each, action, filter)
#define LIST_FOR_EACH_INDEXED(self, action, filter) METHOD(self, _for_each_indexed, action, filter)
#define LIST_GET(self, position) METHOD(self, _get, position)
#define LIST_GET_AS(self, position, type) CAST(type, LIST_GET(self, position))
#define LIST_ITERATOR(self) METHOD(self, _iterator)
#define LIST_IS_EMPTY(self) (LIST_SIZE(self) == 0)
#define LIST_REMOVE(self, position) METHOD(self, _remove, position)
#define LIST_REMOVE_AS(self, position, type) CAST(type, LIST_REMOVE(self, position))
#define LIST_CLEAR(self, free_content, custom_free) METHOD(self, _clear, free_content, custom_free)
#define LIST_FREE(l, free_content, custom_free) METHOD(l, _free, free_content, custom_free)

typedef struct _list_factory ListFactory;

struct _list_factory {

    /* List new();
       Creates and returns a new empty list. */
    FUNC(List, _new);

};

extern const ListFactory Lists;

#define LIST_NEW() Lists._new()

#endif
