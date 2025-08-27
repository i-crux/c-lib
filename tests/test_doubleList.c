#include <assert.h>
#include <stdlib.h>

#include "test.h"
#include "doubleList.h"

static void _deNodeFuncDummy(DoubleListNode *p) {
    printf("free(%p)\n", p);
    free(p);
}

static void test_initDoubleListNode() {
    _TEST_BEGIN();

    DoubleListNode dln;
    initDoubleListNode(&dln);
    assert(dln.next == &dln && dln.prev == &dln && dln.doubleList == NULL);

    DoubleListNode *dlnp = malloc(sizeof(DoubleListNode));
    assert(dlnp);
    initDoubleListNode(dlnp);
    assert(dlnp->next == dlnp && dlnp->prev == dlnp && dlnp->doubleList == NULL);
    free(dlnp);

    _TEST_END();
}

static void test_createDoubleListNode() {
    _TEST_BEGIN();

    DoubleListNode *dlnp = createDoubleListNode();
    assert(dlnp);
    assert(dlnp->next == dlnp && dlnp->prev == dlnp && dlnp->doubleList == NULL);
    free(dlnp);
    
    _TEST_END();
}

static void test_initDoubleList() {
    _TEST_BEGIN();

    DoubleList dl1;

    initDoubleList(&dl1, NULL);
    assert(dl1.header.prev == &dl1.header && 
           dl1.header.next == &dl1.header && dl1.header.doubleList == &dl1);
    assert(dl1.deNode == NULL && dl1.nodeCnt == 0);

    initDoubleList(&dl1, _deNodeFuncDummy);
    assert(dl1.header.prev == &dl1.header && 
           dl1.header.next == &dl1.header && dl1.header.doubleList == &dl1);
    assert(dl1.deNode == _deNodeFuncDummy && dl1.nodeCnt == 0);

    _TEST_END();
}

static void test_createDoubleList() {
    _TEST_BEGIN();

    DoubleList *dlp1 = createDoubleList(NULL);
    assert(dlp1->header.prev == &dlp1->header && 
           dlp1->header.next == &dlp1->header && dlp1->header.doubleList == dlp1);
    assert(dlp1->deNode == NULL && dlp1->nodeCnt == 0);
    free(dlp1);

    dlp1 = createDoubleList(_deNodeFuncDummy);
    assert(dlp1->header.prev == &dlp1->header && 
           dlp1->header.next == &dlp1->header && dlp1->header.doubleList == dlp1);
    assert(dlp1->deNode == _deNodeFuncDummy && dlp1->nodeCnt == 0);
    free(dlp1);

    _TEST_END();
}

static void test_checkDoubListNode() {
    _TEST_BEGIN();

    DoubleListNode  dln;
    DoubleList      dl;

    initDoubleListNode(&dln);
    initDoubleList(&dl, NULL);

    assert(!checkDoubListNode(&dln));

    dln.doubleList = &dl;
    
    assert(checkDoubListNode(&dln));


    _TEST_END();
}

static void test_isDoubleListSentinel() {
    _TEST_BEGIN();

    DoubleListNode  dln;
    DoubleList      dl;

    initDoubleListNode(&dln);
    initDoubleList(&dl, NULL);

    assert(!isDoubleListSentinel(&dln));
    assert(isDoubleListSentinel(&dl.header));


    _TEST_END();
}


static void test_addDoubleListNodeAfter() {
    _TEST_BEGIN();

    DoubleList  dl;
    initDoubleList(&dl, NULL);

    DoubleListNode  dln;
    initDoubleListNode(&dln);

    addDoubleListNodeAfter(&dl.header, &dln);
    assert(dl.header.next == &dln && dl.header.prev == &dln && dl.nodeCnt == 1 && dl.header.doubleList == &dl);
    assert(dln.doubleList == &dl && dln.next == &dl.header && dln.prev == &dl.header);

    DoubleListNode  dln1;
    initDoubleListNode(&dln1);
    addDoubleListNodeAfter(&dln, &dln1);
    assert(dl.header.next == &dln && dl.header.prev == &dln1 && dl.nodeCnt == 2 && dl.header.doubleList == &dl);
    assert(dln.doubleList == &dl && dln.next == &dln1 && dln.prev == &dl.header);
    assert(dln1.doubleList == &dl && dln1.next == &dl.header && dln1.prev == &dln);

    DoubleListNode  dln2;
    initDoubleListNode(&dln2);
    addDoubleListNodeAfter(dl.header.prev, &dln2);
    assert(dl.header.next == &dln && dl.header.prev == &dln2 && dl.nodeCnt == 3 && dl.header.doubleList == &dl);
    assert(dln.doubleList == &dl && dln.next == &dln1 && dln.prev == &dl.header);
    assert(dln1.doubleList == &dl && dln1.next == &dln2 && dln1.prev == &dln);
    assert(dln2.doubleList == &dl && dln2.next == &dl.header && dln2.prev == &dln1);

    _TEST_END();
}

