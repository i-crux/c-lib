#ifndef _ARRAY_H_
#define _ARRAY_H_
#include <stdlib.h>
#include <string.h>
#include "utils.h"


typedef struct ArrayStruct Array;

/**
 * @brief compare two elements in a [Array]
 * @param a: element [a]
 * @param b: element [b]
 * @return a == b:0; a < b:<0 ; a > b:>0
 */
typedef int (*arrayElemCmp)(const void *a, const void *b);

/**
 * @brief search key in [Array]
 * @param arr: pointor of [Array]
 * @param key: key needed be searched
 * @return idx on successful. [-1] on failure
 */
typedef int (*arrayElemSearch)(const Array *arr, const void *key);

/**
 * @brief visit function when traval a [Array]
 * @param a: element in [Array]
 */
typedef void (*arrayElemVisit)(const void *a);

struct ArrayStruct {
    void            *data;              /* data area for element */
    size_t          elemSize;           /* size of element */
    int             nextIndex;          /* index for next element */
    int             initCapacity;       /* initialized array capacity   */
    int             currentCapacity;    /* current array capacity */
    arrayElemCmp    compare;            /* compare function, if [NULL], element is not comparable */
    arrayElemSearch search;             /* search function for element only valid when element is comparable */
};



/**
 * @brief initialize a [Array], memory of [Array] managed by caller
 * @param arr: pointor of [Array]
 * @param eSize: size of single element
 * @param capacity: initialized capacity
 * @param cmp: compare function, if [NULL], element is not comparable
 * @return 1 on successfull. 0 on failure
 */
bool arrayInit(Array *arr, int eSize, int capacity, arrayElemCmp cmp);


/**
 * @brief create a [Array]
 * @param eSize: size of single element
 * @param capacity: initialized capacity 
 * @param cmp: compare function, if [NULL], element is not comparable
 * @return pointor to a [Array] on successful. [NULL] on failure
 */
Array *arrayCreate(int eSize, int capacity, arrayElemCmp cmp);


/**
 * @brief destroy a [Array]
 * @param arr: pointor of [Array]
 * @param freeSelf: free [arr] or not
 */
void arrayDestroy(Array *arr, bool freeSelf);


/**
 * @brief reset search function for a [Array]
 * @param arr: pointor of [Array]
 * @param func: search function for elemnt
 */
#define arraySetSearchFunc(arr, func)                                   \
    do {                                                                \
        Array *_a_arraySetSearchFunc = (Array *)(arr);                  \
        arrayElemSearch _f_arraySetSearchFunc = (func);                 \
        if (_f_arraySetSearchFunc && _a_arraySetSearchFunc->compare) {  \
            _a_arraySetSearchFunc->search = _f_arraySetSearchFunc;      \
        }                                                               \
    } while(0)


/**
 * @brief get current capctity of this [Array]
 * @param arr: pointor of [Array]
 * @return current capacity of this [Array]
 */
#define arrayCapacity(arr) ((arr)->currentCapacity)


/**
 * @brief get array's element size
 * @param arr: pointor of [Array]
 * @return size of element
 */
#define arrayElemSize(arr) ((arr)->elemSize)


/**
 * @brief get current count of element
 * @param arr: pointor of [Array] 
 * @return count of element
 */
#define arrayElemCnt(arr) ((arr)->nextIndex)


/**
 * @brief get the index for a new element
 * @param arr: pointor of [Array]
 * @return index for new element
 */
#define arrayNextIdx(arr) arrayElemCnt(arr)


/**
 * @brief check a index is overflow current element count
 * @param arr: pointor of [Array]
 * @param idx: index need be checked
 * @return 1 when overflow; otherwise 0
 */
#define arrayOverFlow(arr, idx) ({                                                         \
    Array *_a_arrayOverFlow = (arr);                                                       \
    int _idx_arrayOverFlow = (idx);                                                        \
    (_idx_arrayOverFlow) < 0 || (_idx_arrayOverFlow) >= arrayElemCnt(_a_arrayOverFlow);    \
})


