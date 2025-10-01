#include "rbTree.h"

#define _rbtnIsRed(rbtn) rbtnColor(rbtn)
#define _rbtnSetRed(rbtn) ((rbtn)->property = RBT_RED)
#define _rbtnSetBlack(rbtn)               \
    do                                    \
    {                                     \
        if (rbtn)                         \
        {                                 \
            (rbtn)->property = RBT_BLACK; \
        }                                 \
    } while (0)

/**
 * @brief fix up a [RBTree] after insertion a new node
 *
 * @param rbt: pointor to a [RBTree]
 * @param rbtn: pointor to [RBTreeNode]
 */
static inline void _rbtInsertFixup(RBTree *rbt, RBTreeNode *rbtn)
{
    RBTreeNode *parent, *grandParent, **inserted;

    /* if rbtn->parent == NULL, the color if [rbtn->parent] is [RBT_BLACK] */
    while (_rbtnIsRed(rbtn->parent))
    {

        parent = rbtn->parent;
        /*
         * because of [parent->property == RBT_RED], parent must not be [ROOT]
         * that means [grandParent] must not be [NULL]
         * and the color of [grandParent] must be [RBT_BLACK]
         */
        grandParent = parent->parent;
        inserted = (grandParent->parent ? (grandParent->parent->left == grandParent ? &grandParent->parent->left : &grandParent->parent->right)
                                        : (&rbt->root));

        if (grandParent->left == parent)
        {
            if (_rbtnIsRed(grandParent->right))
            {
                _rbtnSetBlack(parent);
                _rbtnSetBlack(grandParent->right);
                _rbtnSetRed(grandParent);
                rbtn = grandParent;
            }
            else /* color of [grandParent->right] is [RBT_BLACK] */
            {
                if (parent->right == rbtn)
                {
                    binTreeLeftRotate(&grandParent->left, parent);
                    rbtn = parent;
                    parent = rbtn->parent;
                }
                binTreeRightRotate(inserted, grandParent);
                _rbtnSetBlack(parent);
                _rbtnSetRed(grandParent);
            }
        }
        else /* grandParent->right == parent */
        {
            if (_rbtnIsRed(grandParent->left))
            {
                _rbtnSetBlack(parent);
                _rbtnSetBlack(grandParent->left);
                _rbtnSetRed(grandParent);
                rbtn = grandParent;
            }
            else /* color of [grandParent->left] is [RBT_BLACK] */
            {
                if (parent->left == rbtn)
                {
                    binTreeRightRotate(&grandParent->right, parent);
                    rbtn = parent;
                    parent = rbtn->parent;
                }

                binTreeLeftRotate(inserted, grandParent);
                _rbtnSetBlack(parent);
                _rbtnSetRed(grandParent);
            }
        }
    }

    _rbtnSetBlack(rbt->root);
}

int rbTreeInsert(RBTree *rbt, RBTreeNode *rbtn)
{
    int ret = bstInsert(rbt, rbtn);

    if (ret != 0)
    {
        goto _done;
    }

    _rbtInsertFixup(rbt, rbtn);

_done:
    return ret;
}

static inline RBTreeNode *_getDeleteNode(RBTreeNode *rbtn)
{
    RBTreeNode *ret;

    if (rbtn->left == NULL || rbtn->right == NULL)
    {
        ret = rbtn;
    }
    else
    {
        ret = rbtn->right;
        while (ret->left)
        {
            ret = ret->left;
        }
    }

    return ret;
}

static inline void _rbtDeleteFixup(RBTree *rbt, RBTreeNode *rbtn, RBTreeNode *parent)
{
    RBTreeNode *grandParent, **inserted, *sibling;

    while (rbtn != rbt->root && !_rbtnIsRed(rbtn))
    {
        /*
         * color of [rbtn] is [RBT_BLACK]
         * parent of [rbtn] is NOT [NULL]
         */

        parent = rbtn ? rbtn->parent : parent;
        grandParent = parent->parent;
        inserted = (grandParent ? (grandParent->left == parent ? &grandParent->left : &grandParent->right)
                                : &rbt->root);

        if (parent->left == rbtn)
        {
            /*
             * because of the [deleted] node is black,
             * so [sibling] must NOT be [NULL].
             * if [sibling] is [NULL], then the black heigth of original tree is not equal.
             */
            sibling = parent->right;

            if (_rbtnIsRed(sibling))
            {
                binTreeLeftRotate(inserted, parent);
                _rbtnSetBlack(sibling);
                _rbtnSetRed(parent);
                inserted = &sibling->left;
                sibling = parent->right;
            }

            if (!sibling || (!_rbtnIsRed(sibling->left) && !_rbtnIsRed(sibling->right)))
            {
                _rbtnSetRed(sibling);
                rbtn = parent;
                continue;
            }
            /* slibling has a RED child at least */
            if (_rbtnIsRed(sibling->left))
            {
                binTreeRightRotate(&parent->right, sibling);
                _rbtnSetRed(sibling);
                sibling = parent->right;
                _rbtnSetBlack(sibling);
            }

            binTreeLeftRotate(inserted, parent);
            sibling->property = parent->property;
            _rbtnSetBlack(sibling->right);
            _rbtnSetBlack(parent);
            rbtn = rbt->root;
        }
        else /* parent->right == rbtn */
        {
            sibling = parent->left;

            if (_rbtnIsRed(sibling))
            {
                binTreeRightRotate(inserted, parent);
                _rbtnSetBlack(sibling);
                _rbtnSetRed(parent);
                inserted = &sibling->right;
                sibling = parent->left;
            }

            if (!sibling || (!_rbtnIsRed(sibling->left) && !_rbtnIsRed(sibling->right)))
            {
                _rbtnSetRed(sibling);
                rbtn = parent;
                continue;
            }
            /* slibling has a RED child at least */
            if (_rbtnIsRed(sibling->right))
            {
                binTreeLeftRotate(&parent->left, sibling);
                _rbtnSetRed(sibling);
                sibling = parent->left;
                _rbtnSetBlack(sibling);
            }

            binTreeRightRotate(inserted, parent);
            sibling->property = parent->property;
            _rbtnSetBlack(sibling->left);
            _rbtnSetBlack(parent);
            rbtn = rbt->root;
        }
    }

    _rbtnSetBlack(rbtn);
}

RBTreeNode *rbTreeDelete(RBTree *rbt, void *key)
{
    RBTreeNode *rbtn = bstSearch(rbt, key), *deleted, *beginFix, *pbf;
    intptr_t color;
    ckpvThenReturn(rbtn, NULL, NULL);
    deleted = _getDeleteNode(rbtn);
    /*
     * if [deleted] is not rbtn then [deleted] only has [right] at most
     * else [deleted] has left child
     */
    if (deleted == rbtn)
    {
        beginFix = deleted->left ? deleted->left : deleted->right;
        pbf = deleted->parent;
    }
    else
    {
        beginFix = deleted->right;
        pbf = rbtn->right == deleted ? deleted : deleted->parent;
    }

    color = deleted->property;
    rbtn = bstDelete(rbt, key);

    if (color == RBT_BLACK)
    {
        _rbtDeleteFixup(rbt, beginFix, pbf);
    }

    return rbtn;
}