#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include "heap.h"

#define PriorityQueue Heap

#define PriorityQueueInit heapInit

#define PriorityQueueCreate heapCreate

#define PriorityQueueEnqueue heapAdd

#define PriorityQueueDequeue heapExtract

#define PriorityQueueDestroy heapDestroy

/**
 * @brief get the first element of PriorityQueue
 * 
 * @param pq: pointor to [PriorityQueue]
 */
#define PriorityQueueGetFront(pq) arrayGetElem(pq, 0)

#endif  /* _PRIORITY_QUEUE_H_ */