/**
 * @brief check a array is empty or not
 * @param arr: pointor of [Array]
 * @return 1 on empty; otherwise 0
 */
#define arrayEmpty(arr) (arrayElemCnt(arr) == 0)


/**
 * @brief check a array is full or not
 * @param arr: pointor of [Array]
 * @return 1 on full; otherwise 0
 */
#define arrayIsFull(arr) ({                                               \
        Array *_a_arrayIsFull = (arr);                                    \
        arrayElemCnt(_a_arrayIsFull) == arrayCapacity(_a_arrayIsFull);    \
    })


/**
 * @brief increment array index
 * @param arr: pointor of [Array]
 */
#define arrayIncIdx(arr) ((arr)->nextIndex++)


/**
 * @brief decrement array index
 * @param arr: pointor of [Array]
 */
#define arrayDecIdx(arr) ((arr)->nextIndex--)


/**
 * @brief get the address of index(not check the index valid or not)
 * @param arr: pointor of [Array]
 * @param idx: index in [Array]
 * @return address of index
 */
#define arrayGetAddr(arr, idx) ({                                                   \
    Array *_a_arrayGetAddr = (Array *)(arr);                                        \
    int _idx_arrayGetAddr = (idx);                                                  \
    ((_a_arrayGetAddr)->data + (_idx_arrayGetAddr) * (_a_arrayGetAddr)->elemSize);  \
})


/**
 * @brief get pointor to the element by index
 * @param arr: pointor of [Array]
 * @param idx: index in [Array]
 * @return pointor to the elment when index not overflow, [NULL] otherwise
 */
#define arrayGetElem(arr, idx) ({                                                                                      \
        Array *_a_arrayGetElem = (arr);                                                                                \
        int _idx_arrayGetElem = (idx);                                                                                 \
        arrayOverFlow(_a_arrayGetElem, _idx_arrayGetElem) ? NULL : arrayGetAddr(_a_arrayGetElem, _idx_arrayGetElem);   \
    })


/**
 * @brief set element on [idx], idx must be valid
 * @param arr: pointor of [Array]
 * @param idx: index of element
 * @param val: pointor to value
 * @return 1 on successfull; 0 on failure
 */
#define arraySetElem(arr, idx, val) ({                                               \
    bool _res_arraySetElem = 0;                                                      \
    Array *_a_arraySetElem = (arr);                                                  \
    int _idx_arraySetElem = (idx);                                                   \
    void *_v_arraySetElem = (void *)(val);                                           \
    void *_p_arraySetElem = arrayGetElem(_a_arraySetElem, _idx_arraySetElem);        \
    if ( _p_arraySetElem ) {                                                         \
        _res_arraySetElem = 1;                                                       \
        memcpy(_p_arraySetElem, _v_arraySetElem, arrayElemSize(_a_arraySetElem));    \
    }                                                                                \
    _res_arraySetElem; })


/**
 * @brief traval [Array]，if handler is not null, call handler in element
 * @param arr: pointor of [Array]
 * @param handler: visit function
*/
#define arrayTraval(arr, handler)                                                                             \
    do {                                                                                                      \
        arrayElemVisit _f_arrayTraval = (handler);                                                            \
        Array *_a_arrayTraval = (arr);                                                                        \
        if(_f_arrayTraval != NULL) {                                                                          \
            for(int _i_arrayTraval = 0; _i_arrayTraval < arrayElemCnt(_a_arrayTraval); ++_i_arrayTraval) {    \
               (_f_arrayTraval)(arrayGetAddr(_a_arrayTraval, _i_arrayTraval));                                \
            }                                                                                                 \
        }                                                                                                     \
    } while(0)


/**
 * @brief check [Array] is ordered or not
 * @param arr: pointor of [Array]
 * @return 1 when ordered; 0 otherwise
 */
