#ifndef _HEAP_H_
#define _HEAP_H_

#include "array.h"

/*
 * - heapify
 */
#define Heap Array

/*
 ! IMPORTANT: [compare] function must not be [NULL]
 */
#define heapInit arrayInit

#define heapCreate arrayCreate

#define heapDestroy arrayDestroy

#define heapEmpyt arrayEmpty

/**
 * @brief get idx of left child
 *
 * @param idx: idx of parent
 *
 * @return idx of left child
 */
#define heapLeftChildIdx(idx)        \
    ({                               \
        int __hlci_idx = (int)(idx); \
        (__hlci_idx << 1) + 1;       \
    })

/**
 * @brief get idx of right child
 *
 * @param idx: idx of parent
 *
 * @return idx of right child
 */
#define heapRightChildIdx(idx)       \
    ({                               \
        int __hrci_idx = (int)(idx); \
        (__hrci_idx << 1) + 2;       \
    })

/**
 * @brief get idx of parent
 *
 * @param idx: idx of child
 *
 * @return idx of parent
 */
#define heapParentIdx(idx)          \
    ({                              \
        int __hpi_idx = (int)(idx); \
        (__hpi_idx - 1) >> 1;       \
    })

/**
 * @brief sift down a element of idx
 *
 * @param hp: pointor to a [Heap]
 * @param idx: index of element
 */
void heapSiftDown(Heap *hp, int idx);

/**
 * @brief sift up a element of idx
 *
 * @param hp: pointor to a [Heap]
 * @param idx: index of element
 */
void heapSiftUp(Heap *hp, int idx);

/**
 * @brief extract the element of heap top
 *
 * @param hp: pointor to a [Heap]
 * @param retval: pointor to get the former top element
 */
void heapExtract(Heap *hp, void *retval);

/**
 * @brief add a element to a heap
 *
 * @param hp: pointor to a [Heap]
 * @param value: pointor to the new element
 */
bool heapAdd(Heap *hp, void *value);

/**
 * @brief replace the element of heap top
 *
 * @param hp: pointor to a [Heap]
 * @param ep: pointor to a [element]
 * @param retval: pointor to get the former top element
 */
void heapRaplace(Heap *hp, void *ep, void *retval);

/**
 * @brief heapify a [Array]
 *
 * @param hp: pointor to a [Heap]
 */
void heapHeapify(Heap *hp);

#endif /* _HEAP_H_ */