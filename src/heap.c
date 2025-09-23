#include "heap.h"

void heapSiftDown(Heap *hp, int idx)
{
    int leftChildIdx, rightChildIdx, xchgIdx, cnt;

    if (!hp || !hp->compare || idx < 0 || idx >= arrayElemCnt(hp))
    {
        /* arguments unvaliable */
        goto _done;
    }

    cnt = arrayElemCnt(hp);
    leftChildIdx = heapLeftChildIdx(idx);

    while (leftChildIdx < cnt)
    {
        rightChildIdx = heapRightChildIdx(idx);
        if (rightChildIdx < cnt)
        {
            xchgIdx = (hp->compare(arrayGetElem(hp, leftChildIdx), arrayGetElem(hp, rightChildIdx)) >= 0 ? leftChildIdx : rightChildIdx);
        }
        else
        {
            xchgIdx = leftChildIdx;
        }

        if (hp->compare(arrayGetElem(hp, idx), arrayGetElem(hp, xchgIdx)) < 0)
        {
            arraySwapElem(hp, idx, xchgIdx);
        }
        else
        {
            goto _done;
        }

        idx = xchgIdx;
        leftChildIdx = heapLeftChildIdx(idx);
    }

_done:
    return;
}

void heapSiftUp(Heap *hp, int idx)
{
    int parentIdx;

    if (!hp || !hp->compare || idx < 0 || idx >= arrayElemCnt(hp))
    {
        /* arguments unvaliable */
        goto _done;
    }

    parentIdx = heapParentIdx(idx);

    while (parentIdx >= 0 &&
           hp->compare(arrayGetElem(hp, idx), arrayGetElem(hp, parentIdx)) > 0)
    {
        arraySwapElem(hp, idx, parentIdx);
        idx = parentIdx;
        parentIdx = heapParentIdx(idx);
    }

_done:
    return;
}

void heapExtract(Heap *hp, void *retval)
{
    int lastIdx;

    if (!hp || !hp->compare || !retval)
    {
        /* arguments unvaliable */
        goto _done;
    }

    lastIdx = arrayNextIdx(hp) - 1;

    arraySwapElem(hp, 0, lastIdx);

    arrayPop(hp, retval);

    heapSiftDown(hp, 0);

_done:
    return;
}

bool heapAdd(Heap *hp, void *value)
{
    bool ret = 0;

    if (!hp || !hp->compare || !value)
    {
        /* arguments unvaliable */
        goto _done;
    }

    arrayPush(hp, value);
    heapSiftUp(hp, arrayElemCnt(hp) - 1);
    ret = 1;

_done:
    return ret;
}

void heapRaplace(Heap *hp, void *ep, void *retval)
{

    if (!hp || !hp->compare || !ep || !retval)
    {
        /* arguments unvaliable */
        goto _done;
    }

    memcpy(retval, arrayGetElem(hp, 0), arrayElemSize(hp));
    arraySetElem(hp, 0, ep);
    heapSiftDown(hp, 0);

_done:
    return;
}

void heapHeapify(Heap *hp)
{
    int lastIdx;

    if (!hp || !hp->compare)
    {
        /* arguments unvaliable */
        goto _done;
    }

    lastIdx = heapParentIdx(arrayElemCnt(hp) - 1);
    for (int i = lastIdx; i >= 0; i--)
    {
        heapSiftDown(hp, i);
    }
_done:
    return;
}