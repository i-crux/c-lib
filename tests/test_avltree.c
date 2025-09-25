#include <string.h>
#include "test.h"
#include "avlTree.h"
#include "tools.h"

static int bstKeyCmp(const void *a, const void *b)
{
    char *strA = *(char **)a, *strB = *(char **)b;

    return strcmp(strA, strB);
}

static int bstIntKeyCmp(const void *a, const void *b)
{
    int ia = *(int *)a, ib = *(int *)b;
    return ia - ib;
}

static void __deNode(void *a)
{
    DoubleListNode *dl = (DoubleListNode *)a;
    BinTreeNodeData *btnd = containerOf(dl, BinTreeNodeData, dln);

    FREE(*(char **)(btnd->data));
    FREE(btnd);
}

static void __freeKey(void *a)
{
    char *str = *(char **)a;
    FREE(str);
}

static void __visitDlNode(DoubleListNode *dln)
{
    BinTreeNodeData *btnd = (BinTreeNodeData *)dln;
    printf("---> %s ", *((char **)(btnd->data)));
}

static void __visitBstNodeStrKey(BinTreeNode *btn)
{
    printf("%s(%p %p %p %p): ", *((char **)btn->key), btn, btn->parent, btn->left, btn->right);
    travalDoubleList(&btn->dlist, __visitDlNode);
    printf("\n");
}

// static void __vistiIntNode(DoubleListNode *dln) {
//     BinTreeNodeData *btnd = (BinTreeNodeData *)dln;
//     print("---> %d\n", *((int *)(btnd->data)));
// }

static void __visitBstNodeIntKey(BinTreeNode *btn)
{
    printf("%d(%p %p %p %p): ", *((int *)btn->key), btn, btn->parent, btn->left, btn->right);
    travalDoubleList(&btn->dlist, __visitDlNode);
    printf("\n");
}

#define _128M 134217728

static char __outR[_128M];
static char __outNR[_128M];
static char *__orp = __outR, *__onrp = __outNR;

static void __visitBstNodeIntKeyR(BinTreeNode *btn)
{
    sprintf(__orp, "%d(%p %p %p %p): ", *((int *)btn->key), btn, btn->parent, btn->left, btn->right);
    // travalDoubleList(&btn->dlist, __visitDlNode);
    // printf("\n");
    __orp += strlen(__orp);
    sprintf(__orp, "\n");
    __orp += strlen(__orp);
}

static void __visitBstNodeIntKeyNR(BinTreeNode *btn)
{
    sprintf(__onrp, "%d(%p %p %p %p): ", *((int *)btn->key), btn, btn->parent, btn->left, btn->right);
    // travalDoubleList(&btn->dlist, __visitDlNode);
    // printf("\n");
    __onrp += strlen(__onrp);
    sprintf(__onrp, "\n");
    __onrp += strlen(__onrp);
}

static void __preorder(BinTreeNode *btn, binTreeNodeVisit visit)
{
    if (!btn)
    {
        return;
    }
    visit(btn);
    __preorder(btn->left, visit);
    __preorder(btn->right, visit);
}

static void __inorder(BinTreeNode *btn, binTreeNodeVisit visit)
{
    if (!btn)
    {
        return;
    }
    __inorder(btn->left, visit);
    visit(btn);
    __inorder(btn->right, visit);
}

static void __postorder(BinTreeNode *btn, binTreeNodeVisit visit)
{
    if (!btn)
    {
        return;
    }
    __postorder(btn->left, visit);
    __postorder(btn->right, visit);
    visit(btn);
}

static inline intptr_t _avlTreeGetBalanceFactor(AVLTreeNode *avltn)
{
    AVLTreeNode *left, *right;
    intptr_t hl, hr;

    left = avltn->left;
    right = avltn->right;

    hl = left ? left->property : 0;
    hr = right ? right->property : 0;

    return hr - hl;
}

bool avlTreeIsBalanced(AVLTreeNode *root)
{
    if (!root)
        return 1;

    intptr_t bf = _avlTreeGetBalanceFactor(root);

    // 如果当前节点不平衡
    if (bf < -1 || bf > 1)
    {
        return 0;
    }

    // 递归检查左右子树
    return avlTreeIsBalanced(root->left) && avlTreeIsBalanced(root->right);
}

#define __ARR_MAX_SIZE 1024
#define __MAX_INT_KEY __INT_MAX__
#define __ARR_MAX_SIZE_PVP 65535

