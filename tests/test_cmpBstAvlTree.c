#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "test.h"
#include "bst.h"     // 你的 BST 接口
#include "avlTree.h" // 你的 AVL 接口

#define N 100000
#define M 50000

static int bstIntKeyCmp(const void *a, const void *b)
{
    int ia = *(int *)a, ib = *(int *)b;
    return ia - ib;
}

static inline void test_compareBstAVLTree()
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

    FREE(arr);

    _TEST_END();
}

int main(void)
{
    test_compareBstAVLTree();
    return 0;
}
