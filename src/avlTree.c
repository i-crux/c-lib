#include "stdlib.h"
#include "avlTree.h"

/**
 * @brief left property in [avlNode]
 *
 * @param avltree: pointor to [AVLTree]
 * @param avlNode: pointor to [AVLTreeNode]
 */
static inline void
_leftBalance(AVLTree *avltree, AVLTreeNode *avlNode)
{
    AVLTreeNode *nl = avlNode->left;
    AVLTreeNode *nlr;
    AVLTreeNode **inserted;
    if (avlNode->parent)
    {
        inserted = avlNode->parent->left == avlNode ? &avlNode->parent->left : &avlNode->parent->right;
    }
    else
    {
        inserted = &avltree->root;
    }
    switch (nl->property)
    {
    case leftHeight:
        nl->property = sameHeight;             /*          n             nl                         */
        avlNode->property = sameHeight;        /*         /             /  \                        */
        binTreeRightRotate(inserted, avlNode); /*        nl      ==>   lh   n                       */
                                               /*       /                                           */
        break;                                 /*      lh                                           */
    case rightHeight:                          /*                                                   */
        nlr = nl->right;                       /*          n              n              nlr        */
        switch (nlr->property)                 /*         / \            / \            /   \       */
        {                                      /*        nl  0          nlr 0          nl    n      */
        case leftHeight:                       /*       /  \     ==>    /        ==>  /  \    \     */
            nl->property = sameHeight;         /*      0   nlr         nl            0   lh    0    */
            avlNode->property = rightHeight;   /*          /          /  \                          */
            break;                             /*         lh         0   lh                         */
        case sameHeight:                       /*                                                   */
            nl->property = sameHeight;         /*          n               n              nlr       */
            avlNode->property = sameHeight;    /*         /               /              /   \      */
            break;                             /*        nl       ==>    nlr     ==>    nl    n     */
        case rightHeight:                      /*          \            /                           */
            nl->property = leftHeight;         /*          nlr         nl                           */
            avlNode->property = sameHeight;    /*                                                   */
            break;                             /*          n                n              nlr      */
        } /*                                              / \              / \            /   \     */
        nlr->property = sameHeight;            /*        nl  0            nlr 0          nl    n    */
        binTreeLeftRotate(&avlNode->left, nl); /*       /  \       ==>   /  \     ==>   /     / \   */
                                               /*      0   nlr          nl  rh         0     rh  0  */
        binTreeRightRotate(inserted, avlNode); /*             \        /                            */
                                               /*             rh      0                             */
        break;                                 /*                                                   */
    case sameHeight:                           /*          n                nl                      */
        nl->property = rightHeight;            /*         /                /  \                     */
        binTreeRightRotate(inserted, avlNode); /*        nl         ==>   0    n                    */
                                               /*       /  \                  /                     */
        break;                                 /*      0    0                0                      */
    }
}

static inline void
_rightBalance(AVLTree *avltree, AVLTreeNode *avlNode)
{
    AVLTreeNode *nr = avlNode->right;
    AVLTreeNode *nrl;
    AVLTreeNode **inserted;
    if (avlNode->parent)
    {
        inserted = avlNode->parent->left == avlNode ? &avlNode->parent->left : &avlNode->parent->right;
    }
    else
    {
        inserted = &avltree->root;
    }

    switch (nr->property)
    {
    case rightHeight:
        nr->property = sameHeight;
        avlNode->property = sameHeight;
        binTreeLeftRotate(inserted, avlNode);
        break;
    case leftHeight:
        nrl = nr->left;
        switch (nrl->property)
        {
        case rightHeight:
            nr->property = sameHeight;
            avlNode->property = leftHeight;
            break;
        case sameHeight:
            nr->property = sameHeight;
            avlNode->property = sameHeight;
            break;
        case leftHeight:
            nr->property = rightHeight;
            avlNode->property = sameHeight;
            break;
        }
        nrl->property = sameHeight;
        binTreeRightRotate(&avlNode->right, nr);
        binTreeLeftRotate(inserted, avlNode);
    case sameHeight:
        nr->property = leftHeight;
        binTreeLeftRotate(inserted, avlNode);
        break;
    }
}

/**
 * @brief insert a node to [AVLTree]
 *
 * @param proot: parent of [avlNode]
 * @param root: insert point
 * @param avlNode: the node will be inserted
 * @param taller: the [AVLTree] get taller of not
 * @param avltree: pointor to the [AVLTree]
 *
 * @return 0 on successful, 1 on duplicated key, -1 error
 */
static inline int _doAvlTreeInsert(AVLTreeNode *proot, AVLTreeNode **root, AVLTreeNode *avlNode, bool *taller, AVLTree *avltree)
{
    int cmpresult; /* compare result for the [key] */
    DoubleListNode *dln;

    if (*root == NULL) /* the tree maybe empty or find the insert point */
    {
        *taller = 1; /* insert successful. the tree get taller */
        if (proot != NULL)
            avlNode->parent = proot;
        else
            avlNode->parent = NULL;
        *root = avlNode; /* insert the node to the tree */
        return 0;        /* insert successful */
    }
    else
    {

        cmpresult = avltree->cmp(proot->key, avlNode->key);
        if (cmpresult == 0)
        {
            /* key already exists */
            if (avltree->allowSameKey)
            { /* not allowed duplicated key */
                /* allowed duplicated key */
                dln = avlNode->dlist.header.next;
                if (!isDoubleListSentinel(dln))
                {
                    removeDoubleListNode(dln);
                    addNodeAtDoubleListTail(&proot->dlist, dln);
                }
            }
            return 1;
        }
        /* insert to left tree */
        else if (cmpresult == 1)
        {
            if (_doAvlTreeInsert(*root, &proot->left, avlNode, taller, avltree) == 0)
            {
                if (*taller)
                {
                    switch (proot->property)
                    {
                    case leftHeight:
                        _leftBalance(avltree, proot);
                        *taller = 0;
                        break;
                    case sameHeight:
                        proot->property = leftHeight;
                        *taller = 1;
                        break;
                    case rightHeight:
                        proot->property = sameHeight;
                        *taller = 0;
                        break;
                    }
                }
            }
            return 0;
        }
        else
        {
            if (_doAvlTreeInsert(*root, &proot->right, avlNode, taller, avltree) == 0)
            {
                if (*taller)
                {
                    switch (proot->property)
                    {
                    case rightHeight:
                        _rightBalance(avltree, proot);
                        *taller = 0;
                        break;
                    case sameHeight:
                        proot->property = rightHeight;
                        *taller = 1;
                        break;
                    case leftHeight:
                        proot->property = sameHeight;
                        *taller = 0;
                        break;
                    }
                }
            }
            return 0;
        }
    }
    return 1;
}
