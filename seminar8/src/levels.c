/*

Слои дерева поиска

Постройте дерево простого поиска для целочисленных ключей. Дерево может
содержать равные ключи, в этом случае при при добавлении такой ключ добавляйте в
правое поддерево.

Кроме операции добавления реализуйте операцию вывода значений всех вершин дерева
с указанием их расстояния до корня. Обратите внимание: при этом требуется
значения всех вершин, находящихся на одном расстоянии от корня, вывести в одну
строку через пробел, а перед этим указать значение этого расстояния. Подробности
см. в примерах.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _tree_vertex {
    struct _tree_vertex *children[2];
    void *data;
} TreeVertex;

typedef struct _tree {
    struct _tree_vertex *top;
    size_t data_size;
    int (*comparator)(const void *, const void *);
} Tree;

enum TreeWalkType { PRE_ORDER, IN_ORDER, OUT_ORDER };

Tree *tree_create(size_t data_size,
                  int (*comparator)(const void *, const void *)) {
    Tree *tree = malloc(sizeof(Tree));
    tree->top = NULL;
    tree->data_size = data_size;
    tree->comparator = comparator;
    return tree;
}

TreeVertex *new_vertex(size_t data_size, void *data) {
    TreeVertex *vertex = malloc(sizeof(TreeVertex));
    vertex->children[0] = vertex->children[1] = NULL;
    vertex->data = malloc(data_size);
    memcpy(vertex->data, data, data_size);
    return vertex;
}

void _tree_insert(Tree *tree, TreeVertex *top, void *data) {
    if (tree->comparator(top->data, data) > 0) {
        if (top->children[0] == NULL)
            top->children[0] = new_vertex(tree->data_size, data);
        else
            _tree_insert(tree, top->children[0], data);
    } else {
        if (top->children[1] == NULL)
            top->children[1] = new_vertex(tree->data_size, data);
        else
            _tree_insert(tree, top->children[1], data);
    }
}

void tree_insert(Tree *tree, void *data) {
    TreeVertex *current = tree->top;
    if (current == NULL)
        tree->top = new_vertex(tree->data_size, data);
    else
        _tree_insert(tree, current, data);
}

void tree_walk(TreeVertex *top, void (*work)(TreeVertex *),
                enum TreeWalkType type) {
    if (type == PRE_ORDER) work(top);
    if (top->children[0] != NULL) tree_walk(top->children[0], work, type);
    if (type == IN_ORDER) work(top);
    if (top->children[1] != NULL) tree_walk(top->children[1], work, type);
    if (type == OUT_ORDER) work(top);
}

void tree_walk(Tree *tree, void (*work)(TreeVertex *), enum TreeWalkType type) {
    tree_walk(tree->top, work, type);
}

void tree_vertex_destroy(TreeVertex *vertex) {
    free(vertex->data);
    free(vertex);
}

Tree *tree_destroy(Tree *tree) {
    tree_walk(tree, tree_vertex_destroy, OUT_ORDER);
    free(tree);
    return NULL;
}

void *_tree_find(Tree *tree, TreeVertex *top, void *data) {
    if (top == NULL) return NULL;
    int result = tree->comparator(top->data, data);
    if (result > 0) return _tree_find(tree, top->children[0], data);
    if (result < 0) return _tree_find(tree, top->children[1], data);
    return top->data;
}

void *tree_find(Tree *tree, void *data) {
    return _tree_find(tree, tree->top, data);
}

size_t tree_vertex_get_depth(TreeVertex *vertex) {
    size_t left = 0, right = 0;
    if (vertex->children[0] != NULL)
        left = tree_vertex_get_depth(vertex->children[0]);
    if (vertex->children[1] != NULL)
        right = tree_vertex_get_depth(vertex->children[1]);
    return (left > right ? left : right) + 1;
}

size_t tree_get_depth(Tree *tree) {
    if (tree->top == NULL) return 0;
    return tree_vertex_get_depth(tree->top);
}

void tree_vertex_print_level(TreeVertex *vertex, void (*print_data)(void *data),
                       size_t level, size_t current) {
    if (level == current) {
        print_data(vertex->data);
        return;
    } else if (level > current) {
        if (vertex->children[0] != NULL)
            tree_vertex_print_level(vertex->children[0], print_data, level,
                              current + 1);
        if (vertex->children[1] != NULL)
            tree_vertex_print_level(vertex->children[1], print_data, level,
                              current + 1);
    }
}

void tree_print_levels(Tree *tree, void (*print_data)(void *data)) {
    size_t depth = tree_get_depth(tree);
    for (size_t i = 0; i < depth; i++) {
        printf("level %zu: ", i);
        tree_vertex_print_level(tree->top, print_data, i, 0);
        printf("\n");
    }
}

int comparator(const void *a, const void *b) {
    if (*((int *)a) > *((int *)b))
        return 1;
    else if (*((int *)a) < *((int *)b))
        return -1;
    else
        return 0;
}

void print_data(TreeVertex *vertex) {
    printf("t[%p]=’%d’\n", vertex, *((int *)vertex->data));
}

void print_int(void *a) { printf("%d ", *((int *)a)); }

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);

    Tree *tree = tree_create(sizeof(int), comparator);

    int temp;
    for (size_t i = 0; i < N; i++) {
        scanf("%d", &temp);
        tree_insert(tree, &temp);
    }

    tree_print_levels(tree, print_int);

    tree = tree_destroy(tree);

    return 0;
}