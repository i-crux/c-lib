#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "array.h"

typedef struct
{
    Array arr;
    int head;
    int tail;
} Queue;

/**
 * @brief initialize a [Queue], memory for [que] is managed by caller
 *
 * @param que: pointor to [Queue]
 * @param eSize: size of single element
 *
 * @return 1 on successfull. 0 on failure
 */
bool queueInit(Queue *que, int eSize);

/**
 * @brief create a [Queue]
 *
 * @param eSize: size of single element
 *
 * @return pointor to a [Queue] on successful. [NULL] on failure
 */
Queue *queueCreate(int eSize);

/**
 * @brief destroy a [Queue]
 * @param que: pointor of [Queue]
 * @param freeSelf: free [que] or not
 */
void queueDestroy(Queue *que, bool freeSelf);

/**
 * @brief get the value added by 1 of head or tail
 *
 * @param que: pointor of [Queue]
 * @param pos: [Queue->head or Queue->tail]
 *
 * @return new value after added by 1
 */
#define queuePosAddOne(que, pos)                            \
    ({                                                      \
        Queue *__qhiv_que = (Queue *)(que);                 \
        int __qhiv_pos = (int)(pos);                        \
        (__qhiv_pos + 1) % __qhiv_que->arr.currentCapacity; \
    })

/**
 * @brief check [que] is empty or not
 *
 * @param que: pointor of [Queue]
 *
 * @return 1 if empty, 0 otherwise
 */
#define queueEmpty(que)                                             \
    ({                                                              \
        Queue *__qe_que = (Queue *)(que);                           \
        queuePosAddOne(__qe_que, __qe_que->head) == __qe_que->tail; \
    })

/**
 * @brief enqueue
 *
 * @param que: pointor of [Queue]
 * @param data: pointor to data
 *
 * @return 1 on successfull. 0 on failure
 */
bool enQueue(Queue *que, void *data);

/**
 * @brief dequeue
 *
 * @param que: pointor of [Queue]
 * @param data: pointor to data, recieve the data
 *
 * @return 1 on successfull. 0 on failure
 */
bool deQueue(Queue *que, void *data);

#endif /* _QUEUE_H_ */