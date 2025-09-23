#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "buddySys.h"
#include "test.h"

#define MAX_PTRS 256 // max alloced ptr at same time

static void _visit(DoubleListNode *dlnp)
{
    printf("\tdln->prev=%p, dln=%p, dln->next=%p\n",
           dlnp->prev, dlnp, dlnp->next);
}

#define _travalBuddy(bp)                                 \
    do                                                   \
    {                                                    \
        for (uint8_t o = 0; o < BUDDY_MAX_ORDER; o++)    \
        {                                                \
            if (bp->array[o].nodeCnt != 0)               \
            {                                            \
                printf("order=%u\n", o);                 \
                travalDoubleList(&bp->array[o], _visit); \
            }                                            \
        }                                                \
        printf("========\n");                            \
    } while (0)

static void test_Buddy()
{
    _TEST_BEGIN();

    srand(time(NULL));
    size_t size = 4096;
    uint8_t magic = 0x84;
    Buddy *bp;

    void *ptr = MALLOC(size);
    assert(ptr);

    bp = initBuddy(ptr, size, magic);
    assert(bp);
    printf("bp=%p\n", bp);

    _travalBuddy(bp);

    /* basic alloc  */
    void *a = buddyAlloc(bp, 64);
    printf("a = %p\n", a);
    _travalBuddy(bp);

    void *b = buddyAlloc(bp, 32);
    printf("b = %p\n", b);
    _travalBuddy(bp);

    void *c = buddyAlloc(bp, 32);
    printf("c = %p\n", c);
    _travalBuddy(bp);

    printf("\n######### free =============\n");
    buddyFree(a);
    _travalBuddy(bp);
    buddyFree(b);
    _travalBuddy(bp);
    buddyFree(c);
    _travalBuddy(bp);

    a = buddyAlloc(bp, 64);
    printf("a = %p\n", a);
    _travalBuddy(bp);

    b = buddyAlloc(bp, 64);
    printf("b = %p\n", b);
    _travalBuddy(bp);

    printf("\n######### free =============\n");
    buddyFree(a);
    _travalBuddy(bp);
    buddyFree(b);
    _travalBuddy(bp);

    printf("Running random stress test...\n");

    void *ptrs[MAX_PTRS] = {0}; /* store the ptr alloced */

    int operations = 10000; /* operation times */
    int allocated = 0;

    for (int i = 0; i < operations; i++)
    {
        if ((rand() % 2 == 0) && allocated < MAX_PTRS)
        {
            // 随机分配
            size_t req = (rand() % 128) + 1; // alloc 1-128 bytes
            void *p = buddyAlloc(bp, req);
            if (p)
            {
                // 存到数组
                for (int j = 0; j < MAX_PTRS; j++)
                {
                    if (!ptrs[j])
                    {
                        ptrs[j] = p;
                        allocated++;
                        break;
                    }
                }
            }
        }
        else
        {
            // 随机释放
            int idx = rand() % MAX_PTRS;
            if (ptrs[idx])
            {
                buddyFree(ptrs[idx]);
                ptrs[idx] = NULL;
                allocated--;
            }
        }
    }

    // 最后全部释放
    for (int j = 0; j < MAX_PTRS; j++)
    {
        if (ptrs[j])
        {
            buddyFree(ptrs[j]);
            ptrs[j] = NULL;
        }
    }

    _travalBuddy(bp);

    printf("[X] Random stress test finished successfully.\n");

    a = buddyAlloc(bp, 512);
    printf("a = %p\n", a);
    _travalBuddy(bp);
    buddyFree(a);
    _travalBuddy(bp);

    _TEST_END();
}

int main(void)
{
    test_Buddy();
    return 0;
}