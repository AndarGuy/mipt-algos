/*

AVL-дерево

Реализуйте структуру данных AVL-дерево. Она должна по заданному ключу находить
значение, которое этому ключу соответствует.

• Ключ и значение таблицы — строка из строчных букв латинского алфавита не более
20 символов.

• В каждой строке стандартного ввода находится одна из трёх возможных команд:

• insert key value — Ключу key ставится в соответствие значение value. Для уже
существующего ключа значение должно поменяться на новое. • erase key — убрать
запись key из дерева. • find key — вывести на стандартный вывод значение по
ключу или слово MISSING, если ключ не существует.

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

void _tree_walk(TreeVertex *top, void (*work)(TreeVertex *),
                enum TreeWalkType type) {
    if (type == PRE_ORDER) work(top);
    if (top->children[0] != NULL) _tree_walk(top->children[0], work, type);
    if (type == IN_ORDER) work(top);
    if (top->children[1] != NULL) _tree_walk(top->children[1], work, type);
    if (type == OUT_ORDER) work(top);
}

void tree_walk(Tree *tree, void (*work)(TreeVertex *), enum TreeWalkType type) {
    _tree_walk(tree->top, work, type);
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

size_t _tree_get_depth(TreeVertex *vertex) {
    size_t left = 0, right = 0;
    if (vertex->children[0] != NULL)
        left = _tree_get_depth(vertex->children[0]);
    if (vertex->children[1] != NULL)
        right = _tree_get_depth(vertex->children[1]);
    return (left > right ? left : right) + 1;
}

size_t tree_get_depth(Tree *tree) {
    if (tree->top == NULL) return 0;
    return _tree_get_depth(tree->top);
}

void _tree_print_level(TreeVertex *vertex, void (*print_data)(void *data),
                       size_t level, size_t current) {
    if (level == current) {
        print_data(vertex->data);
        return;
    } else if (level > current) {
        if (vertex->children[0] != NULL)
            _tree_print_level(vertex->children[0], print_data, level,
                              current + 1);
        if (vertex->children[1] != NULL)
            _tree_print_level(vertex->children[1], print_data, level,
                              current + 1);
    }
}

void tree_print_levels(Tree *tree, void (*print_data)(void *data)) {
    size_t depth = tree_get_depth(tree);
    for (size_t i = 0; i < depth; i++) {
        printf("level %zu: ", i);
        _tree_print_level(tree->top, print_data, i, 0);
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
    Tree *tree = tree_create(sizeof(int), comparator);
    int values[] = {10, 5, 35, 3, 7, 23, 94, 2, 5, 7};
    for (size_t i = 0; i < 10; i++) {
        tree_insert(tree, values + i);
    }
    tree_walk(tree, print_data, PRE_ORDER);

    printf("%d\n", *((int *)tree_find(tree, values + 8)));

    size_t depth = tree_get_depth(tree);
    printf("%zu\n", depth);

    tree_print_levels(tree, print_int);

    tree = tree_destroy(tree);
    return 0;
}