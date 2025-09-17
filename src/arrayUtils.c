#include "arrayUtils.h"
#include "heap.h"


void arrayInsertionSortIdx(Array *arr, const int begin, const int end) {

    if(!arr || !arr->compare) {
        return;
    }
    
    for(int i = begin + 1; i <= end; i++) {
        for(int j = i; 
            j > begin && arr->compare(arrayGetElem(arr, j), arrayGetElem(arr, j-1)) < 0; 
            j--) 
        {
            arraySwapElem(arr, j, j-1);
        }
    }
}


void arrayInsertionSort(Array *arr) {
    arrayInsertionSortIdx(arr, 0, arrayElemCnt(arr) - 1);
}


void arraySelectionSort(Array *arr) {
    int minIdx;

    if(!arr || !arr->compare) {
        return;
    }

    for(int i = 0; i < arrayElemCnt(arr) - 1; i++) {
        minIdx = i;
        for(int j = i+1; j < arrayElemCnt(arr); j++) {
            if(arr->compare(arrayGetElem(arr, minIdx), arrayGetElem(arr, j)) > 0) {
                minIdx = j;
            }
        }
        arraySwapElem(arr, minIdx, i);
    }
}


void arrayBubbleSort(Array *arr) {
    int cnt;
    int chidx;

    if(!arr || !arr->compare) {
        return;
    }

    cnt = arrayElemCnt(arr);

    /* arr->data[cnt-i, cnt) is already sorted */
    for(int i = 0; i < cnt - 1; /* void */) {
        chidx = 0;
        for(int j = 0; j < cnt - i - 1; j++) {
            if(arr->compare(arrayGetElem(arr, j), arrayGetElem(arr, j+1)) > 0) {
                arraySwapElem(arr, j, j+1);
                chidx = j+1;
            }
        }
        i = cnt - chidx - 1;
    }
}


/**
 * @brief get the started value of [h] in shell sort
 * @param arrlen: length of [Array]
 * @return begin [h] for shell sort
 */
#define _shellBeginH(arrLen)                        \
    ({                                              \
        int __sbh_al = (arrLen) / 2, __sbh_ret = 1; \
        while(__sbh_ret < __sbh_al) {               \
            __sbh_ret = __sbh_ret * 3 + 1;          \
        }                                           \
        __sbh_ret;                                  \
    })


/**
 * @brief get next [h] for shell sort
 * @param h: current value of [h]
 * @return next [h] for shell sort
 */
#define _shellNextH(h)      \
    ({                      \
        int __snh_h = (h);  \
        __snh_h / 3;        \
    })


void arrayShellSort(Array *arr) {
    int arrLen;

    if(!arr || !arr->compare) {
        return;
    }

    arrLen = arrayElemCnt(arr);

    for(int step = _shellBeginH(arrLen); step >= 1; step = _shellNextH(step)) {
        for(int i = 0; i < arrLen - step; i++) {
            for(int j = i + step; 
                j >= step && arr->compare(arrayGetElem(arr, j), arrayGetElem(arr, j - step)) < 0; 
                j -= step) 
            {
                arraySwapElem(arr, j, j - step);
            }
        }
    }
}


/**
 * @brief merge arr[begin, mid], arr[mid+1, end]
 * 
 * @param arr: pointor to a [Array]
 * @param begin: begin index
 * @param mid: mid index
 * @param end: end index
 * @param tmp: tmp space
 */
static inline void __merge(Array *arr, int begin, int mid, int end, void *tmp) {
    int     i, j, k, leftCnt;
    void    *left;
    size_t  eSize = arrayElemSize(arr);

    for(i = k = begin, j = mid+1; i <= mid && j <= end; /* void */) {
        if(arr->compare(arrayGetElem(arr, i), arrayGetElem(arr, j)) <= 0) {
            memcpy( tmp + k * eSize, arrayGetElem(arr, i), eSize);
            i++;
        } else {
            memcpy( tmp + k * eSize, arrayGetElem(arr, j), eSize);
            j++;
        }
        k++;
    }

    left = i <= mid ? arrayGetElem(arr, i) : arrayGetElem(arr, j);
    leftCnt = end - k + 1;
    if(leftCnt) {
        memcpy(tmp + k * eSize, left, leftCnt * eSize);
    }
    /* copy back to Array->data */
    memcpy(arrayGetElem(arr, begin), tmp + begin * eSize, (end - begin + 1) * eSize);
}

