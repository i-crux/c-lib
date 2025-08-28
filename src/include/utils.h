/*
 * common utils for all data structure
 */

#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdlib.h>
#include <stdint.h>

/*
 * using macro to define memory management function, 
 * so that we can change memory manage lib at compiling time
 */
#define MALLOC      malloc
#define FREE        free
#define CALLOC      calloc
#define REACCLOC    realloc

typedef struct {
    volatile int locked;
} SpinLock;


/**
 * @brief initialize spin lock structure
 * 
 * @param spinLockPtr: pointor to a spin lock structure
 */
#define initSpinLock(spinLockPtr)               \
    do {                                        \
        SpinLock *__isl_slp = (spinLockPtr);    \
        __isl_slp->locked = 0;                  \
    } while(0)


/**
 * @brief lock a spin lock structure
 * 
 * @param spinLockPtr: pointor to a spin lock structure
 */
#define lockSpinLock(spinLockPtr)                           \
    do {                                                    \
        SpinLock *__lsl_slp = (spinLockPtr);                \
        int     __lsl_tmp;                                  \
        do {                                                \
            __asm__ __volatile__ (                          \
                "xchg %0, %1\n"                             \
                : "=r"(__lsl_tmp), "+m"(__lsl_slp->locked)  \
                : "0"(1)                                    \
                : "memory"                                  \
            );                                              \
        } while(__lsl_tmp != 0);                            \
    } while(0)


/**
 * @brief lock a spin lock structure
 * 
 * @param spinLockPtr: pointor to a spin lock structure
 */
#define unlockSpinLock(spinLockPtr)         \
    SpinLock *__ulsl_slp = (spinLockPtr);   \
    do {                                    \
        __asm__ __volatile__ (              \
            "movl $0, %0\n"                 \
            : "=m"(__ulsl_slp->locked)      \
            :                               \
            : "memory"                      \
        );                                  \
    } while(0)


/**
 * @brief get min value by align value
 * 
 * @param alignValue: align value
 * 
 * @return mininal value after align
 */
#define minSizeByAlign(alignValue)                      \
    ({                                                  \
        typeof(alignValue) __msba_av = (alignValue);    \
        ~__msba_av + 1;                                 \
    })


/**
 * @brief get floor of a value by align
 * 
 * @param value: value need be align
 * @param align: the value of align
 * 
 * @return floor align value with type of uintptr_t
 */
#define floorAlign(value, align)                \
    ({                                          \
        uintptr_t __fa_v = (uintptr_t)(value);  \
        uintptr_t __fa_a = (uintptr_t)(align);  \
        __fa_v & __fa_a;                        \
    })


/**
 * @brief upper align a address
 * 
 * @param value: value need be align
 * @param align: the value of align
 * 
 * @return ceil align value with type of uintptr_t
 */
#define ceilAlign(value, align)                                                                 \
    ({                                                                                          \
        uintptr_t __ca_v = (uintptr_t)(value);                                                  \
        uintptr_t __ca_a = (uintptr_t)(align);                                                  \
        (__ca_v & ~__ca_a) ? (floorAlign(__ca_v, __ca_a) + minSizeByAlign(__ca_a)) : __ca_v;    \
    })

#endif /* _UTILS_H_ */