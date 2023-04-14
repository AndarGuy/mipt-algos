/*

Быстрый обмен

Имеется массив A индексируемый от единицы из 109 натуральных чисел, Ai=i.

Над этим массивом выполняется 1 ⩽ N ⩽ 500000 запросов swap(x,y), каждый из
которых меняет меcтами значения x и y, возвращая в качестве результата
расстояния между этими числами.

Например, после первой операции swap(1,3) массив будет выглядеть так:
3,2,1,4,5,.... Результатом операции будет 2.

Операция swap(3,5) превратит его в 5,2,1,4,3 и её результатом будет 4.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SIZE 4
#define VALUE_SIZE 4

typedef struct _tree_vertex {
    struct _tree_vertex *left, *right;
    int key;
    int value;
} TreeVertex;

enum TreeWalkType { PRE_ORDER, IN_ORDER, OUT_ORDER };

TreeVertex *new_vertex(int key, int value) {
    TreeVertex *vertex = malloc(sizeof(TreeVertex));
    vertex->left = vertex->right = NULL;
    vertex->key = key;
    vertex->value = value;
    return vertex;
}

TreeVertex *tree_vertex_insert(TreeVertex *top, int key, int value) {
    if (top == NULL) return new_vertex(key, value);

    int result = top->key - key;
    if (result > 0) {
        top->left = tree_vertex_insert(top->left, key, value);
    } else if (result < 0) {
        top->right = tree_vertex_insert(top->right, key, value);
    } else {
        top->value = value;
    }

    return top;
}

void tree_vertex_walk(TreeVertex *top, void (*work)(TreeVertex *),
                      enum TreeWalkType type) {
    if (type == PRE_ORDER) work(top);
    if (top->left != NULL) tree_vertex_walk(top->left, work, type);
    if (type == IN_ORDER) work(top);
    if (top->right != NULL) tree_vertex_walk(top->right, work, type);
    if (type == OUT_ORDER) work(top);
}

void tree_vertex_destroy(TreeVertex *vertex) {
    if (!vertex) return;
    free(vertex);
}

TreeVertex *tree_destroy(TreeVertex *tree) {
    if (!tree) return NULL;
    tree_vertex_walk(tree, tree_vertex_destroy, OUT_ORDER);
    return NULL;
}

TreeVertex *tree_vertex_find(TreeVertex *vertex, int key) {
    if (!vertex) return NULL;

    int result = vertex->key - key;

    if (result < 0) {
        return tree_vertex_find(vertex->right, key);
    } else if (result > 0) {
        return tree_vertex_find(vertex->left, key);
    } else
        return vertex;
}

TreeVertex *tree_get_min(TreeVertex *vertex) {
    if (!vertex) return NULL;
    if (vertex->left) return tree_get_min(vertex->left);
    return vertex;
}

// ----------------------------------- //

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);

    TreeVertex *tree = NULL;
    int keyA, keyB;
    for (size_t i = 0; i < N; i++) {
        scanf("%d%d", &keyA, &keyB);
        TreeVertex *a = tree_vertex_find(tree, keyA),
                   *b = tree_vertex_find(tree, keyB);
        if (!a && !b) {
            tree = tree_vertex_insert(tree, keyA, keyB);
            tree = tree_vertex_insert(tree, keyB, keyA);
            printf("%d\n", abs(keyA - keyB));
        } else if (!a) {
            int tempB = b->value;
            b->value = keyA;
            tree = tree_vertex_insert(tree, keyA, tempB);
            printf("%d\n", abs(keyA - tempB));
        } else if (!b) {
            int tempA = a->value;
            a->value = keyB;
            tree = tree_vertex_insert(tree, keyB, tempA);
            printf("%d\n", abs(keyB - tempA));
        } else {
            int tempA = a->value, tempB = b->value;
            a->value = tempB;
            b->value = tempA;
            printf("%d\n", abs(tempA - tempB));
        }
    }

    tree_destroy(tree);

    return 0;
}