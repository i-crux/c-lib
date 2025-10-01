#include <string.h>
#include "test.h"
#include "rbTree.h"
#include "tools.h"

// check root is black
static inline bool _check_root_black(RBTree *rbt)
{
    return rbtnColor(rbt->root) == RBT_BLACK;
}

// check no red-red connection
static inline bool _check_no_red_red(RBTreeNode *node)
{
    if (!node)
        return 1;
    if (rbtnColor(node) == RBT_RED)
    {
        if (rbtnColor(node->left) == RBT_RED || rbtnColor(node->right) == RBT_RED)
        {
            return 0;
        }
    }
    return _check_no_red_red(node->left) && _check_no_red_red(node->right);
}

// check heigth of black
static inline int _check_black_height(RBTreeNode *node)
{
    if (!node)
    {
        return 1;
    }

    int left = _check_black_height(node->left);
    int right = _check_black_height(node->right);
    if (left != right)
        printf("left = %d, %p, right = %d, %p\n", left, node->left, right, node->right);

    if (left == -1 || right == -1 || left != right)
    {
        return -1;
    }

    return left + (rbtnColor(node) == RBT_BLACK ? 1 : 0);
}

static inline bool _validate_rbtree(RBTree *rbt)
{
    if (!_check_root_black(rbt))
    {
        printf("root is not [RBT_BLACK].\n");
        return 0;
    }
    if (!_check_no_red_red(rbt->root))
    {
        printf("red -- red is exists.\n");
        return 0;
    }
    if (_check_black_height(rbt->root) == -1)
    {
        printf("heigth of [RBT_BLACK] is not equal.\n");
        return 0;
    }
    return 1;
}

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

#define __ARR_MAX_SIZE 1024
#define __MAX_INT_KEY __INT_MAX__
#define __ARR_MAX_SIZE_PVP 65535

static inline void test_rbTreeInsert()
{
    _TEST_BEGIN();

    srand(time(NULL));
    BinTree *bt;
    BinTreeNode *btn;
    char *key, *data, *key1;
    int arrSize, intK;

    bt = rbTreeCreate(bstKeyCmp, 1);

    key = genRandomAsciiStr(16);
    assert(key);
    btn = rbTreeNodeCreate(sizeof(char *), &key, __deNode, __freeKey);
    data = genRandomAsciiStr(32);
    assert(data);
    assert(rbTreeNodeAddData(btn, sizeof(char *), &data));
    printf("fuck\n");
    fflush(stdout);
    assert(rbTreeInsert(bt, btn) == 0);

    key1 = MALLOC(17);
    assert(key1);
    memcpy(key1, key, 17);
    btn = rbTreeNodeCreate(sizeof(char *), &key1, __deNode, __freeKey);
    data = genRandomAsciiStr(32);
    assert(data);
    assert(rbTreeNodeAddData(btn, sizeof(char *), &data));

    assert(rbTreeInsert(bt, btn) == 1);
    FREE(btn);

    binTreeTravalPreorder(bt, __visitBstNodeStrKey);
    printf("==========\n");

    key = genRandomAsciiStr(16);
    assert(key);
    btn = rbTreeNodeCreate(sizeof(char *), &key, __deNode, __freeKey);
    data = genRandomAsciiStr(32);
    assert(data);
    assert(rbTreeNodeAddData(btn, sizeof(char *), &data));
    assert(rbTreeInsert(bt, btn) == 0);

    binTreeTravalPreorder(bt, __visitBstNodeStrKey);

    binTreeDestory(bt, 1);

    for (int i = 0; i < 10; i++)
    {

        bt = rbTreeCreate(bstIntKeyCmp, 0);
        arrSize = rand() % __ARR_MAX_SIZE_PVP;
        int btSize = 0;
        printf("arrSize: %d\n", arrSize);
        for (int i = 1; i < arrSize; i++)
        {
            intK = rand() % __MAX_INT_KEY;
            btn = rbTreeNodeCreate(sizeof(char *), &intK, __deNode, NULL);
            data = genRandomAsciiStr(32);
            assert(data);
            assert(rbTreeNodeAddData(btn, sizeof(char *), &data));
            int res = rbTreeInsert(bt, btn);

            if (!_validate_rbtree(bt))
            {
                printf("i = %d\n", i);
                assert(_validate_rbtree(bt));
            }
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

        assert(_validate_rbtree(bt));

        binTreeDestory(bt, 1);
    }
    _TEST_END();
}

static inline void test_rbTreeSearch()
{
    _TEST_BEGIN();
    srand(time(NULL));

    BinTree *bt;
    BinTreeNode *btn;
    int arrSize, keyArr[__ARR_MAX_SIZE * 2];

    bt = rbTreeCreate(bstIntKeyCmp, 0);
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
        btn = rbTreeNodeCreate(sizeof(char *), keyArr + i, FREE, NULL);
        int res = rbTreeInsert(bt, btn);
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

        btn = rbTreeSearch(bt, keyArr + i);
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

static inline void test_rbTreeDelete()
{
    _TEST_BEGIN();
    srand(time(NULL));

    BinTree *bt;
    BinTreeNode *btn;
    int arrSize, keyArr[__ARR_MAX_SIZE * 2];

    bt = rbTreeCreate(bstIntKeyCmp, 0);

    arrSize = rand() % __ARR_MAX_SIZE;
    printf("arrSize: %d\n", arrSize);
    int btSize = 0;
    for (int i = 0; i < arrSize; i++)
    {
        keyArr[i] = rand() % __MAX_INT_KEY;
        btn = rbTreeNodeCreate(sizeof(char *), keyArr + i, FREE, NULL);
        int res = rbTreeInsert(bt, btn);
        assert(res >= 0);
        if (res == 0)
        {
            btSize++;
        }
        assert(_validate_rbtree(bt));
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
        fflush(stdout);
        btn = rbTreeDelete(bt, keyArr + i);
        assert(_validate_rbtree(bt));
        printf("\n---------------> !!!RBTree Valid!!! <------------------\n");
        fflush(stdout);
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
        btn = rbTreeNodeCreate(sizeof(char *), keyArr + i, FREE, NULL);
        int res = rbTreeInsert(bt, btn);
        assert(_validate_rbtree(bt));
        assert(res >= 0);
        if (res == 0)
        {
            btSize++;
        }
    }

    for (int i = 0; i < 10000 && btSize > 0; i++)
    {
        int idx = rand() % arrSize;
        btn = rbTreeDelete(bt, keyArr + idx);
        assert(_validate_rbtree(bt));
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

int main(void)
{

    test_rbTreeInsert();
    test_rbTreeSearch();
    test_rbTreeDelete();
    return 0;
}