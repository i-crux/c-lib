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


BinTreeNode *bstDelete(BinTree *bt, void *key) {
    BinTreeNode *btn = bstSearch(bt, key), *parent, *child, *successor;

    if(!btn) {
        goto _done;
    }

    parent = btn->parent;

    if(!btn->left || !btn->right) {
        /* one child the [btn] has at most */
        child = btn->left ? btn->left : btn->right;
        if(parent) {
            parent->left == btn ? (parent->left = child, 0) : (parent->right = child, 1);
        } else {    /* root */
            bt->root = child;
        }
        if(child) {
            child->parent = parent;
        }
    } else {    /* btn->left != NULL && btn->right != NULL */
        successor = btn->right;
        while(successor->left) {
            successor = successor->left;
        }
        parent = successor->parent;
        child = successor->right;
        /* succesor always be the left child of parent, but the parent is [btn] */
        parent->left == successor ? (parent->left = child, 0) : (parent->right = child, 1);
        if(child) {
            child->parent = parent;
        }
        successor->parent = btn->parent;
        successor->left = btn->left;
        successor->right = btn->right;
        successor->property = btn->property;
    }
    bt->size--;
    btn->property = 0;
    btn->parent = btn->left = btn->right = NULL;

_done:
    return btn;
}