#define arrayIsSortedAsc(arr) ({                                                                               \
    bool _res_arrayIsSortedAsc = 0;                                                                            \
    Array *_a_arrayIsSortedAsc = (arr);                                                                        \
    if ( _a_arrayIsSortedAsc->compare ) {                                                                      \
        _res_arrayIsSortedAsc = 1;                                                                             \
        for(int _i_arrayIsSortedAsc = 0;                                                                       \
            _i_arrayIsSortedAsc < arrayElemCnt(_a_arrayIsSortedAsc) - 1; _i_arrayIsSortedAsc++)                \
        {                                                                                                      \
            if ( _a_arrayIsSortedAsc->compare(arrayGetAddr(_a_arrayIsSortedAsc, _i_arrayIsSortedAsc),          \
                                           arrayGetAddr(_a_arrayIsSortedAsc, _i_arrayIsSortedAsc + 1)) > 0)    \
            {                                                                                                  \
                _res_arrayIsSortedAsc = 0;                                                                     \
                break;                                                                                         \
            }                                                                                                  \
        }                                                                                                      \
    }                                                                                                          \
    _res_arrayIsSortedAsc; })

/**
 * @brief check [Array] is ordered or not
 * @param arr: pointor of [Array]
 * @return 1 when ordered; 0 otherwise
 */
#define arrayIsSortedDesc(arr) ({                                                                               \
    bool _res_arrayIsSortedAsc = 0;                                                                            \
    Array *_a_arrayIsSortedAsc = (arr);                                                                        \
    if ( _a_arrayIsSortedAsc->compare ) {                                                                      \
        _res_arrayIsSortedAsc = 1;                                                                             \
        for(int _i_arrayIsSortedAsc = 0;                                                                       \
            _i_arrayIsSortedAsc < arrayElemCnt(_a_arrayIsSortedAsc) - 1; _i_arrayIsSortedAsc++)                \
        {                                                                                                      \
            if ( _a_arrayIsSortedAsc->compare(arrayGetAddr(_a_arrayIsSortedAsc, _i_arrayIsSortedAsc),          \
                                           arrayGetAddr(_a_arrayIsSortedAsc, _i_arrayIsSortedAsc + 1)) < 0)    \
            {                                                                                                  \
                _res_arrayIsSortedAsc = 0;                                                                     \
                break;                                                                                         \
            }                                                                                                  \
        }                                                                                                      \
    }                                                                                                          \
    _res_arrayIsSortedAsc; })


/**
 * @brief swap two element in a [Array]
 * INFO: because using [MALLOC], maybe failed
 * @param arr: pointor of [Array] 
 * @param i index of element
 * @param j index of elemnt
 * @return 1 on successful; 0 on failure
 */
#define arraySwapElemUsingDynamicMem(arr, i, j) ({                                                                              \
    bool _res_arraySwapElemUDM = 0;                                                                                             \
    Array *_a_arraySwapElemUDM = (arr);                                                                                         \
    int _i_arraySwapElemUDM = (i);                                                                                              \
    int _j_arraySwapElemUDM = (j);                                                                                              \
    __attribute__((__cleanup__(freeMem))) void *_tmp_arraySwapElem = MALLOC(sizeof(arrayElemSize(_a_arraySwapElemUDM)));        \
    if (_tmp_arraySwapElem && !arrayOverFlow(_a_arraySwapElemUDM, _i_arraySwapElemUDM)                                          \
             && !arrayOverFlow(_a_arraySwapElemUDM, _j_arraySwapElemUDM))                                                       \
    {                                                                                                                           \
        memcpy(_tmp_arraySwapElem, arrayGetAddr(_a_arraySwapElemUDM, _i_arraySwapElemUDM), arrayElemSize(_a_arraySwapElemUDM)); \
        memcpy(arrayGetAddr(_a_arraySwapElemUDM, _i_arraySwapElemUDM), arrayGetAddr(_a_arraySwapElemUDM, j),                    \
               arrayElemSize(_a_arraySwapElemUDM));                                                                             \
        memcpy(arrayGetAddr(_a_arraySwapElemUDM, _j_arraySwapElemUDM), _tmp_arraySwapElem, arrayElemSize(_a_arraySwapElemUDM)); \
        _res_arraySwapElemUDM = 1;                                                                                              \
    }                                                                                                                           \
    _res_arraySwapElemUDM; })


