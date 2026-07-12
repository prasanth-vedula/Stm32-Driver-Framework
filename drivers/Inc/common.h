#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>

/*==========================================================
                    Boolean Definitions
==========================================================*/

#ifndef TRUE
#define TRUE    1U
#endif

#ifndef FALSE
#define FALSE   0U
#endif

/*==========================================================
                    Utility Macros
==========================================================*/

#define ARRAY_SIZE(arr) \
    (sizeof(arr) / sizeof((arr)[0]))

#define UNUSED(x) \
    ((void)(x))

#define MIN(a,b) \
    (((a) < (b)) ? (a) : (b))

#define MAX(a,b) \
    (((a) > (b)) ? (a) : (b))

#define ABS(x) \
    (((x) < 0) ? -(x) : (x))

#define ALIGN4(x) \
    (((x) + 3U) & ~3U)

#define ALIGN8(x) \
    (((x) + 7U) & ~7U)

#endif /* COMMON_H */