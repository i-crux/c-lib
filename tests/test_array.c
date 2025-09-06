#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"
#include "array.h"

static int cmpInt(const void *a, const void *b) {
    int intA = *(int *)a, intB = *(int *)b;
    return intA - intB;
}

static int cmpStrPtr(const void *a, const void *b) {
    return strcmp((const char *)(*(char **)a), (const char *)(*(char **)b));
}

static int cmpStr(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

static int _dummySearch(__attribute__((unused))const Array *arr, __attribute__((unused))const void *key) {
    return -1;
}

static void _visit(const void *a) {
    int intA = *(int *)a;
    printf("%d ", intA);
}

#define _INIT_CAP 4

static inline void test_arrayInit() {
    _TEST_BEGIN();
    
    Array arr;
    assert(arrayInit(&arr, sizeof(int), _INIT_CAP, cmpInt));

    assert(arr.compare == cmpInt);
    assert(arr.data != NULL);
    assert(arr.currentCapacity == _INIT_CAP);
    assert(arr.initCapacity == _INIT_CAP);
    assert(arr.elemSize == sizeof(int));
    assert(arr.search != NULL);
    assert(arr.nextIndex==0);

    arrayDestroy(&arr, 0);

    assert(arr.compare == NULL);
    assert(arr.data == NULL);
    assert(arr.currentCapacity == 0);
    assert(arr.initCapacity == 0);
    assert(arr.elemSize == 0);
    assert(arr.search == 0);
    assert(arr.nextIndex==0);

    _TEST_END();
}

static inline void test_arrayCreate() {
    _TEST_BEGIN();

    int a = 1;

    Array *arr = arrayCreate(sizeof(int), _INIT_CAP, cmpInt);
    assert(arr);
    assert(arr->compare == cmpInt);
    assert(arr->data != NULL);
    assert(arrayCapacity(arr) == _INIT_CAP);
    assert(arr->initCapacity == _INIT_CAP);
    assert(arrayElemCnt(arr) == 0);
    assert(arrayNextIdx(arr) == 0);
    assert(arrayElemSize(arr) == sizeof(int));
    assert(arrayOverFlow(arr, 0));
    assert(arrayOverFlow(arr, 2));
    assert(arrayOverFlow(arr, -1));
    assert(arrayEmpty(arr));
    assert(!arrayIsFull(arr));
    assert(arrayGetAddr(arr, 0) == arr->data);
    assert(arrayGetElem(arr, 0) == NULL);
    assert(!arraySetElem(arr, 0, &a));
    arrayTraval(arr, _visit);
    assert(arrayIsSortedAsc(arr));
    
    assert(arr->search != NULL);
    assert(arr->nextIndex==0);

    arrayDestroy(arr, 1);

    _TEST_END();
}

static inline void test_arraySetSearchFunc() {
    _TEST_BEGIN();
    
    Array *arr = arrayCreate(sizeof(int), _INIT_CAP, NULL);
    assert(arr);
    assert(arr->compare == NULL);
    assert(arr->search != _dummySearch);
    arraySetSearchFunc(arr, _dummySearch);
    assert(arr->search != _dummySearch);
    arrayDestroy(arr, 1);

    arr = arrayCreate(sizeof(int), _INIT_CAP, cmpInt);
    assert(arr);
    assert(arr->compare == cmpInt);
    assert(arr->search != _dummySearch);
    arraySetSearchFunc(arr, _dummySearch);
    assert(arr->search == _dummySearch);
    arrayDestroy(arr, 1);

    
    _TEST_END();
}


static void test_arrayInsertAndDelete() {
    _TEST_BEGIN();

    int a;

    Array *arr = arrayCreate(sizeof(int), _INIT_CAP, cmpInt);
    assert(arr);

    a = 0;
    assert(!arrayInsert(arr, 1, &a));
    assert(arrayInsert(arr, 0, &a));
    assert(*(int *)(arrayGetElem(arr, 0)) == a);
    int j = 1;

    for(int i = 1; i < 1000; i++) {
        assert(arrayInsert(arr, i, &i));
        assert(*(int *)(arrayGetElem(arr, i)) == i);
        if( i >=  _INIT_CAP * j ) {
            j <<= 1;
            assert(arrayCapacity(arr) == _INIT_CAP * j );
        }
        assert(arrayIsSortedAsc(arr));
    }

    for(int i = 0; i < 1000; i++) {
        assert(arrayDeleteIdx(arr, 0, &a));
        assert(arrayIsSortedAsc(arr));
        assert(a == i);
    }
    assert(arrayCapacity(arr) == _INIT_CAP);

    j = 1;
    for(int i = 0; i < 1000; i++) {
        assert(arrayInsert(arr, i, &i));
        assert(*(int *)(arrayGetElem(arr, i)) == i);
        if( i >=  _INIT_CAP * j ) {
            j <<= 1;
            assert(arrayCapacity(arr) == _INIT_CAP * j );
        }
        assert(arrayIsSortedAsc(arr));
    }

    for(int i = 0; i < 1000; i++) {
        assert(arrayDeleteVal(arr, &i) == 0);
        assert(arrayIsSortedAsc(arr));
    }
    assert(arrayCapacity(arr) == _INIT_CAP);
    

    arrayDestroy(arr, 1);

    _TEST_END();
}


static inline void test_elemSwap() {
    _TEST_BEGIN();

    Array *arr = arrayCreate(sizeof(int), _INIT_CAP, cmpInt);
    assert(arr);

    int j = 1;
    for(int i = 0; i < 1000; i++) {
        assert(arrayInsert(arr, i, &i));
        assert(*(int *)(arrayGetElem(arr, i)) == i);
        if( i >=  _INIT_CAP * j ) {
            j <<= 1;
            assert(arrayCapacity(arr) == _INIT_CAP * j );
        }
        assert(arrayIsSortedAsc(arr));
    }

    for(int i = 2000; i < 3000; i++) {
        assert(!arrayInsert(arr, i, &i));
    }


    assert(arraySwapElemUsingDynamicMem(arr, 0, 1));
    assert(*(int *)arrayGetElem(arr, 0) == 1);
    assert(*(int *)arrayGetElem(arr, 1) == 0);

    assert(arraySwapElemFixedBuffer(arr, 0, 1));
    assert(*(int *)arrayGetElem(arr, 0) == 0);
    assert(*(int *)arrayGetElem(arr, 1) == 1);

    assert(arraySwapElem(arr, 0, 1));
    assert(*(int *)arrayGetElem(arr, 0) == 1);
    assert(*(int *)arrayGetElem(arr, 1) == 0);

    _TEST_END();
}


static inline void test_pushPopSearchTraval() {
    _TEST_BEGIN();
    Array *arr = arrayCreate(sizeof(int), _INIT_CAP, cmpInt);
    assert(arr);

    int j = 1;
    for(int i = 0; i < 1000; i++) {
        assert(arrayPush(arr, &i));
        assert(*(int *)(arrayGetElem(arr, i)) == i);
        if( i >=  _INIT_CAP * j ) {
            j <<= 1;
            assert(arrayCapacity(arr) == _INIT_CAP * j );
        }
        assert(arrayIsSortedAsc(arr));
        if( i < 20 ) {
                arrayTraval(arr, _visit);
                printf("\n");
            }
    }
    for(int i = 0; i < 2000; i++) {
        if(i < 1000) {
            assert(arraySearch(arr, &i) == i);
            assert(arrayContains(arr, &i));
            
        } else {
            assert(arraySearch(arr, &i) == -1);
            assert(!arrayContains(arr, &i));
        }
    }
    int a;
    for(int i = 0; i < 2000; i++) {
        if(i < 1000) {
            assert(arrayPop(arr, &a));
            assert(a == 999-i);
        } else {
            assert(!arrayPop(arr, &a));
        }
    }


    _TEST_END();
}


static inline void test_elemStrSwap() {
    _TEST_BEGIN();

    char *a = "hello", *b = "bitch";

    Array *arrStrPtr = arrayCreate(sizeof(char *), _INIT_CAP, cmpStrPtr);
    arrayPush(arrStrPtr, &a);
    arrayPush(arrStrPtr, &b);

    assert(strcmp((const char *)(*(char **)arrayGetElem(arrStrPtr, 0)), "hello") == 0);
    assert(strcmp((const char *)(*(char **)arrayGetElem(arrStrPtr, 1)), "bitch") == 0);

    assert(arraySwapElemUsingDynamicMem(arrStrPtr, 0, 1));
    assert(strcmp((const char *)(*(char **)arrayGetElem(arrStrPtr, 1)), "hello") == 0);
    assert(strcmp((const char *)(*(char **)arrayGetElem(arrStrPtr, 0)), "bitch") == 0);

    assert(arraySwapElemFixedBuffer(arrStrPtr, 0, 1));
    assert(strcmp((const char *)(*(char **)arrayGetElem(arrStrPtr, 0)), "hello") == 0);
    assert(strcmp((const char *)(*(char **)arrayGetElem(arrStrPtr, 1)), "bitch") == 0);

    assert(arraySwapElem(arrStrPtr, 0, 1));
    assert(strcmp((const char *)(*(char **)arrayGetElem(arrStrPtr, 1)), "hello") == 0);
    assert(strcmp((const char *)(*(char **)arrayGetElem(arrStrPtr, 0)), "bitch") == 0);

    arrayDestroy(arrStrPtr, 1);

    a = "fuck bitch\n\t";
    b = "hell world\n\n";
    arrStrPtr = arrayCreate(sizeof("fuck bitch\n\t"), _INIT_CAP, cmpStr);
    arrayPush(arrStrPtr, a);
    arrayPush(arrStrPtr, b);

    assert(strcmp((const char *)arrayGetElem(arrStrPtr, 0), a) == 0);
    assert(strcmp((const char *)arrayGetElem(arrStrPtr, 1), b) == 0);
    printf("%s, %s\n", (char *)arrayGetElem(arrStrPtr, 0), (char *)arrayGetElem(arrStrPtr, 1));

    assert(arraySwapElemUsingDynamicMem(arrStrPtr, 0, 1));
    assert(strcmp((const char *)arrayGetElem(arrStrPtr, 1), a) == 0);
    assert(strcmp((const char *)arrayGetElem(arrStrPtr, 0), b) == 0);
    printf("%s, %s\n", (char *)arrayGetElem(arrStrPtr, 0), (char *)arrayGetElem(arrStrPtr, 1));

    assert(arraySwapElemFixedBuffer(arrStrPtr, 0, 1));
    assert(strcmp((const char *)arrayGetElem(arrStrPtr, 0), a) == 0);
    assert(strcmp((const char *)arrayGetElem(arrStrPtr, 1), b) == 0);
    printf("%s, %s\n", (char *)arrayGetElem(arrStrPtr, 0), (char *)arrayGetElem(arrStrPtr, 1));

    assert(arraySwapElem(arrStrPtr, 0, 1));
    assert(strcmp((const char *)arrayGetElem(arrStrPtr, 1), a) == 0);
    assert(strcmp((const char *)arrayGetElem(arrStrPtr, 0), b) == 0);
    printf("%s, %s\n", (char *)arrayGetElem(arrStrPtr, 0), (char *)arrayGetElem(arrStrPtr, 1));


    arrayDestroy(arrStrPtr, 1);

    _TEST_END();
}


int main(void) {
    test_arrayInit();
    test_arrayCreate();
    test_arraySetSearchFunc();
    test_arrayInsertAndDelete();
    test_elemSwap();
    test_pushPopSearchTraval();
    test_elemStrSwap();
    return 0;
}

