/*
 * common utils for all data structure
 */

#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdlib.h>

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

#endif /* _UTILS_H_ */