static inline void test_avlTreeInsert()
{
    _TEST_BEGIN();

    srand(time(NULL));
    BinTree *bt;
    BinTreeNode *btn;
    char *key, *data, *key1;
    int arrSize, intK;

    bt = avlTreeCreate(bstKeyCmp, 1);

    key = genRandomAsciiStr(16);
    assert(key);
    btn = avlTreeNodeCreate(sizeof(char *), &key, __deNode, __freeKey);
    data = genRandomAsciiStr(32);
    assert(data);
    assert(avlTreeNodeAddData(btn, sizeof(char *), &data));
    assert(avlTreeInsert(bt, btn) == 0);

    key1 = MALLOC(17);
    assert(key1);
    memcpy(key1, key, 17);
    btn = avlTreeNodeCreate(sizeof(char *), &key1, __deNode, __freeKey);
    data = genRandomAsciiStr(32);
    assert(data);
    assert(avlTreeNodeAddData(btn, sizeof(char *), &data));

    assert(avlTreeInsert(bt, btn) == 1);
    FREE(btn);

    binTreeTravalPreorder(bt, __visitBstNodeStrKey);
    printf("==========\n");

    key = genRandomAsciiStr(16);
    assert(key);
    btn = avlTreeNodeCreate(sizeof(char *), &key, __deNode, __freeKey);
    data = genRandomAsciiStr(32);
    assert(data);
    assert(avlTreeNodeAddData(btn, sizeof(char *), &data));
    assert(avlTreeInsert(bt, btn) == 0);

    binTreeTravalPreorder(bt, __visitBstNodeStrKey);

    binTreeDestory(bt, 1);

    for (int i = 0; i < 200; i++)
    {

        bt = avlTreeCreate(bstIntKeyCmp, 0);
        arrSize = rand() % __ARR_MAX_SIZE_PVP;
        int btSize = 0;
        printf("arrSize: %d\n", arrSize);
        for (int i = 1; i < arrSize; i++)
        {
            intK = rand() % __MAX_INT_KEY;
            btn = avlTreeNodeCreate(sizeof(char *), &intK, __deNode, NULL);
            data = genRandomAsciiStr(32);
            assert(data);
            assert(avlTreeNodeAddData(btn, sizeof(char *), &data));
            int res = avlTreeInsert(bt, btn);
            assert(res >= 0);
            if (res == 0)
            {
                btSize++;
            }
        }
        assert(btSize == bt->size);

        __orp = __outR;
        __onrp = __outNR;
        binTreeTravalPreorder(bt, __visitBstNodeIntKeyNR);
        __preorder(bt->root, __visitBstNodeIntKeyR);
        assert(strcmp(__outR, __outNR) == 0);

        __orp = __outR;
        __onrp = __outNR;
        binTreeTravalInorder(bt, __visitBstNodeIntKeyNR);
        __inorder(bt->root, __visitBstNodeIntKeyR);
        assert(strcmp(__outR, __outNR) == 0);

        __orp = __outR;
        __onrp = __outNR;
        binTreeTravalPostorder(bt, __visitBstNodeIntKeyNR);
        __postorder(bt->root, __visitBstNodeIntKeyR);
        assert(strcmp(__outR, __outNR) == 0);

        assert(avlTreeIsBalanced(bt->root));

        binTreeDestory(bt, 1);
    }
    _TEST_END();
}

static inline void test_avlTreeSearch()
{
    _TEST_BEGIN();
    srand(time(NULL));

    BinTree *bt;
    BinTreeNode *btn;
    int arrSize, keyArr[__ARR_MAX_SIZE * 2];

    bt = avlTreeCreate(bstIntKeyCmp, 0);
    assert(bt);
    assert(bt->cmp == bstIntKeyCmp);
    assert(bt->root == NULL);
    assert(bt->allowSameKey == 0);
    assert(bt->size == 0);

    arrSize = rand() % __ARR_MAX_SIZE;
    printf("arrSize: %d\n", arrSize);
    int btSize = 0;
    for (int i = 0; i < arrSize; i++)
    {
        keyArr[i] = rand() % __MAX_INT_KEY;
        btn = avlTreeNodeCreate(sizeof(char *), keyArr + i, FREE, NULL);
        int res = avlTreeInsert(bt, btn);
        assert(res >= 0);
        if (res == 0)
        {
            btSize++;
        }
    }
    assert(bt->size == btSize);
    for (int i = arrSize; i < arrSize * 2; i++)
    {
        keyArr[i] = rand() % __MAX_INT_KEY;
    }
    printf("keyArr: ");
    for (int i = 0; i < arrSize * 2 - 1; i++)
    {
        printf("%d(%d), ", keyArr[i], i);

        btn = avlTreeSearch(bt, keyArr + i);
        if (i < arrSize)
        {
            assert(*((int *)(btn->key)) == keyArr[i]);
        }
        else
        {
            assert(btn == NULL);
        }
    }
    printf("\n");

    binTreeDestory(bt, 1);

    _TEST_END();
}

