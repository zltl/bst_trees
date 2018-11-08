#include <stdio.h>
#include <malloc.h>

#include "trees.h"

struct tree_node *x_new_node(int key, int pri) {
    struct tree_node *n = (struct tree_node *)malloc(sizeof(struct tree_node));
    assert(n);
    n->p = n->left = n->right = t_nil;
    n->key = (void *)(long)key;
    n->fea.priority = (void *)(long)pri;
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
        printf("find key: %ld, priority is %d\n", (long)n->key, (long)n->fea.priority);
    }
}
int main() {
    struct tree_node *a, *b, *c, *d, *e, *f;
    a = x_new_node(1, 345);
    b = x_new_node(6, 232);
    c = x_new_node(4, 765);
    d = x_new_node(8, 1234);
    e = x_new_node(5, 55);
    f = x_new_node(3, 4622);

    struct tree t = T_INITIAL;
    t.type = T_TREAP;
    t.key_less = x_less;
    t.priority_less = x_less;

    treap_insert(&t, a);
    treap_insert(&t, b);
    treap_insert(&t, c);
    treap_insert(&t, d);
    treap_insert(&t, e);
    treap_insert(&t, f);

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
    treap_delete(&t, c);
    x_free_node(c);
    for (i = 0; i < 10; i++) {
        try_find(&t, i);
    }
}
