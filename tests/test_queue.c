#include <assert.h>

#include "test.h"
#include "queue.h"

#define _MAX_SIZE 5000

typedef struct
{
    int data[_MAX_SIZE];
    int size;
} RefQueue;

void refInit(RefQueue *q)
{
    q->size = 0;
}

int refIsEmpty(RefQueue *q)
{
    return q->size == 0;
}

void refEnqueue(RefQueue *q, int val)
{
    if (q->size == _MAX_SIZE)
    {
        fprintf(stderr, "RefQueue overflow!\n");
        exit(1);
    }
    q->data[q->size++] = val;
}

int refDequeue(RefQueue *q)
{
    if (refIsEmpty(q))
    {
        fprintf(stderr, "RefQueue underflow!\n");
        exit(1);
    }
    int val = q->data[0];
    for (int i = 1; i < q->size; i++)
    {
        q->data[i - 1] = q->data[i];
    }
    q->size--;
    return val;
}

int refFront(RefQueue *q)
{
    if (refIsEmpty(q))
    {
        fprintf(stderr, "RefQueue empty!\n");
        exit(1);
    }
    return q->data[0];
}

static inline void test_queueInit()
{
    _TEST_BEGIN();

    Queue que;
    assert(queueInit(&que, sizeof(int)));
    assert(que.head == 0);
    assert(que.tail == 1);
    assert(que.arr.data != NULL);
    assert(queueEmpty(&que));
    assert(que.arr.currentCapacity == 16);

    queueDestroy(&que, 0);
    assert(que.arr.data == NULL);

    _TEST_END();
}

static inline void test_queueCreate()
{
    _TEST_BEGIN();

    Queue *que = queueCreate(sizeof(int));

    assert(que);
    assert(que->head == 0);
    assert(que->tail == 1);
    assert(que->arr.data != NULL);
    assert(queueEmpty(que));
    assert(que->arr.currentCapacity == 16);

    queueDestroy(que, 1);

    _TEST_END();
}

static inline void test_enQueue()
{
    _TEST_BEGIN();
    int a[48], b;

    Queue *que = queueCreate(sizeof(int));
    assert(que);

    for (int i = 0; i < 96; i++)
    {
        assert(enQueue(que, &i));
    }

    assert(que->arr.currentCapacity == 128);

    for (int i = 0; i < 128; i++)
    {
        if (i < 48)
        {
            assert(deQueue(que, a + i));
            // printf("a[%d] = %d\n", i, a[i]);
        }
        else if (i < 96)
        {
            assert(deQueue(que, &b));
            // printf("%d\n", b);
            assert(b == i);
        }
        else
        {
            assert(!deQueue(que, &b));
        }
    }

    for (int i = 0; i < 48; i++)
    {
        assert(a[i] == i);
    }

    assert(que->arr.currentCapacity == 128);

    _TEST_END();
}

#define TEST_ITERATIONS 50000

static inline void test_randomizedEnqueDeque()
{
    _TEST_BEGIN();
    Queue testQ;
    RefQueue refQ;

    int a, b;

    queueInit(&testQ, sizeof(int));
    refInit(&refQ);

    for (int i = 0; i < TEST_ITERATIONS; i++)
    {
        int op = rand() % 4; // 0=enqueue,1=dequeue,2=isEmpty
        if (op == 0)
        { // enqueue
            int val = rand() % 100;
            enQueue(&testQ, &val);
            refEnqueue(&refQ, val);
        }
        else if (op == 1)
        { // dequeue
            if (!queueEmpty(&testQ) && !refIsEmpty(&refQ))
            {
                deQueue(&testQ, &a);
                b = refDequeue(&refQ);
                assert(a == b);
            }
        }
        else if (op == 2)
        { // isEmpty
            assert(queueEmpty(&testQ) == refIsEmpty(&refQ));
        }
    }
    printf("capacity of [Queue]: %d\n", testQ.arr.currentCapacity);
    queueDestroy(&testQ, 0);
    _TEST_END();
}

int main(void)
{

    test_queueInit();
    test_queueCreate();
    test_enQueue();
    test_randomizedEnqueDeque();

    return 0;
}