void arrayMergeSort(Array *arr) {
    int i, sz = ARRAY_INSERTION_SORT_SIZE, m, r;
    int size = arrayElemCnt(arr);
    __attribute__((__cleanup__(freeMem))) void *tmp;

    if(!arr || !arr->compare) {
        return;
    }

    tmp = MALLOC(arrayElemSize(arr) * arrayElemCnt(arr));

    if(!tmp) {
        return;
    }

    do {
        for (i = 0; i < size; i += sz << 1) {
            m = i + sz - 1;
            r = i + (sz << 1) - 1;
            r = r < size ? r : size - 1;
            if(sz == ARRAY_INSERTION_SORT_SIZE) {
                /* using insertion sort */
                if( m <= r ) {
                    arrayInsertionSortIdx(arr, i, m);
                    if( m + 1 < r ) {
                        arrayInsertionSortIdx(arr, m+1, r);
                    }
                } else {
                    arrayInsertionSortIdx(arr, i, r);
                }
            }
            if( m < r && arr->compare(arrayGetElem(arr, m), arrayGetElem(arr, m+1)) > 0 ) {
                __merge(arr, i, m, r, tmp);
            }
        }
        sz <<= 1;
    } while(sz <= size);
}


/**
 * @brief split a [Array]
 * @param arr: pointor to a [Array]
 * @param l: begin index
 * @param r: end index
 * @return [index] splited
 */
static inline int __partition(Array *arr, int l, int r) {
    int p = randRange(l, r);
    int i, j;
    void *e;

    arraySwapElem(arr, l, p);
    e = arrayGetElem(arr, l);
    /* arr->data[l+1, j] < arr->data[l]; arr->data[j+1, i-1] >= arr->data[l] */
    for(i = l + 1, j = l; i <= r; i++) {
        if(arr->compare(arrayGetElem(arr, i), e) < 0) {
            ++j;
            arraySwapElem(arr, i, j);
        }
    }
    arraySwapElem(arr, l, j);

    return j;
}


/**
 * @brief split a [Array]
 * @param arr: pointor to a [Array]
 * @param l: begin index
 * @param r: end index
 * @return [index] splited
 */
static inline int __partition2way(Array *arr, int l, int r) {
    int i, j;
    int p = randRange(l, r);
    void *e;
    arraySwapElem(arr, l, p);

    e = arrayGetElem(arr, l);

    i = l + 1;
    j = r;
    while(1) {
        for(/* void */; i <= j && arr->compare(arrayGetElem(arr, i), e) < 0; i++) {
            /* void */
        }
         for(/* void */; i <= j && arr->compare(arrayGetElem(arr, j), e) > 0; j--) {
            /* void */
        }

        if( i >= j ) {
            break;
        }
        arraySwapElem(arr, i, j);
        i++;
        j--;
    }
    arraySwapElem(arr, l, j);
    return j;
}


/**
 * @brief execute a [quick sort]
 * 
 * @param arr: pointor to a [Array]
 * @param begin: start index
 * @param end: end index
 */
static inline void __doQuichSort(Array *arr, int begin, int end) {
    int partition;

    if (end <= begin) {
        return;
    } 

    if(end - begin <= ARRAY_INSERTION_SORT_SIZE) {
        arrayInsertionSortIdx(arr, begin, end);
    } else {
        partition = __partition(arr, begin, end);
        __doQuichSort(arr, begin, partition-1);
        __doQuichSort(arr, partition+1, end);
    }
}


void arrayQuickSort(Array *arr) {
    if(!arr || !arr->compare) {
        return;
    }
    __doQuichSort(arr, 0, arrayElemCnt(arr) - 1);
}



/**
 * @brief execute a [quick sort two way]
 * 
 * @param arr: pointor to a [Array]
 * @param begin: start index
 * @param end: end index
 */
