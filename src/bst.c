#include "bst.h"

int bstInsert(BinTree *bt, BinTreeNode *btn) {
    int ret = -1, res;
    BinTreeNode **inserted, *parent = NULL;
    DoubleListNode  *dln;

    if (!bt || !btn) {
        goto _done;
    }

    inserted = &bt->root;
    
    while(*inserted) {
        parent = *inserted;
        res = bt->cmp(parent->key, btn->key);
        if(res == 0) {
            /* key already exists */
            ret = 1;
            if(!bt->allowSameKey) { /* not allowed duplicated key */
                goto _done;
            }
            /* allowed duplicated key */
            dln = btn->dlist.header.next;
            if(!isDoubleListSentinel(dln)) {
                removeDoubleListNode(dln);
                addNodeAtDoubleListTail(&parent->dlist, dln);
            }

            goto _done;
        }

        if(res < 0) {
            inserted = &parent->right;
        } else { /* > 0 */
            inserted = &parent->left;
        }
    }

    ret = 0;
    *inserted = btn;
    btn->parent = parent;
    bt->size ++;

_done:
    return ret;
}


BinTreeNode *bstSearch(BinTree *bt, void *key) {
    BinTreeNode *btn = NULL, *root;
    int         res;

    if(!bt || !key) {
        goto _done;
    }
    root = bt->root;

    while(root) {
        res = bt->cmp(root->key, key);
        if (res < 0) {
            root = root->right;
        } else if (res > 0) {
            root = root->left;
        } else { /* res == 0 */
            btn = root;
            goto _done;
        }
    }

_done:
    return btn;
}

/**
 * @brief connect parent[p] and child[c]
 * 
 * @param t: pointor to [BinTree]
 * @param p: parent [BinTreeNode] pointor
 * @param c: child [BinTreeNode] pointor
 * @param fc: former child of parent[p]
 */
#define _connChild(t, p, c, fc)                                                         \
    do {                                                                                \
        BinTree *__cc_bt = (t);                                                         \
        BinTreeNode *__cc_parent = (p), *__cc_child = (c);                              \
        BinTreeNode *__cc_formerChild = (fc);                                           \
        if(__cc_parent) {                                                               \
            __cc_parent->left == __cc_formerChild ? (__cc_parent->left = __cc_child) :  \
                                                    (__cc_parent->right = __cc_child);  \
        } else {                                                                        \
            __cc_bt->root = __cc_child;                                                 \
        }                                                                               \
        if(__cc_child) {                                                                \
            __cc_child->parent = __cc_parent;                                           \
        }                                                                               \
    } while(0)

BinTreeNode *bstDelete(BinTree *bt, void *key) {
    BinTreeNode *btn = bstSearch(bt, key), *parent, *child, *successor;

    if(!btn) {
        goto _done;
    }


    if(!btn->left || !btn->right) {
        parent = btn->parent;
        /* one child the [btn] has at most */
        child = btn->left ? btn->left : btn->right;
    
        _connChild(bt, parent, child, btn);
        
    } else {    /* btn->left != NULL && btn->right != NULL */
        /* get the successor of [btn] */
        successor = btn->right;
        
        while(successor->left) {
            successor = successor->left;
        }

        /* remove successor from the [BST] */
        parent = successor->parent;
        /* if [btn] has child, only [right] child */
        child = successor->right;
        _connChild(bt, parent, child, successor);

        successor->parent = btn->parent;
        successor->left = btn->left;
        if(successor->left) {successor->left->parent = successor;}
        successor->right = btn->right;
        if(successor->right) {successor->right->parent = successor;}
        successor->property = btn->property;

        parent = btn->parent;
        child = successor;
        _connChild(bt, parent, child, btn);
    }
    bt->size--;
    btn->property = 0;
    btn->parent = btn->left = btn->right = NULL;

_done:
    return btn;
}