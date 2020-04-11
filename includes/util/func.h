#ifndef _FUNC_
#define _FUNC_

#include "lang.h"

/* Alias for a runnable, ie. a function taking
   zero argument and returning nothing. */
typedef FUNC(void, Runnable, void);

/* Macro to easily declare a consumer, ie. a function
   taking one argument and returning nothing. */
#define CONSUMER(i, n) FUNC(void, n, i)

/* Alias for a generic consumer, ie. consuming a generic pointer. */
typedef CONSUMER(Any, AnyConsumer);

#define BI_CONSUMER(i1, i2, n) FUNC(void, n, i1, i2)
typedef BI_CONSUMER(Any, Any, AnyBiConsumer);

#define FUNCTION(i, o, n) FUNC(o, n, i)
typedef FUNCTION(Any, Any, AnyFunction);

#define BI_FUNCTION(i1, i2, o, n) FUNC(o, n, i1, i2)
typedef BI_FUNCTION(Any, Any, Any, AnyBiFunction);

#define PREDICATE(i, n) FUNC(Boolean, n, i)
typedef PREDICATE(Any, AnyPredicate);

#define BI_PREDICATE(i1, i2, n) FUNC(Boolean, n, i1, i2)
typedef BI_PREDICATE(Any, Any, AnyBiPredicate);

#endif
