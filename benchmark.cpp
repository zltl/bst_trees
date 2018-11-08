#include <stdio.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <set>
#include <random>
#include <vector>
#include <chrono>

#include "trees.h"

using std::string;
using std::cout;
using std::endl;
using std::set;
using std::vector;
using namespace std::chrono;

std::random_device rd;
std::mt19937 rng(rd());
const char charset[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
const size_t max_index = (sizeof(charset) - 1);
std::uniform_int_distribution<int> uni(0,max_index - 1);

string random_string(size_t length) {
    auto randchar = []() -> char {
        return charset[uni(rng)];
    };
    string str(length, 0);
    for (size_t i = 0; i < length; i++) {
        str[i] = randchar();
    }
    return str;
}

auto random_keys(size_t length) -> vector<string> {
    set<string> s;
    while (length--) {
        s.insert(random_string(10));
    }
    vector<string> v(s.begin(), s.end());
    std::random_shuffle(v.begin(), v.end());
    return v;
}

struct tree_node* new_node(string key) {
    struct tree_node* n = (struct tree_node*)malloc(sizeof(struct tree_node));
    assert(n);
    n->p = n->left = n->right = t_nil;
    n->key = static_cast<void*>(new string(key));
    return n;
}
void free_node(struct tree_node* n) { 
    delete static_cast<string*>(n->key);
    free(n);
}
struct tree_node *new_treap_node(string key, string pri) {
    struct tree_node* n = (struct tree_node*)malloc(sizeof(struct tree_node));
    assert(n);
    n->p = n->left = n->right = t_nil;
    n->key = static_cast<void*>(new string(key));
    n->fea.priority = static_cast<void*>(new string(pri));
    return n;
}
void free_treap_node(struct tree_node* n) {
    delete static_cast<string*>(n->key);
    delete static_cast<string*>(n->fea.priority);
    free(n);
}
int less(void* a, void* b) { 
    return *static_cast<string*>(a) < *static_cast<string*>(b);
}
void try_find(struct tree* t, int key) {
    struct tree_node* n = tree_search(t, (void*)(long)key);
    cout << "find " << key << ": ";
    if (n == t_nil) {
        cout << "OK" << endl;
    } else {
        cout << "NOT FOUND" << endl;
    }
}

void bench_bst(const vector<string> &keys) {
    struct tree t = T_INITIAL;
    t.type = T_BST;
    t.key_less = less;
    int height = 0;

    auto start = high_resolution_clock::now();
    for (auto key: keys) {
        bst_insert(&t, new_node(key));
    }
    auto end = high_resolution_clock::now();
    height = tree_height(&t, t.root);
    cout << "bst height: " << height << endl;
    cout << "bst insert:" << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << endl;

    start = high_resolution_clock::now();
    for (auto key: keys) {
        tree_search(&t, static_cast<void*>(&key));
    }
    end = high_resolution_clock::now();
    cout << "bst search " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << endl;

    start = high_resolution_clock::now();
    for (auto key: keys) {
        auto x = tree_search(&t, static_cast<void*>(&key));
        bst_delete(&t, x);
        free_node(x);
    }
    end = high_resolution_clock::now();
    cout << "bst search delete " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << endl;
}

void bench_rb(const vector<string> &keys) {
    struct tree t = T_INITIAL;
    t.type = T_RB;
    t.key_less = less;
    int height = 0;

    auto start = high_resolution_clock::now();
    for (auto key: keys) {
        rb_tree_insert(&t, new_node(key));
    }
    auto end = high_resolution_clock::now();
    height = tree_height(&t, t.root);
    cout << "rb height: " << height << endl;
    cout << "rb insert:" << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << endl;

    start = high_resolution_clock::now();
    for (auto key: keys) {
        tree_search(&t, static_cast<void*>(&key));
    }
    end = high_resolution_clock::now();
    cout << "rb search " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << endl;

    start = high_resolution_clock::now();
    for (auto key: keys) {
        auto x = tree_search(&t, static_cast<void*>(&key));
        rb_tree_delete(&t, x);
        free_node(x);
    }
    end = high_resolution_clock::now();
    cout << "rb search delete " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << endl;
}

int main() {
    auto r = random_keys(int(2));
    cout << "key set: " << r.size() << endl;
    bench_bst(r);
    bench_rb(r);
}