static inline void __doQuichSort2Way(Array *arr, int begin, int end) {
    int partition;

    if (end <= begin) {
        return;
    } 

    if(end - begin <= ARRAY_INSERTION_SORT_SIZE) {
        arrayInsertionSortIdx(arr, begin, end);
    } else {
        partition = __partition2way(arr, begin, end);
        __doQuichSort2Way(arr, begin, partition-1);
        __doQuichSort2Way(arr, partition+1, end);
    }
}


void arrayQuickSort2way(Array *arr) {
    if(!arr || !arr->compare) {
        return;
    }
    __doQuichSort2Way(arr, 0, arrayElemCnt(arr) - 1);
}


/**
 * @brief execute a [quick sort three way]
 * 
 * @param arr: pointor to a [Array]
 * @param begin: start index
 * @param end: end index
 */
void __doQuickSort3way(Array *arr, int begin, int end) {
    int i, j, k, p, res;
    void *e;
    if(end <= begin) {
        goto _done;
    }
    if(end - begin <= ARRAY_INSERTION_SORT_SIZE) {
        arrayInsertionSortIdx(arr, begin, end);
        goto _done;
    }

    p = randRange(begin, end);
    arraySwapElem(arr, begin, p);
    e = arrayGetElem(arr, begin);

    /* 
     * arr->data[begin + 1, j] < e; 
     * arr->data[j+1, k] == e; 
     * arr->data[k+1, i-1] > e 
     */
    for(i = begin + 1, k = j = begin; i <= end; i++) {
        res = arr->compare(arrayGetElem(arr, i), e);
        if(res < 0) {
            ++k;
            ++j;
            arraySwapElem(arr, i, k);   /* swap(<e, >e) */
            arraySwapElem(arr, j, k);   /* swap(==e,<e) */
        } else if (res == 0) {
            ++k;
            arraySwapElem(arr, k, i);   /* swap(>e, ==e) */
        }
    }
    arraySwapElem(arr, begin, j);       /* swap(==e, <e) */
    __doQuickSort3way(arr, begin, j-1);
    __doQuickSort3way(arr, k + 1, end);
    
_done:
    return;
}

void arrayQuickSort3way(Array *arr) {
    if(!arr || !arr->compare) {
        return;
    }
    __doQuickSort3way(arr, 0, arrayElemCnt(arr) - 1);
}


void arrayHeapSort(Array *arr) {
    int cnt;

    if(!arr || !arr->compare) {
        return;
    }

    cnt = arrayElemCnt(arr);

    heapHeapify(arr);

    while(!heapEmpyt(arr)) {
        arraySwapElem(arr, 0, arrayElemCnt(arr) - 1);
        arrayDecIdx(arr);
        heapSiftDown(arr, 0);
    }
    arr->nextIndex = cnt;
}



void *arraySelectKthElemet(Array *arr, int k) {
    void *e = NULL, *tmp;
    int p, l, r, i, j, res;

    if(arr == NULL || arr->compare == NULL) {
        goto _done;
    }

    l = 0; 
    r = arrayElemCnt(arr) - 1;

    if( k < 0 || k > r ) {
        goto _done;
    }

    do {
        p = randRange(l, r);
        arraySwapElem(arr, p, l);
        tmp = arrayGetElem(arr, l);
        /* arr->data[l+1, j] < tmp; arr->data[j+1, i-1] >= tmp */
        for(i = l + 1, j = l; i <= r; i++) {
            res = arr->compare(arrayGetElem(arr, i), tmp);
            if(res < 0) {
                j++;
                arraySwapElem(arr, i, j);
            }
        }
        if(k < j) {
            r = j - 1;
        } else {
            l = j + 1;
        }
   } while( k == j);

   e = arrayGetElem(arr, k);
_done:
    return e;
}


void *arrayBinSearch(Array *arr, void *key) {
    int l, r, m, res;
    void *ret = NULL;

    if (!arr || !arr->compare) {
        goto _done;
    }

    l = 0;
    r = arrayElemCnt(arr) - 1;

    while(l <= r) {
        m = l + (r-l) / 2;
        res = arr->compare(arrayGetElem(arr, m), key);
        if(res == 0) {
            ret = arrayGetElem(arr, m);
            goto _done;
        } else if(res > 0) {
            r = m -1;
        } else {
            l = m + 1;
        }
    }

_done:
    return ret;
}