# AVL Tree

**Definition:** A height-balanced binary search tree. For every node `x`, the height difference between its left and right subtrees is at most 1.

## AVL Tree Node Structure

```c
struct AVLTreeNode
    bf ;          // Balance factor: 1 (LH, left high by 1), 0 (EH, balanced), -1 (RH, right high by 1)
    key_t k ;     // Key
    Data_t data ; // Satellite data
    parent ;      // Parent node
    left ;        // Left child
    right ;       // Right child
```

## Imbalance Cases

### LL Case

* Left subtree is higher by 2, and the newly inserted node is on the left of its parent.

```text
        PP                                P
       /        RightRotate             /   \
      P         ================>      x    PP
     /
    x
```

### LR Case

* Left subtree is higher by 2, and the newly inserted node is on the right of its parent.

```text
        PP                     PP                        x           
       /    LeftRotate        /    RightRotate         /   \           
      P     ==========>      X     ===========>       P     PP    
       \                    /                                  
        X                  P                       
```

### RR and RL Cases

* Symmetric to LL and LR cases, respectively.

## Balancing Algorithm

* This algorithm takes a node `x` that lost balance (bf = 2) as input and restores balance.

```c
LeftBalance(x)   // Left subtree of x is unbalanced
    y = left[x];
    switch (bf[y]) {
        case LH:   // LL Case
            bf[x] = EH;
            bf[y] = EH;
            RightRotate(T, x);

        case RH:   // LR Case
            z = right[y];
            switch (bf[z]) {
                case LH:
                    bf[y] = EH;
                    bf[x] = RH;
                    break;
                case EH:
                    bf[y] = EH;
                    bf[x] = EH;
                    break;
                case RH:
                    bf[y] = LH;
                    bf[x] = EH;
                    break;
            }
            bf[z] = EH;
            LeftRotate(T, y);
            RightRotate(T, x);
    }

/* Right balancing is symmetric to left balancing */
```

## AVL Tree Insertion

```c
AVLTreeInsert(t, e, taller)   // t: subtree root, e: node to insert, taller: boolean flag for height change
    if t == nil               // pointer to pointer
        t = e;
        taller = true;        // only if tree height increases, balancing is needed
    else
        if key[t] == key[e]   // duplicate keys are not allowed
            taller = false;   // taller must be a passable value (pointer or global variable)
            return false;

        else if key[t] > key[e] // insert into left subtree
            if !AVLTreeInsert(left[t], e, taller)
                return false;
            if taller
                switch (bf[t]) {
                    case LH:
                        LeftBalance(t);
                        taller = false;
                        break;
                    case EH:
                        bf[t] = LH;
                        taller = true;
                        break;
                    case RH:
                        bf[t] = EH;
                        taller = false;
                        break;
                }
        else                    // insert into right subtree (symmetric)
            ...
    return true;
```
