#include "binTree.h"
#include "utils.h"



void binTreeInit(BinTree *binTree, binTreeElemCmp cmp, binTreeDateFree freeData) {
    binTree->cmp = cmp;
    binTree->freeData = freeData;
    binTree->root = NULL;
    binTree->size = 0;
}


BinTree *binTreeCreate(binTreeElemCmp cmp, binTreeDateFree freeData) {
    BinTree *bt = MALLOC(sizeof(BinTree));

    ckpvThenReturn(bt, NULL, NULL);

    binTreeInit(bt, cmp, freeData);
    
    return bt;
}
