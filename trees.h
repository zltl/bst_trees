#include <stdio.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

enum rb_color {
    BLACK, RED
};

enum rb_tree_type {
    T_BST,T_RB,T_AVL,T_TREAP
};

struct tree_node {
    struct tree_node *p, *left, *right;
    union {
        enum rb_color color;
        int height;
        void *priority;
    } fea;
    void *key;
    void *data;    
};

struct tree {
    int (*key_less)(void *key1, void *key2);
    int (*priority_less)(void *key1, void *key2);
    struct tree_node *root;
    enum rb_tree_type type;
};

extern struct tree_node t_null_node;
extern struct tree_node *t_nil;

#define T_INITIAL {NULL, NULL, t_nil, T_BST}
#define N_INITIAL {t_nil, t_nil, t_nil, {RED}, t_nil, t_nil}

int tree_height(struct tree *t, struct tree_node *x);
struct tree_node *tree_search(struct tree *t, void *key);
struct tree_node *tree_min(struct tree *t, struct tree_node *r);
struct tree_node *tree_max(struct tree *t, struct tree_node *r);
struct tree_node *tree_successor(struct tree *t, struct tree_node *x);
struct tree_node *tree_predecessor(struct tree *t, struct tree_node *x);
void bst_right_rotate(struct tree *t, struct tree_node *x);
void bst_left_rotate(struct tree *t, struct tree_node *x);
struct tree_node *bst_insert(struct tree *t, struct tree_node *z);
void bst_delete(struct tree *t, struct tree_node *z);
void rb_tree_delete(struct tree *t, struct tree_node *z);
struct tree_node *rb_tree_insert(struct tree *t, struct tree_node *z);
void treap_delete(struct tree *t, struct tree_node *z);
struct tree_node *treap_insert(struct tree *t, struct tree_node *z);
void avl_delete(struct tree *t, struct tree_node *z);
struct tree_node *avl_insert(struct tree *t, struct tree_node *z);

#ifdef __cplusplus
}
#endif
