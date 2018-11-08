#include <stdio.h>
#include <malloc.h>

#include "trees.h"

struct tree_node *x_new_node(int key) {
    struct tree_node *n = (struct tree_node *)malloc(sizeof(struct tree_node));
    assert(n);
    n->p = n->left = n->right = t_nil;
    n->key = (void *)(long)key;
    return n;
}
void x_free_node(struct tree_node *n) {
    free(n);
}
int x_less(void *a, void *b) {
    return (long)a < (long)b;
}
void try_find(struct tree *t, int key) {
    struct tree_node *n = tree_search(t, (void *)(long)key);
    if (n == t_nil) {
        printf("not fould key: %d\n", key);
    } else {
        printf("find key: %ld, color is %d\n", (long)n->key, n->fea.color);
    }
}
int main() {
    struct tree_node *a, *b, *c, *d, *e, *f;
    a = x_new_node(1);
    b = x_new_node(6);
    c = x_new_node(4);
    d = x_new_node(8);
    e = x_new_node(5);
    f = x_new_node(3);

    struct tree t = T_INITIAL;
    t.type = T_RB;
    t.key_less = x_less;

    rb_tree_insert(&t, a);
    rb_tree_insert(&t, b);
    rb_tree_insert(&t, c);
    rb_tree_insert(&t, d);
    rb_tree_insert(&t, e);
    rb_tree_insert(&t, f);

    int i;
    for (i = 0; i < 10; i++) {
        try_find(&t, i);
    }

    struct tree_node *x = tree_min(&t, t.root);
    if (x != t_nil) {
        printf("min=%ld\n", (long)x->key);
    } else {
        printf("min empty\n");
    }

    x = tree_max(&t, t.root);
    if (x != t_nil) {
        printf("max=%ld\n", (long)x->key);
    } else {
        printf("max empty\n");
    }

    printf("delete 4\n");
    rb_tree_delete(&t, c);
    x_free_node(c);
    for (i = 0; i < 10; i++) {
        try_find(&t, i);
    }
}
