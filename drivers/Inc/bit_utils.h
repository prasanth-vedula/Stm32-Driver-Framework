#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <stdint.h>

/*==========================================================
                    Bit Definitions
==========================================================*/

#define BIT(pos)                    (1UL << (pos))

/*==========================================================
                    Bit Operations
==========================================================*/

#define SET_BIT(REG, BIT_MASK)      ((REG) |= (BIT_MASK))

#define CLEAR_BIT(REG, BIT_MASK)    ((REG) &= ~(BIT_MASK))

#define TOGGLE_BIT(REG, BIT_MASK)   ((REG) ^= (BIT_MASK))

#define READ_BIT(REG, BIT_MASK)     ((REG) & (BIT_MASK))

#define WRITE_REG(REG, VAL)         ((REG) = (VAL))

#define READ_REG(REG)               (REG)

#define MODIFY_REG(REG, CLEAR, SET) \
    WRITE_REG((REG), (((READ_REG(REG)) & ~(CLEAR)) | (SET)))

#endif /* BIT_UTILS_H */