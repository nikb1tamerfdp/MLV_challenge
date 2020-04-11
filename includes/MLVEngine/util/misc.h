#ifndef _MISC_
#define _MISC_

#include <stddef.h>

double get_absolute_time(void);

/* Gets maximum value between A and B (with securities) */
#define MAX(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a > _b ? _a : _b; })

/* Gets minimum value between A and B (with securities) */
#define MIN(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a < _b ? _a : _b; })

/* Initializes a new seed for the randomizator. */
#define INIT_RANDOM() srand(time(NULL))

#define PI 3.14159265358979323846

#define DEG_TO_RAD(angle) (angle * PI / 180)
#define RAD_TO_DEG(angle) (angle * 180 / PI)

#endif
