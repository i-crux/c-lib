#include "test.h"
#include "binTree.h"
#include "utils.h"

static void __freeDummy(void *p) {
    printf("%p\n", p);
}

static inline void test_binTreeInit() {
    _TEST_BEGIN();

    BinTree bt, *btp;

    binTreeInit(&bt, NULL, 1);
    assert(bt.allowSameKey == 1);
    assert(bt.cmp == NULL);
    assert(bt.root == NULL);
    assert(bt.size == 0);

    btp = binTreeCreate(NULL, 0);
    assert(btp);
    assert(btp->allowSameKey == 0);
    assert(btp->root == NULL);
    assert(btp->cmp == NULL);
    assert(btp->size == 0);

    FREE(btp);

    _TEST_END();
}

#define __STR_DATA "hello, bitch"

static inline void test_binTreeNodeDataCreate() {
    _TEST_BEGIN();

    int             intData = 1024;
    char            *strData = __STR_DATA;
    BinTreeNodeData *btndp;

    btndp = binTreeNodeDataCreate(sizeof(int), &intData);
    assert(btndp);
    assert(*(int *)(btndp->data) == intData);
    assert(btndp->dataSize == sizeof(int));
    FREE(btndp);

    btndp = binTreeNodeDataCreate(sizeof(__STR_DATA), strData);
    assert(btndp);
    printf("%s   %s\n", (char *)btndp->data, strData);
    for(size_t i = 0; i < sizeof(__STR_DATA); i++) {
        assert(((char *)(btndp->data))[i] == strData[i]);
    }
    FREE(btndp);

    _TEST_END();
}


static inline void test_binTreeNodeCreate() {
    _TEST_BEGIN();

    int     intKey = 1024;
    char    *strKey = malloc(sizeof(__STR_DATA));
    assert(strKey);
    memcpy(strKey, __STR_DATA, sizeof(__STR_DATA));
    
    BinTreeNode  *btnp = binTreeNodeCreate(0, sizeof(int), &intKey, NULL, NULL);
    assert(btnp);
    assert(btnp->freeKey == NULL);
    assert(*(int *)(btnp->key) == intKey);
    assert(btnp->left == NULL);
    assert(btnp->right == NULL);
    assert(btnp->parent == NULL);
    assert(btnp->property == 0);
    assert(btnp->dlist.deNode == FREE);
    assert(btnp->keySize == sizeof(int));
    FREE(btnp);

    btnp = binTreeNodeCreate(0, sizeof(char *), &strKey, __freeDummy, free);
    assert(btnp);
    assert(btnp->freeKey == free);
    assert(*(char **)(btnp->key) == strKey);
    assert(btnp->left == NULL);
    assert(btnp->right == NULL);
    assert(btnp->parent == NULL);
    assert(btnp->property == 0);
    assert(btnp->dlist.deNode == __freeDummy);
    printf("%s\n", *(char **)(btnp->key));
    btnp->freeKey(*(char **)(btnp->key));
    assert(btnp->keySize == sizeof(char *));
    FREE(btnp);
    

    _TEST_END();
}


int main(void) {

    test_binTreeInit();
    test_binTreeNodeDataCreate();
    test_binTreeNodeCreate();


    return 0;
}