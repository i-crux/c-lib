#include "queue.h"
#include "utils.h"

#define _INIT_QUEUE_SIZE 16

#define _queueFull(que)                   \
    ({                                    \
        Queue *__qf_que = (Queue *)(que); \
        __qf_que->head == __qf_que->tail; \
    })

bool queueInit(Queue *que, int eSize)
{

    que->head = 0;
    que->tail = 1;

    return arrayInit(&que->arr, eSize, _INIT_QUEUE_SIZE, NULL);
}

Queue *queueCreate(int eSize)
{
    Queue *que = MALLOC(sizeof(Queue));

    ckpvThenReturn(que, NULL, NULL);

    if (!queueInit(que, eSize))
    {
        FREE(que);
        que = NULL;
    }

    return que;
}

void queueDestroy(Queue *que, bool freeSelf)
{
    arrayDestroy(&que->arr, 0);

    if (freeSelf)
    {
        FREE(que);
    }
}

bool enQueue(Queue *que, void *data)
{

    int capacity;
    int eSize = arrayElemSize(&que->arr);
    void *newArrData;

    if (_queueFull(que))
    {
        capacity = arrayCapacity(&que->arr);
        /* expand data size */
        newArrData = REACCLOC(que->arr.data, eSize * capacity * 2);

        ckpvThenReturn(newArrData, NULL, 0);
        que->arr.data = newArrData;

        que->arr.currentCapacity += capacity;

        /* move elemt to new position */
        for (int i = capacity * 2 - 1; i > que->head; i--)
        {
            memcpy(arrayGetAddr(&que->arr, i), arrayGetAddr(&que->arr, i - capacity), eSize);
        }

        /* adjust head */
        que->head += capacity;
    }

    memcpy(arrayGetAddr(&que->arr, que->tail), data, eSize);
    que->tail = queuePosAddOne(que, que->tail);

    return 1;
}

bool deQueue(Queue *que, void *data)
{
    if (queueEmpty(que))
    {
        return 0;
    }

    que->head = queuePosAddOne(que, que->head);

    memcpy(data, arrayGetAddr(&que->arr, que->head), arrayElemSize(&que->arr));

    return 1;
}