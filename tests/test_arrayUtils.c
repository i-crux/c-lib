#include "test.h"
#include "arrayUtils.h"



static inline void test_arraySort() {
    _TEST_BEGIN();

    Array *arr = genIntArray(10000, 1);
    assert(arr);
    assert(arrayIsSortedAsc(arr));
    FREE(arr);
    
    arr = genIntArray(10000, 0);
    assert(arr);
    _measureBegin();
    arrayInsertionSort(arr);
    _measureEnd(arrayInsertionSort, 10000);
    assert(arrayIsSortedAsc(arr));
    FREE(arr);

    arr = genIntArray(10000, 0);
    assert(arr);
    _measureBegin();
    arraySelectionSort(arr);
    _measureEnd(arraySelectionSort, 10000);
    assert(arrayIsSortedAsc(arr));
    FREE(arr);

    for (int i = 0; i < 100; i++) {
        int size = rand() % 100000 + 1;
        arr = genIntArray(size, 0);

        int key = rand();
        ((int *)(arr->data))[0] = key;

        assert(arr);
        _measureBegin();
        arrayMergeSort(arr);
        _measureEnd(arrayMergeSort, size);
        assert(arrayIsSortedAsc(arr));
        FREE(arr);

    }
    printf("========================\n\n");
    for (int i = 0; i < 100; i++) {
        int size = rand() % 100000 + 1;
        arr = genIntArray(size, 0);

        int key = rand();
        ((int *)(arr->data))[0] = key;

        assert(arr);
        _measureBegin();
        arrayQuickSort(arr);
        _measureEnd(arrayQuickSort, size);
        assert(arrayIsSortedAsc(arr));

        void *p = arrayBinSearch(arr, &key);
        assert(p);

        FREE(arr);
    }

    printf("========================\n\n");
    for (int i = 0; i < 100; i++) {
        int size = rand() % 100000 + 1;
        arr = genIntArray(size, 0);

        int key = rand();
        ((int *)(arr->data))[0] = key;

        assert(arr);
        _measureBegin();
        arrayQuickSort2way(arr);
        _measureEnd(arrayQuickSort2way, size);
        assert(arrayIsSortedAsc(arr));

        void *p = arrayBinSearch(arr, &key);
        assert(p);

        FREE(arr);
    }

    printf("========================\n\n");
    for (int i = 0; i < 100; i++) {
        int size = rand() % 100000 + 1;
        arr = genIntArray(size, 0);

        int key = rand();
        ((int *)(arr->data))[0] = key;

        assert(arr);
        _measureBegin();
        arrayQuickSort3way(arr);
        _measureEnd(arrayQuickSort3way, size);
        assert(arrayIsSortedAsc(arr));

        void *p = arrayBinSearch(arr, &key);
        assert(p);

        FREE(arr);
    }

    printf("========================\n\n");
    for (int i = 0; i < 100; i++) {
        int size = rand() % 100000 + 1;
        arr = genIntArray(size, 0);

        int key = rand();
        ((int *)(arr->data))[0] = key;

        assert(arr);
        _measureBegin();
        arrayHeapSort(arr);
        _measureEnd(arrayHeapSort, size);
        assert(arrayIsSortedAsc(arr));

        void *p = arrayBinSearch(arr, &key);
        assert(p);

        FREE(arr);
    }

    printf("========================\n\n");
    for (int i = 0; i < 100; i++) {
        int size = rand() % 100000 + 1;
        arr = genIntArray(size, 0);
        assert(arr);
        _measureBegin();
        int k = rand() % size;
        void *e = arraySelectKthElemet(arr, k);
        printf("the %dth [%d]  ", k, *(int *)e);
        _measureEnd(arraySelectKthElemet, size);
        FREE(arr);
    }

    _TEST_END();
}


int main(void) {

    test_arraySort();

    return 0;
}