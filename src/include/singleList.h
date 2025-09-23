#ifndef _SINGLE_LIST_H_
#define _SINGLE_LIST_H_

#include "utils.h"

typedef struct sList
{
    struct sList *next;
} SingleList;

typedef void (*singleListFree)(void *);
typedef void (*singleListVisit)(SingleList *);

/**
 * @brief initialize a [SingleList], the memory of [SingleList] managed by caller
 *
 * @param sl: pointor to [SingleList]
 */
#define singleListInit(sl)                         \
    do                                             \
    {                                              \
        SingleList *__sli_sl = (SingleList *)(sl); \
        __sli_sl->next = NULL;                     \
    } while (0)

/**
 * @brief create a [SingleList] on heap.
 *
 * @return pointor to [SingleList] on successful. [NULL] on failure
 */
#define singleListCreate()                                 \
    ({                                                     \
        SingleList *__slc_sl = MALLOC(sizeof(SingleList)); \
        if (__slc_sl)                                      \
        {                                                  \
            singleListInit(__slc_sl);                      \
        }                                                  \
        __slc_sl;                                          \
    })

/**
 * @brief add a new node before the head
 *
 * @param head: head of single list
 * @param newNode: new node need be added into the single list
 *
 * @return new head of this single list
 */
#define singleListAdd(head, newNode)      \
    ({                                    \
        SingleList *__sla_h = (head);     \
        SingleList *__sla_nn = (newNode); \
        __sla_nn->next = __sla_h;         \
        __sla_nn;                         \
    })

/**
 * @brief delete a node(head) from single list
 *
 * @param head: head of single list
 *
 * @return new head of this single list
 */
#define singleListDelete(head)                \
    ({                                        \
        SingleList *__sld_h = (head);         \
        SingleList *__sld_nh = __sld_h->next; \
        __sld_h->next = NULL;                 \
        __sld_nh;                             \
    })

/**
 * @brief concat tow single list
 *
 * @param l1: pointor to list one
 * @param l2: pointor to list two
 *
 * @return new head of this list
 */
#define singleListAppend(l1, l2)                               \
    ({                                                         \
        SingleList *__sla_l1 = (l1);                           \
        SingleList *__sla_l2 = (l2);                           \
        SingleList *__sla_nh = __sla_l1 ? __sla_l1 : __sla_l2; \
        SingleList **__sla_tail;                               \
        if (__sla_l1)                                          \
        {                                                      \
            __sla_tail = &__sla_l1->next;                      \
            while (__sla_l1->next)                             \
            {                                                  \
                __sla_l1 = __sla_l1->next;                     \
                __sla_tail = &__sla_l1->next;                  \
            }                                                  \
            *__sla_tail = __sla_l2;                            \
        }                                                      \
        __sla_nh;                                              \
    })

/**
 * @brief split a single list at n([0, n-1], [n:-1] )
 *
 * @param l: pointor to list
 * @param n: index where to split(index start at [0], but n must > 0)
 *
 * @return new list head if n not overflow, else [NULL]
 */
#define singleListSplit(l, n)                           \
    ({                                                  \
        SingleList *__sls_l = (l), **__sls_next = NULL; \
        int __sls_idx = (n), __sls_ci = 0;              \
        while (__sls_l && __sls_ci < __sls_idx)         \
        {                                               \
            __sls_next = &__sls_l->next;                \
            __sls_ci++;                                 \
            __sls_l = __sls_l->next;                    \
        }                                               \
        if (__sls_l && __sls_next)                      \
        {                                               \
            *__sls_next = NULL;                         \
        }                                               \
        __sls_l;                                        \
    })

#define singleListTraval(l, v)         \
    do                                 \
    {                                  \
        SingleList *__slt_l = (l);     \
        singleListVisit __slt_v = (v); \
        while (__slt_l)                \
        {                              \
            if (__slt_v)               \
            {                          \
                __slt_v(__slt_l);      \
            }                          \
            __slt_l = __slt_l->next;   \
        }                              \
    } while (0)

/**
 * @brief reverse a single List
 *
 * @param list: pointor to single list
 *
 * @return new head pointor after reverse
 */
SingleList *singleListReverse(SingleList *list);

/**
 * @brief destory a single list
 *
 * @param list: pointor to single list
 * @param func: function for freeing SingleList node
 */
void singleListDestory(SingleList *list, singleListFree func);

#endif /* _SINGLE_LIST_H_ */