static void test_removeDoubleListNode() {
    _TEST_BEGIN();

    DoubleList  dl;
    initDoubleList(&dl, NULL);

    removeDoubleListNode(&dl.header);
    assert(dl.header.prev == &dl.header && 
           dl.header.next == &dl.header && dl.header.doubleList == &dl && dl.header.doubleList == &dl);
    assert(dl.deNode == NULL && dl.nodeCnt == 0);
    assert(isDoubleListSentinel(&dl.header));
    


    DoubleListNode  dln;
    initDoubleListNode(&dln);

    addDoubleListNodeAfter(&dl.header, &dln);

    DoubleListNode  dln1;
    initDoubleListNode(&dln1);
    addDoubleListNodeAfter(&dln, &dln1);

    DoubleListNode  dln2;
    initDoubleListNode(&dln2);
    addDoubleListNodeAfter(dl.header.prev, &dln2);

    removeDoubleListNode(&dln1);
    assert(dl.header.next == &dln && dl.header.prev == &dln2 && dl.nodeCnt == 2 && dl.header.doubleList == &dl);
    assert(dln.doubleList == &dl && dln.next == &dln2 && dln.prev == &dl.header);
    assert(dln1.doubleList == NULL && dln1.next == &dln1 && dln1.prev == &dln1);
    assert(dln2.doubleList == &dl && dln2.next == &dl.header && dln2.prev == &dln);

    removeDoubleListNode(dl.header.next);
    assert(dl.header.next == &dln2 && dl.header.prev == &dln2 && dl.nodeCnt == 1 && dl.header.doubleList == &dl);
    assert(dln.doubleList == NULL && dln.next == &dln && dln.prev == &dln);
    assert(dln1.doubleList == NULL && dln1.next == &dln1 && dln1.prev == &dln1);
    assert(dln2.doubleList == &dl && dln2.next == &dl.header && dln2.prev == &dl.header);

    removeDoubleListNode(dl.header.prev);
    assert(dl.header.next == &dl.header && dl.header.prev == &dl.header && dl.nodeCnt == 0 && dl.header.doubleList == &dl);
    assert(dln.doubleList == NULL && dln.next == &dln && dln.prev == &dln);
    assert(dln1.doubleList == NULL && dln1.next == &dln1 && dln1.prev == &dln1);
    assert(dln2.doubleList == NULL && dln2.next == &dln2 && dln2.prev == &dln2);

    _TEST_END();
}


static void test_destoryDoubleList() {
    _TEST_BEGIN();

    DoubleList  dl;
    initDoubleList(&dl, NULL);

    DoubleListNode  dln;
    initDoubleListNode(&dln);

    addDoubleListNodeAfter(&dl.header, &dln);

    DoubleListNode  dln1;
    initDoubleListNode(&dln1);
    addDoubleListNodeAfter(&dln, &dln1);

    DoubleListNode  dln2;
    initDoubleListNode(&dln2);
    addDoubleListNodeAfter(dl.header.prev, &dln2);

    destoryDoubleList(&dl, 0);
    assert(dl.header.prev == &dl.header && 
           dl.header.next == &dl.header && dl.header.doubleList == &dl && dl.header.doubleList == &dl);
    assert(dl.deNode == NULL && dl.nodeCnt == 0);
    assert(isDoubleListSentinel(&dl.header));

    DoubleList *dlp = createDoubleList(_deNodeFuncDummy);
    DoubleListNode *dlnp1, *dlnp2, *dlnp3;
    dlnp1 = createDoubleListNode();
    dlnp2 = createDoubleListNode();
    dlnp3 = createDoubleListNode();

    addNodeAtDoubleListHeader(dlp, dlnp1);
    addNodeAtDoubleListTail(dlp, dlnp2);
    addNodeAtDoubleListTail(dlp, dlnp3);

    destoryDoubleList(dlp, 1);

    _TEST_END();
}


int main(void) {
    test_initDoubleListNode();
    test_createDoubleListNode();
    test_initDoubleList();
    test_createDoubleList();
    test_checkDoubListNode();
    test_isDoubleListSentinel();
    test_addDoubleListNodeAfter();
    test_removeDoubleListNode();
    test_destoryDoubleList();
    return 0;
}