#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "test.h"
#include "bst.h"
#include "avlTree.h"
#include "rbTree.h"
#include "hash.h"

#define N 100000
#define M 50000

static int bstIntKeyCmp(const void *a, const void *b)
{
    int ia = *(int *)a, ib = *(int *)b;
    return ia - ib;
}

static int __intKeyCmp(uintptr_t a, uintptr_t b)
{
    int aInt = (int)a, bInt = (int)b;

    return aInt - bInt;
}
static void _dlnFreeIntKeyStrData(void *n)
{
    DoubleListNode *dln = n;
    HashNode *hn = containerOf(dln, HashNode, dln);
    FREE(hn);
}

static inline void test_compareBstAVLRBTreeHash()
{
    _TEST_BEGIN();
    int *arr = MALLOC(sizeof(int) * N);
    int i;

    srand((unsigned int)time(NULL));
    for (i = 0; i < N; i++)
        arr[i] = rand();

    // ----------------- test BST -----------------
    BinTree *bst = binTreeCreate(bstIntKeyCmp, 0);
    BinTreeNode *bstn;

    clock_t t1 = clock();
    for (i = 0; i < N; i++)
    {
        bstn = binTreeNodeCreate(0, sizeof(int), &i, FREE, NULL);
        assert(bstInsert(bst, bstn) == 0);
    }
    clock_t t2 = clock();
    printf("BST Insert %d elements: %f sec\n", N, (double)(t2 - t1) / CLOCKS_PER_SEC);

    t1 = clock();
    for (i = 0; i < M; i++)
        bstSearch(bst, &arr[rand() % N]);
    t2 = clock();
    printf("BST Search %d elements: %f sec\n", M, (double)(t2 - t1) / CLOCKS_PER_SEC);

    t1 = clock();
    for (i = 0; i < M; i++)
        bstDelete(bst, &arr[rand() % N]);
    t2 = clock();
    printf("BST Delete %d elements: %f sec\n", M, (double)(t2 - t1) / CLOCKS_PER_SEC);
    binTreeDestory(bst, 1);

    // ----------------- test AVL -----------------
    AVLTree *avlt = avlTreeCreate(bstIntKeyCmp, 0);
    AVLTreeNode *avltn;

    t1 = clock();
    for (i = 0; i < N; i++)
    {
        avltn = avlTreeNodeCreate(sizeof(int), &i, FREE, NULL);
        assert(avlTreeInsert(avlt, avltn) == 0);
    }
    t2 = clock();
    printf("AVL Insert %d elements: %f sec\n", N, (double)(t2 - t1) / CLOCKS_PER_SEC);

    t1 = clock();
    for (i = 0; i < M; i++)
        avlTreeSearch(avlt, &arr[rand() % N]);
    t2 = clock();
    printf("AVL Search %d elements: %f sec\n", M, (double)(t2 - t1) / CLOCKS_PER_SEC);

    t1 = clock();
    for (i = 0; i < M; i++)
        avlTreeDelete(avlt, &arr[rand() % N]);
    t2 = clock();
    printf("AVL Delete %d elements: %f sec\n", M, (double)(t2 - t1) / CLOCKS_PER_SEC);
    binTreeDestory(avlt, 1);

    // ----------------- test RB -----------------
    RBTree *rbt = rbTreeCreate(bstIntKeyCmp, 0);
    RBTreeNode *rbtn;

    t1 = clock();
    for (i = 0; i < N; i++)
    {
        rbtn = rbTreeNodeCreate(sizeof(int), &i, FREE, NULL);
        assert(rbTreeInsert(rbt, rbtn) == 0);
    }
    t2 = clock();
    printf("RB tree Insert %d elements: %f sec\n", N, (double)(t2 - t1) / CLOCKS_PER_SEC);

    t1 = clock();
    for (i = 0; i < M; i++)
        rbTreeSearch(rbt, &arr[rand() % N]);
    t2 = clock();
    printf("RB tree Search %d elements: %f sec\n", M, (double)(t2 - t1) / CLOCKS_PER_SEC);

    t1 = clock();
    for (i = 0; i < M; i++)
        rbTreeDelete(rbt, &arr[rand() % N]);
    t2 = clock();
    printf("RB tree Delete %d elements: %f sec\n", M, (double)(t2 - t1) / CLOCKS_PER_SEC);
    binTreeDestory(rbt, 1);

    // ----------------- test hash -----------------
    Hash *h;
    h = hashCreate(defaultHashNum, __intKeyCmp, 49157, _dlnFreeIntKeyStrData);
    t1 = clock();
    for (i = 0; i < N; i++)
    {
        assert(hashAdd(h, (uintptr_t)i, 0, NULL) == 0);
    }
    t2 = clock();
    printf("hash Insert %d elements: %f sec\n", N, (double)(t2 - t1) / CLOCKS_PER_SEC);

    t1 = clock();
    for (i = 0; i < M; i++)
        hashSearch(h, (uintptr_t)(((uintptr_t)rand()) % N));
    t2 = clock();
    printf("hash Search %d elements: %f sec\n", M, (double)(t2 - t1) / CLOCKS_PER_SEC);

    t1 = clock();
    for (i = 0; i < M; i++)
        hashSearch(h, (uintptr_t)(((uintptr_t)rand()) % N));
    t2 = clock();
    printf("hash Delete %d elements: %f sec\n", M, (double)(t2 - t1) / CLOCKS_PER_SEC);
    hashDestory(h);

    FREE(arr);

    _TEST_END();
}

int main(void)
{
    test_compareBstAVLRBTreeHash();
    return 0;
}
