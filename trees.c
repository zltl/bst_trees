#include "trees.h"

#include <malloc.h>

struct tree_node t_null_node = {NULL, NULL, NULL, {BLACK}, NULL, NULL};
struct tree_node *t_nil = &t_null_node;

#define T_KEY_LT(less, k1, k2) less(k1, k2)
#define T_KEY_EQ(less, k1, k2) !(less(k1, k2) || less(k2, k1))

static int max(int a, int b) {
    return a > b ? a : b;
}

struct tree_node *tree_search(struct tree *t, void *key) {
    assert(t);
    struct tree_node *x = t->root;
    while (x != t_nil && !T_KEY_EQ(t->key_less, x->key, key))
        if (T_KEY_LT(t->key_less, key, x->key))
            x = x->left;
        else
            x = x->right;
    return x;
}

void tree_travel(struct tree *t, struct tree_node *r, void(*fn)(struct tree_node *n)) {
    if (r == t_nil)
        return;
    fn(r);
    tree_travel(t, r->left, fn);
    tree_travel(t, r->right, fn);
}

int tree_height(struct tree *t, struct tree_node *x) {
    if (x == t_nil) {
        return 0;
    }
    int l = tree_height(t, x->left);
    int r = tree_height(t, x->right);
    return max(l, r) + 1;
}

struct tree_node *tree_min(struct tree *t, struct tree_node *r) {
    struct tree_node *x = r;
    if (x == t_nil) {
        return x;
    }
    while (x->left != t_nil) {
        x = x->left;
    }
    return x;
}

struct tree_node *tree_max(struct tree *t, struct tree_node *r) {
    struct tree_node *x = r;
    if (x == t_nil) {
        return x;
    }
    while (x->right != t_nil) {
        x = x->right;
    }
    return x;
}

struct tree_node *tree_successor(struct tree *t, struct tree_node *x) {
    assert(t);
    assert(x);
    struct tree_node *y = x->p;
    if (x->right != t_nil)
        return tree_min(t, x->right);
    while (y != t_nil && x == y->right) {
        x = y;
        y = y->p;
    }
    return y;
}

struct tree_node *tree_predecessor(struct tree *t, struct tree_node *x) {
    assert(t);
    assert(x);

    struct tree_node *y = x->p;
    if (x->left != t_nil)
        return tree_max(t, x->left);
    while (y != t_nil && x == y->left) {
        x = y;
        y = y->p;
    }
    return y;
}

/*
     x                             y
    / \                           / \
   y   c   -- right rotate ->    a   x
  / \                               / \
 a   b                             b   c
*/
void bst_right_rotate(struct tree *t, struct tree_node *x) {
    struct tree_node *y = x->left;
    x->left = y->right;
    if (y->right != t_nil)
        y->right->p = x;
    y->p = x->p;
    if (x->p == t_nil)
        t->root = y;
    else if (x == x->p->right)
        x->p->right = y;
    else
        x->p->left = y;
    y->right = x;
    x->p = y;
}

/*
    x                              y
   / \                            / \
  a   y   -- left rotate ->      x   c
     / \                        / \
    b   c                      a   b
*/
void bst_left_rotate(struct tree *t, struct tree_node *x) {
    struct tree_node *y = x->right;
    x->right = y->left;
    if (y->left != t_nil)
        y->left->p = x;
    y->p = x->p;
    if (x->p == t_nil)
        t->root = y;
    else if (x == x->p->left)
        x->p->left = y;
    else
        x->p->right = y;
    y->left = x;
    x->p = y;
}

/* replace u with v */
static void bst_transplant(struct tree *t, struct tree_node *u, struct tree_node *v) {
    if (u->p == t_nil)
        t->root = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
}

struct tree_node *bst_insert(struct tree *t, struct tree_node *z) {
    assert(t);
    assert(z);
    struct tree_node *y = t_nil;
    struct tree_node *x = t->root;
    while (x != t_nil) {
        y = x;
        if (T_KEY_LT(t->key_less, z->key, x->key))
            x = x->left;
        else if (T_KEY_LT(t->key_less, x->key, z->key))
            x = x->right;
        else
            return x;
    }
    z->p = y;
    if (y == t_nil)
        t->root = z;
    else if (T_KEY_LT(t->key_less, z->key, y->key))
        y->left = z;
    else
        y->right = z;
    z->left = t_nil;
    z->right = t_nil;
    return z;
}

