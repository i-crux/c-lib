#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "array.h"

#define _TEST_BEGIN()                                                        \
    do                                                                       \
    {                                                                        \
        printf("\033[34mtest: %s ---------> begin.\033[0m\n", &__func__[5]); \
    } while (0)

#define _TEST_END()                                                             \
    do                                                                          \
    {                                                                           \
        printf("\033[32mtest: %s ---------> passed.\033[0m\n\n", &__func__[5]); \
    } while (0)

static int __test_arr_cmpInt(const void *a, const void *b)
{
    int intA = *(int *)a, intB = *(int *)b;
    return intA - intB;
}

static __attribute__((unused)) Array *genIntArray(const int length, const bool ordered)
{
    Array *arr;
    int randInt;

    if ((arr = arrayCreate(sizeof(int), length, __test_arr_cmpInt)) == NULL)
    {
        goto _done;
    }

    for (int i = 0; i < length; i++)
    {
        if (ordered)
        {
            arrayPush(arr, &i);
        }
        else
        {
            randInt = rand() % length;
            arrayPush(arr, &randInt);
        }
    }

_done:
    return arr;
}

static __attribute__((unused)) void printIntArray(const int *arr, const size_t len)
{
    printf("[ ");
    for (size_t i = 0; i < len; i++)
    {
        i != len - 1 ? printf("%d, ", arr[i]) : printf("%d", arr[i]);
    }
    printf(" ]\n");
}

#define _measureBegin()              \
    do                               \
    {                                \
        struct timeval _start, _end; \
    gettimeofday(&_start, NULL)

#define _measureEnd(algorithmName, sampleSize)                                                        \
    gettimeofday(&_end, NULL);                                                                        \
    double _time_taken = (_end.tv_sec - _start.tv_sec) + (_end.tv_usec - _start.tv_usec) / 1000000.0; \
    printf(#algorithmName "\t| sample size: %11d, run time: %7f s\n", (sampleSize), _time_taken);     \
    }                                                                                                 \
    while (0)

#endif /* _TEST_H_ */