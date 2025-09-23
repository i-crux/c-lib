#ifndef _DOUBLE_LIST_H_
#define _DOUBLE_LIST_H_

#include "utils.h"

typedef struct doubleList DoubleList;
typedef struct doubleListNode DoubleListNode;
typedef void (*doubleListNodeFree)(void *node);
typedef void (*doubleListNodeVisitFunc)(DoubleListNode *node);

/*
 * structure of double list node
 */
struct doubleListNode
{
    DoubleListNode *prev;   /* pointor to prev node */
    DoubleListNode *next;   /* pointor to next node */
    DoubleList *doubleList; /* pointor to doubleList structure */
};

/*
 * structure of double list
 */
struct doubleList
{
    DoubleListNode header;     /* dummy header of double list */
    doubleListNodeFree deNode; /* free memory of node */
    int nodeCnt;               /* count of all nodes */
};

/**
 * @brief initialize double list node.
 * INFO: memory of double list node already alloced in stack or heap
 *
 * @param doubleListNodePtr: pointor of double list node
 */
#define initDoubleListNode(doubleListNodePtr)                \
    do                                                       \
    {                                                        \
        DoubleListNode *__idln_dlnp = (doubleListNodePtr);   \
        /* [prev] and [next] point to self */                \
        __idln_dlnp->prev = __idln_dlnp->next = __idln_dlnp; \
        __idln_dlnp->doubleList = NULL;                      \
    } while (0)

/**
 * @brief create a double list node in heap memory
 *
 * @return Pointer to the node on seccess, or [NULL] on failure.
 */
#define createDoubleListNode()                                        \
    ({                                                                \
        DoubleListNode *__cdln_dlnp = MALLOC(sizeof(DoubleListNode)); \
        if (__cdln_dlnp)                                              \
        {                                                             \
            initDoubleListNode(__cdln_dlnp);                          \
        }                                                             \
        __cdln_dlnp;                                                  \
    })

/**
 * @brief initialize a double list
 * INFO: memory of double list structure already alloced in stack or heap
 *
 * @param doubleListPtr: pointor to double list structure
 * @param deNodeFunc: pointor to free node function. set to NULL when destory the double list,
 *                    the node will not be free
 */
#define initDoubleList(doubleListPtr, deNodeFunc)    \
    do                                               \
    {                                                \
        DoubleList *__idl_dlp = (doubleListPtr);     \
        doubleListNodeFree __idl_dnf = (deNodeFunc); \
        /* initialize dummy header */                \
        initDoubleListNode(&__idl_dlp->header);      \
        __idl_dlp->header.doubleList = __idl_dlp;    \
        __idl_dlp->deNode = __idl_dnf;               \
        __idl_dlp->nodeCnt = 0;                      \
    } while (0)

/**
 * @brief create a double list on heap memory
 *
 * @param deNodeFunc: pointor to free node function. set to NULL when destory the double list,
 *                    the node will not be free
 *
 * @return Pointer to the double list structure on seccess, or [NULL] on failure.
 */
#define createDoubleList(deNodeFunc)                        \
    ({                                                      \
        DoubleList *__cdl_dlp = MALLOC(sizeof(DoubleList)); \
        if (__cdl_dlp)                                      \
        {                                                   \
            initDoubleList(__cdl_dlp, deNodeFunc);          \
        }                                                   \
        __cdl_dlp;                                          \
    })

/**
 * @brief check a double list node in a double list or not
 *
 * @param doubleListNodePtr: pointor of double list node
 *
 * @return [true] when the node in a double list, [false] otherwise
 */
#define checkDoubListNode(doubleListNodePtr)                \
    ({                                                      \
        DoubleListNode *__ckdln_dlnp = (doubleListNodePtr); \
        DoubleList *__ckdln_dlp = __ckdln_dlnp->doubleList; \
        __ckdln_dlp != NULL;                                \
    })

/**
 * @brief check whether a node is dummy header node
 *
 * @param doubleListNodePtr: pointor of double list node
 *
 * @return [true] when [doubleListNodePtr] is dummy header, or [false] on not
 */
#define isDoubleListSentinel(doubleListNodePtr)               \
    ({                                                        \
        DoubleListNode *__idls_dlnp = (doubleListNodePtr);    \
        bool __idls_ret = checkDoubListNode(__idls_dlnp);     \
        if (__idls_ret)                                       \
        {                                                     \
            DoubleList *__idls_dlp = __idls_dlnp->doubleList; \
            __idls_ret = __idls_dlnp == &__idls_dlp->header;  \
        }                                                     \
        __idls_ret;                                           \
    })

/**
 * @brief add a new node [doubleListNodeNewPtr] after [doubleListNodePtr]
 *
 * @param doubleListNodePtr: the node where to add after
 * @param doubleListNodeNewPtr: the node which to be added
 */