#define _ARR_FIX_BUFFER_SIZE 4096
/* thread safe */
static inline char *_arrayGetBuffer(void) {
    _Thread_local static char _arrayBuffer[_ARR_FIX_BUFFER_SIZE];
    return _arrayBuffer;
}
/**
 * @brief swap two element in a [Array]
 * INFO: size of elemnt maybe larger than [_ARR_FIX_BUFFER_SIZE]
 * @param arr: pointor of [Array] 
 * @param i index of element
 * @param j index of elemnt
 * @return 1 on successful; 0 on failure
 */
#define arraySwapElemFixedBuffer(arr, i, j) ({                                                                  \
    Array *_a_arraySwapElemUDMFixedBuffer = (arr);                                                              \
    int _i_arraySwapElemUDMFixedBuffer = (i);                                                                   \
    int _j_arraySwapElemUDMFixedBuffer = (j);                                                                   \
    bool _res_arraySwapElemUDMFixedBuffer = 0;                                                                  \
    char *__asefb_buffer = _arrayGetBuffer();                                                                   \
    if( arrayElemSize(_a_arraySwapElemUDMFixedBuffer) <= _ARR_FIX_BUFFER_SIZE                                   \
        && !arrayOverFlow(_a_arraySwapElemUDMFixedBuffer, _i_arraySwapElemUDMFixedBuffer)                       \
        && !arrayOverFlow(_a_arraySwapElemUDMFixedBuffer, _j_arraySwapElemUDMFixedBuffer))                      \
    {                                                                                                           \
        memcpy(__asefb_buffer, arrayGetAddr(_a_arraySwapElemUDMFixedBuffer, _i_arraySwapElemUDMFixedBuffer),    \
               arrayElemSize(_a_arraySwapElemUDMFixedBuffer));                                                  \
        memcpy(arrayGetAddr(_a_arraySwapElemUDMFixedBuffer, _i_arraySwapElemUDMFixedBuffer),                    \
               arrayGetAddr(_a_arraySwapElemUDMFixedBuffer, _j_arraySwapElemUDMFixedBuffer),                    \
               arrayElemSize(_a_arraySwapElemUDMFixedBuffer));                                                  \
        memcpy(arrayGetAddr(_a_arraySwapElemUDMFixedBuffer, _j_arraySwapElemUDMFixedBuffer),                    \
              __asefb_buffer, arrayElemSize(_a_arraySwapElemUDMFixedBuffer));                                   \
        _res_arraySwapElemUDMFixedBuffer = 1;                                                                   \
    }                                                                                                           \
    _res_arraySwapElemUDMFixedBuffer; })

/**
 * @brief swap two element in a [Array]
 * INFO: [i] or [j] maybe overflow
 * @param arr: pointor of [Array] 
 * @param i index of element
 * @param j index of elemnt
 * @return 1 on successful; 0 on failure
 */
#define arraySwapElem(arr, i, j) ({                                                                                     \
    Array *__ase_array = (arr);                                                                                         \
    int __ase_i = (i), __ase_j = (j);                                                                                   \
    bool __ase_res = 0;                                                                                                 \
    if( !arrayOverFlow(__ase_array, __ase_i) && !arrayOverFlow(__ase_array, __ase_j) ) {                                \
        swapMem(arrayGetAddr(__ase_array, __ase_i), arrayGetAddr(__ase_array, __ase_j), arrayElemSize(__ase_array));    \
        __ase_res = 1;                                                                                                  \
    }                                                                                                                   \
    __ase_res;                                                                                                          \
})


