#include "test.h"
#include "heap.h"

int __intCmp(const void *a, const void *b)
{
    int intA = *(int *)a, intB = *(int *)b;

    return intA - intB;
}

static inline void test_heapAddExtract()
{
    _TEST_BEGIN();

    srand(time(NULL));

    for (int l = 0; l < (rand() % 10000) + 1; l++)
    {
        int size = (rand() % 65535) + 1;
        Heap *hp = heapCreate(sizeof(int), 1000, __intCmp);
        Array *arr = arrayCreate(sizeof(int), 1000, __intCmp);

        for (int i = 0; i < size; i++)
        {
            int v = rand();
            heapAdd(hp, &v);
        }

        while (!heapEmpyt(hp))
        {
            int v;
            heapExtract(hp, &v);
            arrayPush(arr, &v);
        }

        assert(arrayIsSortedDesc(arr));
        assert(hp->nextIndex == 0);
        arrayDestroy(arr, 1);
        heapDestroy(hp, 1);
        printf("%d: done\n", l);
    }

    _TEST_END();
}

static inline void test_heapReplaceHeapify()
{
    _TEST_BEGIN();

    for (int l = 0; l < (rand() % 10000) + 1; l++)
    {
        int size = (rand() % 65535) + 1;
        Heap *hp = heapCreate(sizeof(int), 1000, __intCmp);

        int __max = 0;

        for (int i = 0; i < size; i++)
        {
            int v = rand();
            __max = max(__max, v);
            arrayPush(hp, &v);
        }

        heapHeapify(hp);

        assert(*((int *)hp->data) == __max);

        int v = rand(), ret;
        heapRaplace(hp, &v, &ret);
        assert(ret == __max);

        heapDestroy(hp, 1);
    }

    _TEST_END();
}

int main(void)
{

    test_heapAddExtract();
    test_heapReplaceHeapify();

    return 0;
}