#define addDoubleListNodeAfter(doubleListNodePtr, doubleListNodeNewPtr)            \
    do                                                                             \
    {                                                                              \
        DoubleListNode *__adlna_dlnp = (doubleListNodePtr);                        \
        DoubleListNode *__adlna_dlnp_n = (doubleListNodeNewPtr);                   \
        if (checkDoubListNode(__adlna_dlnp) && !checkDoubListNode(__adlna_dlnp_n)) \
        {                                                                          \
            /* add __adlna_dlnp_n after __adlna_dlnp */                            \
            __adlna_dlnp_n->prev = __adlna_dlnp;                                   \
            __adlna_dlnp_n->next = __adlna_dlnp->next;                             \
            __adlna_dlnp_n->next->prev = __adlna_dlnp_n;                           \
            __adlna_dlnp_n->prev->next = __adlna_dlnp_n;                           \
            __adlna_dlnp_n->doubleList = __adlna_dlnp->doubleList;                 \
            __adlna_dlnp_n->doubleList->nodeCnt++;                                 \
        }                                                                          \
    } while (0)

/**
 * @brief remove a node from a double list. node must be in a double list.
 *        and node is not dummy header
 *
 * @param doubleListNodePtr: pointor to double list node
 */
#define removeDoubleListNode(doubleListNodePtr)                                   \
    do                                                                            \
    {                                                                             \
        DoubleListNode *__rdln_dlnp = (doubleListNodePtr);                        \
        DoubleList *__rdln_dlp = __rdln_dlnp->doubleList;                         \
        /* comfirm node is in a double list and node is not dummy header */       \
        if (checkDoubListNode(__rdln_dlnp) && !isDoubleListSentinel(__rdln_dlnp)) \
        {                                                                         \
            /* remove the node from double list */                                \
            __rdln_dlnp->prev->next = __rdln_dlnp->next;                          \
            __rdln_dlnp->next->prev = __rdln_dlnp->prev;                          \
            __rdln_dlp->nodeCnt--;                                                \
            initDoubleListNode(__rdln_dlnp);                                      \
        }                                                                         \
    } while (0)

/**
 * @brief insert a node at double list header
 *
 * @param doubleListPtr: a pointor to double list structure
 * @param doubleListNodeNewPtr: a pointor to double list new node
 */
#define addNodeAtDoubleListHeader(doubleListPtr, doubleListNodeNewPtr)       \
    do                                                                       \
    {                                                                        \
        DoubleList *__anadlh_dlp = (doubleListPtr);                          \
        addDoubleListNodeAfter(&__anadlh_dlp->header, doubleListNodeNewPtr); \
    } while (0)

/**
 * @brief insert a node at double list tail
 *
 * @param doubleListPtr: a pointor to double list structure
 * @param doubleListNodeNewPtr: a pointor to double list new node
 */
#define addNodeAtDoubleListTail(doubleListPtr, doubleListNodeNewPtr)             \
    do                                                                           \
    {                                                                            \
        DoubleList *__anadlt_dlp = (doubleListPtr);                              \
        addDoubleListNodeAfter(__anadlt_dlp->header.prev, doubleListNodeNewPtr); \
    } while (0)

/**
 * @brief: traval a double list
 *
 * @param doubleListPtr: pointor of a Double list
 * @param func: visit function
 */
#define travalDoubleList(doubleListPtr, func)                                 \
    do                                                                        \
    {                                                                         \
        DoubleList *__tdl_dlp = (DoubleList *)(doubleListPtr);                \
        doubleListNodeVisitFunc __tdl_func = (doubleListNodeVisitFunc)(func); \
        DoubleListNode *__tdl_dlnp = __tdl_dlp->header.next;                  \
        while (!isDoubleListSentinel(__tdl_dlnp))                             \
        {                                                                     \
            __tdl_func(__tdl_dlnp);                                           \
            __tdl_dlnp = __tdl_dlnp->next;                                    \
        }                                                                     \
    } while (0)

/**
 * @brief destory a double list, if [doubleList->deNode] != NULL, call [deNode]
 * WARNING: if free(doubleListPtr),  [doubleListPtr] will NOT set to NULL
 *
 * @param dl: pointor to a double list
 * @param freeSelf: if true, free DoubleList Structure
 */
#define destoryDoubleList(doubleListPtr, freeDoubleList)     \
    do                                                       \
    {                                                        \
        DoubleList *__ddl_dlp = (doubleListPtr);             \
        doubleListNodeFree __ddl_dnf = __ddl_dlp->deNode;    \
        DoubleListNode *__ddl_dlpn = __ddl_dlp->header.next; \
        while (!isDoubleListSentinel(__ddl_dlpn))            \
        {                                                    \
            removeDoubleListNode(__ddl_dlpn);                \
            if (__ddl_dnf)                                   \
            {                                                \
                __ddl_dnf(__ddl_dlpn);                       \
            }                                                \
            __ddl_dlpn = __ddl_dlp->header.next;             \
        }                                                    \
        if (freeDoubleList)                                  \
        {                                                    \
            FREE(__ddl_dlp);                                 \
        }                                                    \
    } while (0)

/*
void destoryDoubleList(DoubleList *dl, bool freeSelf);

void destoryDoubleList(DoubleList *dl, bool freeSelf) {
    DoubleListNode *dln = dl->header.next;
    while(!isDoubleListSentinel(dln)) {
        removeDoubleListNode(dln);
        if(dl->deNode) {
            dl->deNode(dln);
        }
        dln = dl->header.next;
    }
    if(freeSelf) {
        FREE(dl);
    }
}
*/

#endif /* _DOUBLE_LIST_H_ */