/**
 * @brief resize a [Array] capacity
 * @param arr: pointor of [Array] 
 * @param newSize: new capacity
 * @return 1 on successful, 0 on failure
 */
#define arrayResize(arr, newSize) ({                                                                                    \
    Array *_a_arrayResize = (arr);                                                                                      \
    int _newSize_arrayResize = (newSize);                                                                               \
    bool _res_arrayResize = 1;                                                                                          \
    if ( _a_arrayResize->initCapacity <= _newSize_arrayResize                                                           \
        && arrayCapacity(_a_arrayResize) != _newSize_arrayResize )                                                      \
    {                                                                                                                   \
        void *_data_arrayResize = realloc(_a_arrayResize->data, _newSize_arrayResize * arrayElemSize(_a_arrayResize));  \
        if ( _data_arrayResize != NULL ) {                                                                              \
            _a_arrayResize->data = _data_arrayResize;                                                                   \
            _a_arrayResize->currentCapacity = _newSize_arrayResize;                                                     \
        } else {                                                                                                        \
            _res_arrayResize = 0;                                                                                       \
        }                                                                                                               \
    }                                                                                                                   \
    _res_arrayResize; })


/**
 * @brief insert a elemnt at [idx] 
 * @param arr: pointor of [Array] 
 * @param idx: index where to be inserted
 * @param val: pointor to the value of new element
 * @return 1 on successful, 0 on failure
 */
#define arrayInsert(arr, idx, val) ({                                                                                   \
    Array *_a_arrayInsert = (arr);                                                                                      \
    int _idx_arrayInsert = (idx);                                                                                       \
    void *_v_arrayInsert = (void *)(val);                                                                               \
    bool _res_arrayInsert = 0;                                                                                          \
    /* if array is full then extend the capacity */                                                                     \
    _res_arrayInsert = (_idx_arrayInsert) >= 0 && (_idx_arrayInsert) <= arrayNextIdx(_a_arrayInsert)                    \
                       && (!arrayIsFull(_a_arrayInsert)                                                                 \
                           || arrayResize(_a_arrayInsert, arrayCapacity(_a_arrayInsert) * 2));                          \
    if ( _res_arrayInsert ) {                                                                                           \
        arrayIncIdx(_a_arrayInsert);                                                                                    \
        for(int _i_arrayInsert = arrayNextIdx(_a_arrayInsert) - 1; _i_arrayInsert > _idx_arrayInsert; _i_arrayInsert--) \
        {                                                                                                               \
            /* move the data[i] to data[i+1] */                                                                         \
            arraySetElem(_a_arrayInsert, _i_arrayInsert, arrayGetAddr(arr, _i_arrayInsert - 1));                        \
        }                                                                                                               \
        arraySetElem(_a_arrayInsert, _idx_arrayInsert, _v_arrayInsert);                                                 \
    }                                                                                                                   \
    _res_arrayInsert; })


/**
 * @brief search a val in a [Array] (the first one)
 * @param arr: pointor of [Array]  
 * @param val: pointor to the value of element
 * @return index on successful; -1 on no this [val]
 */
#define arraySearch(arr, val) ({                                    \
        Array *_a_arraySearch = (arr);                              \
        void *_v_arraySearch = (void *)(val);                       \
        _a_arraySearch->search(_a_arraySearch, _v_arraySearch); })


/**
 * @brief check a [val] whether in a [Array]
 * @param arr: pointor of [Array]  
 * @param val: pointor to the value of element
 * @return 1 when [val] in [Array]; 0 otherwise
 */
#define arrayContains(arr, val) (arraySearch(arr, val) != -1)


