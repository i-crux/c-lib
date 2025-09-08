#include "binTree.h"
#include "utils.h"
#include "queue.h"
#include "stack.h"



void binTreeInit(BinTree *binTree, binTreeElemCmp cmp, bool allowSameKey) {
    binTree->cmp = cmp;
    binTree->root = NULL;
    binTree->size = 0;
    binTree->allowSameKey = allowSameKey;
}


BinTree *binTreeCreate(binTreeElemCmp cmp, bool allowSameKey) {
    BinTree *bt = MALLOC(sizeof(BinTree));

    ckpvThenReturn(bt, NULL, NULL);

    binTreeInit(bt, cmp, allowSameKey);
    
    return bt;
}

BinTreeNodeData *binTreeNodeDataCreate(size_t dataSize, void *data) {
    BinTreeNodeData *btnd = MALLOC(sizeof(BinTreeNodeData) + dataSize - 1);
    
    ckpvThenReturn(btnd, NULL, NULL);

    initDoubleListNode(&btnd->dln);

    memcpy(btnd->data, data, dataSize);

    return btnd;
}

BinTreeNode *binTreeNodeCreate(uintptr_t property, size_t keySize, void *key, 
                               doubleListNodeFree deNode, binTreeNodeFreeKey freeKey)
{
    BinTreeNode *btn = MALLOC(sizeof(BinTreeNode) + keySize - 1);

    ckpvThenReturn(btn, NULL, NULL);

    if (deNode) {
        initDoubleList(&btn->dlist, deNode);
    } else {
        initDoubleList(&btn->dlist, FREE);
    }
    

    btn->property = property;
    btn->freeKey = freeKey;
    memcpy(btn->key, key, keySize);

    btn->parent = btn->left = btn->right = NULL;

    return btn;
}

bool binTreeNodeAddData(BinTreeNode *btn, size_t dataSize, void *data) {
    BinTreeNodeData *btnd = binTreeNodeDataCreate(dataSize, data);

    ckpvThenReturn(btnd, NULL, 0);

    addNodeAtDoubleListTail(&btn->dlist, &btnd->dln);

    return 1;
}

bool binTreeTravalPreorder(BinTree *binTree, binTreeNodeVisit binTreeNodeVisit) {
    bool ret = 0;
    BinTreeNode *btn;
    Stack *stack;

    if(!binTree || !binTree->root || !binTreeNodeVisit) {
        goto _done;
    }

    stack = stackCreate(sizeof(BinTreeNode *));
    if (!stack) {
        goto _done;
    }

    btn = binTree->root;

    if(!stackPush(stack, &btn)) {
        goto _error;
    }
    
    while(!stackEmpty(stack)) {
        stackPop(stack, &btn);
        binTreeNodeVisit(btn);
        if(btn->right) {
            if(!stackPush(stack, &btn->right)) {
                goto _error;
            }
        }
        if(btn->left) {
            if(!stackPush(stack, &btn->left)) {
                goto _error;
            }
        }
    }


    ret = 1;
_error:
    stackDestroy(stack, 1);
_done:
    return ret;
}


bool binTreeTravalInorder(BinTree *binTree, binTreeNodeVisit binTreeNodeVisit) {
    BinTreeNode *btn;
    bool ret = 0;

    if(!binTree || !binTree->root || !binTreeNodeVisit) {
        goto _done;
    }

    ret = 1;
    btn = binTree->root;

_begin:
    while(btn->left) {
        btn = btn->left;
    } 

_again:
    binTreeNodeVisit(btn);

    if(btn->right) {
        btn = btn->right;
        goto _begin;
    } else {
    _rightChild:
        if(!btn->parent) {
            goto _done;
        }
        if(btn->parent->left == btn) {
            btn = btn->parent;
            goto _again;
        } else {    /* right child  */ 
            btn = btn->parent;
            goto _rightChild;
        }
    }

_done:
    return ret;
}


bool binTreeTravalPostorder(BinTree *binTree, binTreeNodeVisit binTreeNodeVisit) {
    bool ret = 0;
    BinTreeNode *btn, *lastVisited = NULL;
    Stack *stack;

    if(!binTree || !binTree->root || !binTreeNodeVisit) {
        goto _done;
    }

    stack = stackCreate(sizeof(BinTreeNode *));
    if (!stack) {
        goto _done;
    }

    btn = binTree->root;

    while(btn || !stackEmpty(stack)) {
        while(btn) {
            if(!stackPush(stack, &btn)) {
                goto _error;
            }
            btn = btn->left;
        }
        
        btn = *(BinTreeNode **)stackPeek(stack);

        if (btn->right && lastVisited != btn->right) {
            btn = btn->right;
        } else {
            stackPop(stack, &btn);
            binTreeNodeVisit(btn);
            lastVisited = btn;
        }
    }

    ret = 1;
_error:
    stackDestroy(stack, 1);
_done:
    return ret;
}

bool binTreeTravalLevelorder(BinTree *binTree, binTreeNodeVisit binTreeNodeVisit) {
    Queue *que;
    BinTreeNode *btn;
    bool ret = 0;

    if(!binTree || !binTree->root || !binTreeNodeVisit) {
        goto _done;
    }

    que = queueCreate(sizeof(BinTreeNode *));
    if (!que) {
        goto _done;
    }

    if (!enQueue(que, &binTree->root)) {
        goto _error;
    }

    while(!queueEmpty(que)) {
        deQueue(que, &btn);
        if(btn->left) {
            if (!enQueue(que, &btn->left)) {
                goto _error;
            }
        }
        if(btn->right) {
            if (!enQueue(que, &btn->right)) {
                goto _error;
            }
        }
        binTreeNodeVisit(btn);
    }

    ret = 1;
_error:
    queueDestroy(que, 1);
_done:
    return ret;
}


static void __freeBinTreeNode(BinTreeNode *btn) {
    destoryDoubleList(&btn->dlist, 0);
    if(btn->freeKey) {
        btn->freeKey(btn->key);
    }
    FREE(btn);
}

static inline void __binTreeDestory(BinTreeNode *btn) {
    if (!btn) {
        return;
    }
    __binTreeDestory(btn->left);
    __binTreeDestory(btn->right);
    __freeBinTreeNode(btn);
}

void binTreeDestory(BinTree *binTree, bool freeSelf) {
    
    if(binTree) {
        __binTreeDestory(binTree->root);
    }

    if(freeSelf) {
        FREE(binTree);
    }

}
