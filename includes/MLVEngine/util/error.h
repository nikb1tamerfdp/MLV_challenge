#ifndef _ERROR_
#define _ERROR_

#include <stdlib.h>

#include "MLVEngine/util/lang.h"
#include "MLVEngine/util/contract.h"

#define MAX_ERROR_MESSAGE_LEN 256

/* Error type defining a message and a reason. */
struct _exception {
    char* file;
    int line;
    char* reason;
    char message[MAX_ERROR_MESSAGE_LEN];
};

typedef struct _exception _Exception;
typedef _Exception* Exception;

/* All public methods of Error */
Exception Exception_new(char* file, int line, char* reason, char* message, ...);
void Exception_log(Exception self, Boolean strict);
void Exception_free(Exception self);

#define NO_ERROR NULL
#define LEAVE_WITHOUT_ERROR return NO_ERROR
#define HAS_ERROR(e) (NO_ERROR != (e))
#define NOT_ERROR(e) (NO_ERROR == (e))
#define ERROR(r, m, ...) Exception_new(__FILE__, __LINE__, r, m, ## __VA_ARGS__)
#define THROW(r, m, ...) return ERROR(r, m, ## __VA_ARGS__)
#define THROW_AND_KILL(r, m, ...) Exception_log(ERROR(r, m, ## __VA_ARGS__), TRUE)
#define PROPAGATE(e) return e
#define PROPAGATE_IF_THROWN(e) if (NON_NULL(e)) { PROPAGATE(e); }

#endif