/**
 * @brief delete a element at [idx]
 * @param arr: pointor of [Array]  
 * @param idx: element of index needed be deleted
 * @param retval: address which recieve the element to be deleted. the call ensure that there is enoungh space. 
 * @return 1 on successful, 0 on failure
 */
#define arrayDeleteIdx(arr, idx, retval) ({                                                                             \
    Array *_a_arrayDeleteIdx = (arr);                                                                                   \
    int _idx_arrayDeleteIdx = (idx);                                                                                    \
    void *_v_arrayDeleteIdx = (void *)(retval);                                                                         \
    bool _res_arrayDeleteIdx = 0;                                                                                       \
    if(!arrayOverFlow(_a_arrayDeleteIdx, _idx_arrayDeleteIdx)) {                                                        \
        _res_arrayDeleteIdx = 1;                                                                                        \
        if (_v_arrayDeleteIdx) {                                                                                        \
            memcpy(_v_arrayDeleteIdx, arrayGetElem(_a_arrayDeleteIdx,                                                   \
                   _idx_arrayDeleteIdx), arrayElemSize(_a_arrayDeleteIdx));                                             \
        }                                                                                                               \
        for(int _i_arrayDeleteIdx = _idx_arrayDeleteIdx; _i_arrayDeleteIdx < arrayElemCnt(_a_arrayDeleteIdx) - 1;       \
            _i_arrayDeleteIdx++) {                                                                                      \
            /* move the element to the right postion */                                                                 \
            arraySetElem(_a_arrayDeleteIdx, _i_arrayDeleteIdx, arrayGetAddr(_a_arrayDeleteIdx, _i_arrayDeleteIdx + 1)); \
        }                                                                                                               \
        arrayDecIdx(_a_arrayDeleteIdx);                                                                                 \
        if(arrayElemCnt(_a_arrayDeleteIdx) <= arrayCapacity(_a_arrayDeleteIdx) / 4) {                                   \
            arrayResize(_a_arrayDeleteIdx, arrayCapacity(_a_arrayDeleteIdx) / 2);                                       \
        }                                                                                                               \
    }                                                                                                                   \
    _res_arrayDeleteIdx; })


/**
 * @brief delete a val in a element
 * @param arr: pointor of [Array]  
 * @param val: pointor to the value of element 
 * @return the former index(the first one) of this [val] in the [Array]
 */
#define arrayDeleteVal(arr, val) ({                                                 \
    Array *_a_arrayDeleteVal = (arr);                                               \
    void *_v_arrayDeleteVal = (void *)(val);                                        \
    int _res_arrayDeleteVal = arraySearch(_a_arrayDeleteVal, _v_arrayDeleteVal);    \
    arrayDeleteIdx(_a_arrayDeleteVal, _res_arrayDeleteVal, NULL);                   \
    _res_arrayDeleteVal; })


/**
 * @brief add a element in the tail of [Array]
 * @param arr: pointor of [Array]  
 * @param val: pointor to the value of new element
 * @return 1 on successful, 0 on failure
 */
#define arrayPush(arr, val) ({                                             \
        Array *_a_arrayPush = (arr);                                       \
        void *_v_arrayPush = (void *)(val);                                \
        int _nextIdx_arrayPush = arrayNextIdx(_a_arrayPush);               \
        arrayInsert(_a_arrayPush, _nextIdx_arrayPush, _v_arrayPush); })


/**
 * @brief pop a element in the tail of [Array]
 * @param arr: pointor of [Array]  
 * @param retval: address which recieve the element to be deleted. the call ensure that there is enoungh space.  
 * @return 1 on successful, 0 on failure
 */
#define arrayPop(arr, retval) ({                                                         \
        Array *_a_arrayPop = (arr);                                                      \
        void *_v_arrayPop = (void *)(retval);                                            \
        arrayDeleteIdx(_a_arrayPop, arrayNextIdx(_a_arrayPop) - 1, _v_arrayPop);})


#endif //_ARRAY_H_