static inline void test_avlTreeDelete()
{
    _TEST_BEGIN();
    srand(time(NULL));

    BinTree *bt;
    BinTreeNode *btn;
    int arrSize, keyArr[__ARR_MAX_SIZE * 2];

    bt = avlTreeCreate(bstIntKeyCmp, 0);

    arrSize = rand() % __ARR_MAX_SIZE;
    printf("arrSize: %d\n", arrSize);
    int btSize = 0;
    for (int i = 0; i < arrSize; i++)
    {
        keyArr[i] = rand() % __MAX_INT_KEY;
        btn = avlTreeNodeCreate(sizeof(char *), keyArr + i, FREE, NULL);
        int res = avlTreeInsert(bt, btn);
        assert(res >= 0);
        if (res == 0)
        {
            btSize++;
        }
        assert(avlTreeIsBalanced(bt->root));
    }

    assert(bt->size == btSize);
    // binTreeTravalInorder(bt, __visitBstNodeIntKey);
    // printf("==================\n");
    for (int i = arrSize; i < arrSize * 2; i++)
    {
        keyArr[i] = rand() % __MAX_INT_KEY;
    }
    printf("keyArr: ");
    for (int i = 0; i < arrSize * 2 - 1; i++)
    {
        printf("%d(%d), ", keyArr[i], i);

        btn = avlTreeDelete(bt, keyArr + i);
        assert(avlTreeIsBalanced(bt->root));
        if (i < arrSize)
        {
            assert(btn);
            assert(*((int *)(btn->key)) == keyArr[i]);
            assert(bt->size == --btSize);
            FREE(btn);
            // binTreeTravalInorder(bt, __visitBstNodeIntKey);
            // printf("==================\n");
        }
        else
        {
            assert(btn == NULL);
        }
    }
    printf("\n");

    arrSize = rand() % __ARR_MAX_SIZE;
    printf("arrSize: %d\n", arrSize);
    btSize = 0;
    for (int i = 0; i < arrSize; i++)
    {
        keyArr[i] = rand() % __MAX_INT_KEY;
        btn = avlTreeNodeCreate(sizeof(char *), keyArr + i, FREE, NULL);
        int res = avlTreeInsert(bt, btn);
        assert(avlTreeIsBalanced(bt->root));
        assert(res >= 0);
        if (res == 0)
        {
            btSize++;
        }
    }

    for (int i = 0; i < 10000 && btSize > 0; i++)
    {
        int idx = rand() % arrSize;
        btn = avlTreeDelete(bt, keyArr + idx);
        assert(avlTreeIsBalanced(bt->root));
        binTreeTravalInorder(bt, __visitBstNodeIntKey);
        printf("---------------------\n");
        binTreeTravalInorder(bt, __visitBstNodeIntKey);
        printf("---------------------\n");
        binTreeTravalInorder(bt, __visitBstNodeIntKey);
        printf("---------------------\n");
        if (btn)
        {
            assert(bt->size == --btSize);
            FREE(btn);
        }
    }
    printf("############# btSize: %d arrSize: %d  ###################\n", btSize, arrSize);

    binTreeDestory(bt, 1);
    _TEST_END();
}

#if 0
static inline void test_bstRotate()
{
    _TEST_BEGIN();

    BinTree *bt = binTreeCreate(bstIntKeyCmp, 0);
    BinTreeNode *btn;
    int i;

    i = 1;
    btn = binTreeNodeCreate(0, sizeof(int), &i, FREE, NULL);
    bstInsert(bt, btn);
    i = 2;
    btn = binTreeNodeCreate(0, sizeof(int), &i, FREE, NULL);
    bstInsert(bt, btn);
    i = 3;
    btn = binTreeNodeCreate(0, sizeof(int), &i, FREE, NULL);
    bstInsert(bt, btn);

    binTreeTravalInorder(bt, __visitBstNodeIntKey);
    printf("---------------------\n");
    binTreeLeftRotate(&bt->root, bt->root);
    binTreeTravalInorder(bt, __visitBstNodeIntKey);
    printf("---------------------\n");
    binTreeLeftRotate(&bt->root, bt->root);
    binTreeTravalInorder(bt, __visitBstNodeIntKey);
    printf("---------------------\n");

    binTreeRightRotate(&bt->root, bt->root);
    binTreeTravalInorder(bt, __visitBstNodeIntKey);
    printf("---------------------\n");

    binTreeRightRotate(&bt->root, bt->root);
    binTreeTravalInorder(bt, __visitBstNodeIntKey);
    printf("---------------------\n");

    binTreeLeftRotate(&bt->root->right, bt->root->right);
    binTreeTravalInorder(bt, __visitBstNodeIntKey);
    printf("---------------------\n");

    binTreeRightRotate(&bt->root->right, bt->root->right);
    binTreeTravalInorder(bt, __visitBstNodeIntKey);
    printf("---------------------\n");

    binTreeDestory(bt, 1);

    _TEST_END();
}
#endif
int main(void)
{

    test_avlTreeInsert();
    test_avlTreeSearch();
    test_avlTreeDelete();
    return 0;
}