void bst_delete(struct tree *t, struct tree_node *z) {
    assert(t);
    assert(z);
    if (z->left == t_nil) {
        bst_transplant(t, z, z->right);
    } else if (z->right == t_nil) {
        bst_transplant(t, z, z->left);
    } else {
        struct tree_node *y = tree_min(t, z->right);
        if (y->p != z) {
            bst_transplant(t, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        bst_transplant(t, z, y);
        y->left = z->left;
        z->left->p = y;
    }
}

static void rb_insert_fixup(struct tree *t, struct tree_node *z) {
    struct tree_node *y = t_nil;
    while (z->p->fea.color == RED) {
        if (z->p == z->p->p->left) {
            // parent is left
            y = z->p->p->right; // y is uncle
            if (y->fea.color == RED) {
                /*
                    case 1 - z's uncle is red (color flips)
                    (lower is red, UPPER IS BLACK)
                         G          *g
                        / \         / \
                       p   y  -->  P   Y
                      /           /
                    *z           z
                */
                z->p->fea.color = BLACK;
                y->fea.color = BLACK;
                z->p->p->fea.color = RED;
                z = z->p->p;
                continue;
            }
            if (z == z->p->right) {
                /*
                    case 2 - z's uncle is black and z is right child (left rotate parent)
                        G           G
                       / \         / \
                      P   U  -->  z   U
                       \         /
                       *z      *p
                    This still leave us in violation of 4, the continuation
                    into case 3 will fix that.
                */
                z = z->p;
                bst_left_rotate(t, z);
            }
            /*
                case 3 - z's uncle is black and z is left child 
                (right rotate grant parent and flip color)
                    G          *P
                   / \         / \
                  P   U  -->  z   g
                 /                 \
               *z                   U
            */
            z->p->fea.color = BLACK;
            z->p->p->fea.color = RED;
            bst_right_rotate(t, z->p->p);
            break;
        } else {
            /* parent is right */
            y = z->p->p->left;
            if (y->fea.color == RED) {
                /* case 1 - color flips */
                z->p->fea.color = BLACK;
                y->fea.color = BLACK;
                z->p->p->fea.color = RED;
                z = z->p->p;
                continue;
            }
            if (z == z->p->left) {
                /* case 2 - right rotate parent */
                z = z->p;
                bst_right_rotate(t, z);
            }
            /* case 3 - left rotate grant parent and flip color */
            z->p->fea.color = BLACK;
            z->p->p->fea.color = RED;
            bst_left_rotate(t, z->p->p);
            break;
        }
    }
    t->root->fea.color = BLACK;
}

static void rb_tree_transplant(struct tree *t, struct tree_node *u, struct tree_node *v) {
    bst_transplant(t, u, v);
}

static void rb_tree_delete_fixup(struct tree *t, struct tree_node *x) {
    struct tree_node *w;
    while (x != t->root && x->fea.color == BLACK) {
        if (x == x->p->left) {
            w = x->p->right;
            if (w->fea.color == RED) {
                /*
                    case 1 - brother is red (left rotate parent)
                         P             W
                        / \           / \
                      *X  ^w    -->  p   Wr
                          / \       / \
                        Xl   Xr   *X  ^Wl
                */
                w->fea.color = BLACK;
                x->p->fea.color = RED;
                bst_left_rotate(t, x->p);
                w = x->p->right;
                continue;
            }
            if (w->left->fea.color == BLACK && w->right->fea.color == BLACK) {
                /*
                    case 2 - brother is BLACK, and nephews are BLACK (brother color flip)
                    (P could be either color here)
                        (p)           *(P)
                        / \            / \
                      *X   W    -->   X   w
                          / \            / \
                         Wl  Wr         Wl  Wr
                */
                w->fea.color = RED;
                x = x->p;
                continue;
            } else if (w->right->fea.color == BLACK) {
                /*
                    case 3 - brother is BLACK, brother->left is red, brother->right is BLACK
                    (right rotate at brother, and color flip)
                    (p could be either color here)
                        (p)             (p)
                        / \             / \
                      *X   W    -->   *X   Wl
                          / \               \
                         wl  Wr              w
                                              \
                                               wr
                */ 
               w->left->fea.color = BLACK;
               w->fea.color = RED;
               bst_right_rotate(t, w);
               w = x->p->right;
            }
            /*
                case 4 - brother is BLACK, brother->right is red
                (left rotate at parent and color flips)
                (p and wl be either color, after rotate, p become black,
                 w acquires p's color, wl keeps its color)
                    (p)           (w)
                    / \           / \
                  *X   W    -->  P   Wr
                      / \       / \
                    (wl) wr    X  (wl)
            */
            w->fea.color = x->p->fea.color;
            x->p->fea.color = BLACK;
            w->right->fea.color = BLACK;
            bst_left_rotate(t, x->p);
            x = t->root;
        } else {
            w = x->p->left;
            if (w->fea.color == RED) {
                w->fea.color = BLACK;
                x->p->fea.color = RED;
                bst_right_rotate(t, x->p);
                w = x->p->left;
                continue;
            }
            if (w->right->fea.color == BLACK && w->left->fea.color == BLACK) {
                w->fea.color = RED;
                x = x->p;
                continue;
            } else if (w->left->fea.color == BLACK) {
               w->right->fea.color = BLACK;
               w->fea.color = RED;
               bst_left_rotate(t, w);
               w = x->p->left;
            }
            w->fea.color = x->p->fea.color;
            x->p->fea.color = BLACK;
            w->left->fea.color = BLACK;
            bst_right_rotate(t, x->p);
            x = t->root;
        }
    }
    x->fea.color = BLACK;
}

void rb_tree_delete(struct tree *t, struct tree_node *z) {
    assert(t);
    assert(z);
    struct tree_node *x = t_nil;
    struct tree_node *y = z;
    enum rb_color y_origin_color = y->fea.color;
    if (z->left == t_nil) {
        x = z->right;
        rb_tree_transplant(t, z, z->right);
    } else if (z->right == t_nil) {
        x = z->left;
        rb_tree_transplant(t, z, z->left);
    } else {
        y = tree_min(t, z->right);
        y_origin_color = y->fea.color;
        x = y->right;
        if (y->p == z) {
            x->p = y;
        } else {
            rb_tree_transplant(t, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        rb_tree_transplant(t, z, y);
        y->left = z->left;
        y->left->p = y;
        y->fea.color = z->fea.color;
    }
    if (y_origin_color == BLACK)
        rb_tree_delete_fixup(t, x);
}

struct tree_node *rb_tree_insert(struct tree *t, struct tree_node *z) {
    struct tree_node *zz = bst_insert(t, z);
    if (zz != z)
        return zz;
    z->fea.color = RED;
    rb_insert_fixup(t, z);
    return z;
}

static void treap_balance_up(struct tree *t, struct tree_node *z) {
    while (z != t->root && T_KEY_LT(t->priority_less, z->p->fea.priority, z->fea.priority)) {
        if (z->p->left == z) {
            bst_right_rotate(t, z->p);
        } else {
            bst_left_rotate(t, z->p);
        }
    }
}

static int treap_pri_less(struct tree *t, struct tree_node *a, struct tree_node *b) {
    if (a == t_nil && b == t_nil) {
        return 0;
    }
    if (a == t_nil) {
        return 1;
    }
    if (b == t_nil) {
        return 0;
    }
    return T_KEY_LT(t->priority_less, a->fea.priority, b->fea.priority);
}

struct tree_node *treap_insert(struct tree *t, struct tree_node *z) {
    assert(t);
    assert(z);
    struct tree_node *zz = bst_insert(t, z);
    if (zz != z)
        return zz;
    treap_balance_up(t, z);
    return z;
}

void treap_delete(struct tree *t, struct tree_node *z) {
    assert(t);
    assert(z);
    while (z->left != t_nil || z->right != t_nil) {
        if (z->left == t_nil) {
            bst_transplant(t, z, z->right);
            return;
        }
        if (z->right == t_nil) {
            bst_transplant(t, z, z->left);
            return;
        }

        if (treap_pri_less(t, z->left, z->right))
            bst_right_rotate(t, z);
        else 
            bst_left_rotate(t, z);

    }
    if (z == t->root)
        t->root = t_nil;
    else if (z->p->left == z)
        z->p->left = t_nil;
    else
        z->p->right = t_nil;
}

static void update_height(struct tree_node *x) {
    x->fea.height = max(x->left->fea.height, x->right->fea.height) + 1;
}

static void avl_right_rotate(struct tree *t, struct tree_node *x) {
    bst_right_rotate(t, x);
    update_height(x);
    update_height(x->p);
}

static void avl_left_rotate(struct tree *t, struct tree_node *x) {
    bst_left_rotate(t, x);
    update_height(x);
    update_height(x->p);
}

static int avl_is_balance(struct tree_node *x) {
    int h = x->left->fea.height - x->right->fea.height;
    if (h >= -1 && h <= 1)
        return 1;
    return 0;
}

static void avl_rebalance(struct tree *t, struct tree_node *x, struct tree_node *y, struct tree_node *z) {
   if (x == y->left && y == z->left) {
       /*
        left left case
            z                                      y 
           / \                                   /   \
          y   T4      Right Rotate (z)          x      z
         / \          - - - - - - - - ->      /  \    /  \ 
        x   T3                               T1  T2  T3  T4
       / \
      T1  T2
       */
       avl_right_rotate(t, z);
   } else if (x == y->right && y == z->left) {
       /*
        left right case
          z                               z                           x
         / \                            /   \                        /  \ 
        y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
       / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
      T1  x                          y    T3                     T1  T2 T3  T4
          / \                       / \
        T2   T3                   T1   T2
       */
       avl_left_rotate(t, y);
       avl_right_rotate(t, z);
   } else if (x == y->right && y == z->right) {
       /*
        right right case
                 z                                y
                /  \                            /   \ 
               T1   y     Left Rotate(z)       z      x
                   /  \   - - - - - - - ->    / \    / \
                  T2   x                     T1  T2 T3  T4
                      / \
                     T3  T4
       */
       avl_left_rotate(t, z);
   } else if (x == y->left && y == z->right) {
       /*
        right left case
             z                            z                            x
            / \                          / \                          /  \ 
          T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      x
              / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
             x   T4                      T2   y                  T1  T2  T3  T4
            / \                              /  \
          T2   T3                           T3   T4
       */
       avl_right_rotate(t, y);
       avl_left_rotate(t, z);
   }
}

void avl_insert_fixup(struct tree *t, struct tree_node *w) {
    struct tree_node *x, *y, *z;
    x = y = z = w;
    while (z != t_nil && y != t->root) {
        update_height(z);
        if (!avl_is_balance(z))
            avl_rebalance(t, x, y, z);
        x = y;
        y = z;
        z = z->p;
    }
}

struct tree_node *avl_insert(struct tree *t, struct tree_node *z) {
    assert(t);
    assert(z);
    z->fea.height = 1;
    struct tree_node *zz = bst_insert(t, z);
    if (zz != z)
        return zz;
    avl_insert_fixup(t, z);
    return z;
}

static void avl_delete_fixup(struct tree *t, struct tree_node *w) {
    while (w != t_nil) {
        update_height(w);
        w = w->p;
    }
}

void avl_delete(struct tree *t, struct tree_node *z) {
    assert(t);
    assert(z);
    while (z->left != t_nil || z->right != t_nil) {
        if (z->left->fea.height < z->right->fea.height)
            avl_left_rotate(t, z);
        else
            avl_right_rotate(t, z);
    }
    if (z == t->root) {
        t->root = t_nil;
    } else {
        if (z == z->p->left)
            z->p->left = t_nil;
        else
            z->p->right = t_nil;
        avl_delete_fixup(t, z->p);
    }
}
