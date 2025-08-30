#ifndef _BUDDY_SYS_H_
#define _BUDDY_SYS_H_

#include <stdint.h>
#include "doubleList.h"
#include "utils.h"



/* max order of buddy sys. in this order, max mem could be alloced is 256T (40 + 8(min mem)) */
#define BUDDY_MAX_ORDER     40


typedef struct {
    uintptr_t       blockStart;                 /* the whole memory begin addr */
    size_t          blockSize;                  /* whole memory end addr */
    uintptr_t       start;                      /* memory begin addr after align */
    size_t          size;                       /* memory size after align */
    SpinLock        lock;                       /* using in multiple thread env */
    DoubleList      array[BUDDY_MAX_ORDER];     /* free mem list of different [order] */
    uint8_t         magic;                      /* magic num of buddy sys. */
} Buddy;



/**
 * @brief initialize buddy system
 * IMPORTANT: this function can and just can called onec on an buddy system
 * 
 * @param ptr: pointor to a block of memory
 * @param size: size of the memory point by [ptr].(the size should larger than sizeof(Buddy))
 * @param magic: magic number, should not be 0
 *
 * @return: pointor to a [Buddy] structure on successful, NULL on failure.
 */
Buddy *initBuddy(void *ptr, size_t size, uint8_t magic);


/**
 * @brief alloc memory from buddy system. and set the content of memory to '0'
 * 
 * @param bp: pointor to [Buddy] structure
 * @param size: size of the memory need to be alloced
 *
 * @return: pointor to the memory alloced on successful. NULL on failure
 */
void *buddyAlloc(Buddy *bp, size_t size);


/**
 * @brief free a memory alloced in a buddy system
 * 
 * @param ptr: 
 */
void buddyFree(void *ptr);


/**
 * @brief destory buddy system
 * 
 * @param bp: pointor to [Buddy] structure
 */
#define buddyDestory(bp)                                \
    do {                                                \
        Buddy *__bd_bp = (Buddy *)(bp);                 \
        bzero(__bd_bp->blockStart, __bd_bp->blockSize); \
    } while(0)
#endif  /* _BUDDY_SYS_H_ */
