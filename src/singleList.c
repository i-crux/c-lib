#include "singleList.h"

SingleList *singleListReverse(SingleList *list)
{
    SingleList *cur = list, *res = list;

    if (list && list->next)
    {
        cur = singleListReverse(list->next);
        list->next->next = list;
        list->next = NULL;
        res = cur;
    }

    return res;
}

void singleListDestory(SingleList *list, singleListFree func)
{
    if (list)
    {
        singleListDestory(list->next, func);
        list->next = NULL;
        if (func)
        {
            func(list);
        }
    }
}