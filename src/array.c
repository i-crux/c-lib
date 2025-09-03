#include "array.h"


/**
 * @brief search key in [Array]
 * 
 * @param arr: pointor of [Array]
 * @param key: key needed be searched
 * 
 * @return idx on successful. [-1] on failure
 */
static int linearSearch(const Array *arr, const void *key);



bool arrayInit(Array *arr, int eSize, int capacity, arrayElemCmp cmp) {
    arr->data = MALLOC(eSize * capacity);
    arr->elemSize = eSize;
    arr->nextIndex = 0;
    arr->initCapacity = capacity;
    arr->currentCapacity = capacity;
    arr->compare = cmp;
    arr->search = linearSearch;
    return arr->data != NULL;
}


Array *arrayCreate(int eSize, int capacity, arrayElemCmp cmp) {
    Array *arr = MALLOC(sizeof(Array));

    if (arr) {
        if(!arrayInit(arr, eSize, capacity, cmp)) {
            free(arr);
            arr = NULL;
        }
    }

    return arr;
}


void arrayDestroy(Array *arr, bool freeSelf) {
    if (arr) {
        if(arr->data) {
            FREE(arr->data);
            arr->data = NULL;
        }
        arr->elemSize = 0;
        arr->nextIndex = 0;
        arr->initCapacity = 0;
        arr->currentCapacity = 0;
        arr->compare = NULL;
        arr->search = NULL;
        if(freeSelf) {
            FREE(arr);
        }
    }
}



static int linearSearch(const Array *arr, const void *key) {
    /* element are not comparable */
    if (!arr || arr->compare == NULL) {
        goto notFind;
    }
    for (int i = 0; i < arrayElemCnt(arr); i++) {
        if (arr->compare(arrayGetAddr(arr, i), key) == 0) {
            return i;
        }
    }
    notFind:
        return -1;
}