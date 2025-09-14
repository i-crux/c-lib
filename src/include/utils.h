/*
 * common utils for all data structure
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

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


/**
 * @brief in structure [type], get offset of [member]

 * @param __oso_type typeof[type]
 * @param __oso_member name of member
 * 
 * @return offset of [member] in type
 */
#define offsetOf(__oso_type, __oso_member) ((size_t) &((__oso_type *)0)->__oso_member)

/**
 * @brief get address of [type]

 * @param __co_ptr pointor to member;
 * @param __co_type typeof(type)
 * @param __co_member name of member
 * 
 * @return address of [type] which contained member pointed by [ptr]
 */
#define containerOf(__co_ptr, __co_type, __co_member) ((__co_type *)((char *)(__co_ptr) - offsetOf(__co_type, __co_member)))


/**
 * @brief get the smaller value
 * 
 * @param a: value [a]
 * @param b: value [b]
 * 
 * @return the smaller value
 */
#define min(_a, _b)                             \
    ({                                          \
        typeof(_a) __min_a = (_a);              \
        typeof(_b) __min_b = (_b);              \
        __min_a <= __min_b ? __min_a : __min_b; \
    })


/**
 * @brief get the bigger value
 * 
 * @param a: value [a]
 * @param b: value [b]
 * 
 * @return the bigger value
 */
#define max(_a, _b)                             \
    ({                                          \
        typeof(_a) __max_a = (_a);              \
        typeof(_b) __max_b = (_b);              \
        __max_a >= __max_b ? __max_a : __max_b; \
    })


/**
 * @brief: check the pointor p, if p == val then return ret
 * 
 * @param p: the pointor needed to be checked
 * @param val: checked val
 * 
 * @return ret if p == val else not excute return
 */
#define ckpvThenReturn(p, val, ret)                 \
    do {                                            \
        uintptr_t __ckpvtr_ptr = (uintptr_t)(p);    \
        uintptr_t __ckpvtr_val = (uintptr_t)(val);  \
        typeof(ret) __ckpvtr_ret = (ret);           \
        if( __ckpvtr_ptr == __ckpvtr_val ) {        \
            return __ckpvtr_ret;                    \
        }                                           \
    } while(0)


#define bzero(p, s)                     \
    do {                                \
        void *__bz_ptr = (void *)(p);   \
        size_t __bz_size = (size_t)(s); \
        memset(__bz_ptr, 0, __bz_size); \
    } while(0)


/**
 * @brief swap two memory 
 * 
 * @param ma pointor to the first memory
 * @param mb pointor to the 2nd memory
 * @param size memory size 
 */
#define swapMemUsingDymArr(ma, mb, size)                            \
    do {                                                            \
        size_t __sm_size = (size_t)(size);                          \
        uint8_t __sm_tmp[__sm_size]; /* for swaping tmp memory */   \
        void *__sm_a = (void *)(ma);                                \
        void *__sm_b = (void *)(mb);                                \
        memcpy((void *)__sm_tmp, __sm_a, __sm_size);                \
        memcpy(__sm_a, __sm_b, __sm_size);                          \
        memcpy(__sm_b, (void *)__sm_tmp, __sm_size);                \
    } while(0)

/**
 * @brief swap two memory 
 * IMPORTANT: [ma] must not equal [mb]
 * 
 * @param ma pointor to the first memory
 * @param mb pointor to the 2nd memory
 * @param size memory size 
 */
#define swapMem(ma, mb, size)                                                       \
    do {                                                                            \
        size_t __smxor_size = (size_t)(size);                                       \
        uint8_t *__smxor_a = (uint8_t *)(ma);                                       \
        uint8_t *__smxor_b = (uint8_t *)(mb);                                       \
        if( __smxor_a != __smxor_b ) {                                              \
            for(size_t __smxor_i = 0; __smxor_i < __smxor_size; __smxor_i++) {      \
                __smxor_a[__smxor_i] = __smxor_a[__smxor_i] ^ __smxor_b[__smxor_i]; \
                __smxor_b[__smxor_i] = __smxor_a[__smxor_i] ^ __smxor_b[__smxor_i]; \
                __smxor_a[__smxor_i] = __smxor_a[__smxor_i] ^ __smxor_b[__smxor_i]; \
            }                                                                       \
        }                                                                           \
    } while(0)

/**
 * @brief free a address 
 * @param p: pointor needed by free
 */
void freeMem(void *p);

_Thread_local static int seedSet = 0;

static inline void _setRandSeed(void) {
    if (!seedSet) {
        srand(time(NULL));
        seedSet = 1;
    }    
}


/**
 * 在 min 和 max 之前取随机数
 * @param min 随机数的下限
 * @param max 随机数的上限
 * @return 返回 min, max 之间的随机整数
 */
#define randRange(min, max) ({_setRandSeed(); rand() % ((max) - (min) + 1) + (min);})


#endif /* _UTILS_H_ */