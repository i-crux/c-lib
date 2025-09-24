#include "avlTree.h"
#include "utils.h"

/**
 * @brief update a [AVLTreeNode] height
 *
 * @param avltn: pointor to [AVLTreeNode]
 */
static inline void _avlTreeUpdateHeight(AVLTreeNode *avltn)
{

    AVLTreeNode *left, *right;
    intptr_t hl, hr;
    if (!avltn)
    {
        return;
    }

    left = avltn->left;
    right = avltn->right;

    hl = left ? left->property : 0;
    hr = right ? right->property : 0;

    avltn->property = max(hl, hr) + 1;
}

/**
 * @brief get balance factor of a [AVLTree]
 *
 * @param avltn: pointor to a [AVLTreeNode]
 * @return subof(height-of-rigth-subtree, height-of-left-subtree)
 */
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

/*          n             nl                         */
/*         /             /  \                        */
/*        nl      ==>   lh   n                       */
/*       /                                           */
/*      lh                                           */
/*                                                   */
/*          n              n              nlr        */
/*         / \            / \            /   \       */
/*        nl  0          nlr 0          nl    n      */
/*       /  \     ==>    /        ==>  /  \    \     */
/*      0   nlr         nl            0   lh    0    */
/*          /          /  \                          */
/*         lh         0   lh                         */
/*                                                   */
/*          n               n              nlr       */
/*         /               /              /   \      */
/*        nl       ==>    nlr     ==>    nl    n     */
/*          \            /                           */
/*          nlr         nl                           */
/*                                                   */
/*          n                n              nlr      */
/*         / \              / \            /   \     */
/*        nl  0            nlr 0          nl    n    */
/*       /  \       ==>   /  \     ==>   /     / \   */
/*      0   nlr          nl  rh         0     rh  0  */
/*             \        /                            */
/*             rh      0                             */
/*                                                   */
/*          n                nl                      */
/*         /                /  \                     */
/*        nl         ==>   0    n                    */
/*       /  \                  /                     */
/*      0    0                0                      */

/**
 * @brief rebalance a [AVLTree]
 *
 * @param avlt: pointor to a [AVLTree]
 * @param avltn: pointor to a [AVLTreeNode]
 * @param height: 0: keep same height; 1: get higher; -1: get shorter
 */
static inline void _avlTreeRebalance(AVLTree *avlt, AVLTreeNode *avltn)
{
    AVLTreeNode *parent = avltn, **inserted;
    intptr_t bf;

    while (parent)
    {
        _avlTreeUpdateHeight(parent);

        if (parent->parent)
        {
            inserted = parent->parent->left == parent ? &parent->parent->left : &parent->parent->right;
        }
        else
        {
            inserted = &avlt->root;
        }

        bf = _avlTreeGetBalanceFactor(parent);

        if (bf < -1) /* left tree too higher */
        {
            avltn = parent->left;
            if (_avlTreeGetBalanceFactor(avltn) == 1) /* right tree higher */
            {
                binTreeLeftRotate(&parent->left, avltn);
                _avlTreeUpdateHeight(avltn);
                avltn = parent->left;
                _avlTreeUpdateHeight(avltn);
            }

            binTreeRightRotate(inserted, parent);
        }
        else if (bf > 1) /* right tree too higher */
        {
            avltn = parent->right;
            if (_avlTreeGetBalanceFactor(avltn) == -1) /* left tree higher */
            {
                binTreeRightRotate(&parent->right, avltn);
                _avlTreeUpdateHeight(avltn);
                avltn = parent->right;
                _avlTreeUpdateHeight(avltn);
            }

            binTreeLeftRotate(inserted, parent);
        }
        _avlTreeUpdateHeight(parent);
        parent = *inserted;
        _avlTreeUpdateHeight(parent);
        parent = parent->parent;
    }
}

int avlTreeInsert(AVLTree *avlt, AVLTreeNode *avltn)
{
    int ret = bstInsert(avlt, avltn);

    if (ret != 0)
    {
        goto _done;
    }

    _avlTreeRebalance(avlt, avltn);

_done:
    return ret;
}

static AVLTreeNode *_getDeleteNode(AVLTreeNode *avltn)
{
    AVLTreeNode *ret;

    if (avltn->left == NULL || avltn->right == NULL)
    {
        ret = avltn;
    }
    else
    {
        ret = avltn->right;
        while (ret->left)
        {
            ret = ret->left;
        }
    }

    return avltn->right == ret ? ret : ret->parent;
}

AVLTreeNode *avlTreeDelete(AVLTree *avlt, void *key)
{
    AVLTreeNode *avltn = bstSearch(avlt, key), *deleted;
    ckpvThenReturn(avltn, NULL, NULL);

    deleted = _getDeleteNode(avltn);
    avltn = bstDelete(avlt, key);

    _avlTreeRebalance(avlt, deleted);

    return avltn;
}
