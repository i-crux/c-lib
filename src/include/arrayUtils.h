#ifndef _ARRAY_UTILS_H_
#define _ARRAY_UTILS_H_

#include "array.h"


#define ARRAY_INSERTION_SORT_SIZE 10

/**
 * @brief sort [Array] from [begin, end] using insertion
 * 
 * @param arr: pointor to [Array]
 * @param begin: begin index
 * @param end: end index
 */
void arrayInsertionSortIdx(Array *arr, const int begin, const int end);

/**
 * @brief sort a [Array] using insertion
 * @param arr: pointor to [Array]
 */
void arrayInsertionSort(Array *arr);


/**
 * @brief sort a [Array] using selection
 * @param arr: pointor to [Array]
 */
void arraySelectionSort(Array *arr);


/**
 * @brief sort a [Array] using selection
 * @param arr: pointor to [Array]
 */
void arrayMergeSort(Array *arr);


/**
 * @brief sort a [Array] using quick sort
 * @param arr: pointor to [Array]
 */
void arrayQuickSort(Array *arr);


/**
 * @brief sort a [Array] using two way quick sort
 * @param arr: pointor to [Array]
 */
void arrayQuickSort2way(Array *arr);


/**
 * @brief sort a [Array] using three way quick sort
 * @param arr: pointor to [Array]
 */
void arrayQuickSort3way(Array *arr);


/**
 * @brief get the kth element of Array
 * @param arr: pointor to [Array]
 * @param k: kth
 * @return the kth address of elements if [k] not overflow else [NULL]
 */
void *arraySelectKthElemet(Array *arr, int k);



/**
 * @brief search a sorted [Array] using binary search
 * IMPORTANT: arr->compare is not [NULL] and arr has already been sorted
 * @param arr: pointor to [Array]
 * @param key: pointor to key
 * @return the pointor to the [Array] element if [key] exsits, otherwise [NULL] 
 */
void *arrayBinSearch(Array *arr, void *key);

#endif  /* _ARRAY_UTILS_H_ */