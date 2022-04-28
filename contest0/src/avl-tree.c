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

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(A, B) A > B ? A : B
#define STRING_SIZE 21

typedef struct _tree_vertex {
    struct _tree_vertex *left, *right;
    size_t height;
    char *key;
    char *value;
} TreeVertex;

enum TreeWalkType { PRE_ORDER, IN_ORDER, OUT_ORDER };

TreeVertex *new_vertex(char *key, char *value) {
    TreeVertex *vertex = malloc(sizeof(TreeVertex));
    vertex->left = vertex->right = NULL;
    vertex->key = malloc(STRING_SIZE);
    memcpy(vertex->key, key, STRING_SIZE);
    vertex->value = malloc(STRING_SIZE);
    memcpy(vertex->value, value, STRING_SIZE);
    vertex->height = 1;
    return vertex;
}

int tree_delta(TreeVertex *vertex) {
    if (!vertex) return 0;
    int a = vertex->left ? vertex->left->height : 0;
    int b = vertex->right ? vertex->right->height : 0;
    return a - b;
}

int tree_vertex_height(TreeVertex *vertex) {
    int a = vertex->left ? vertex->left->height : 0;
    int b = vertex->right ? vertex->right->height : 0;
    return (a > b ? a : b) + 1;
}

void tree_height_recalculate(TreeVertex *vertex) {
    vertex->height = tree_vertex_height(vertex);
}

TreeVertex *tree_vertex_rotate_right(TreeVertex *vertex) {
    TreeVertex *p = vertex, *x = vertex->left;
    p->left = x->right;
    x->right = p;
    tree_height_recalculate(p);
    tree_height_recalculate(x);
    return x;
}

TreeVertex *tree_vertex_rotate_left(TreeVertex *vertex) {
    TreeVertex *p = vertex, *x = vertex->right;
    p->right = x->left;
    x->left = p;
    tree_height_recalculate(p);
    tree_height_recalculate(x);
    return x;
}

void tree_print(TreeVertex *vertex, int space, int margin) {
    if (vertex == NULL) return;

    space += margin;

    tree_print(vertex->right, space, margin);

    for (int i = margin; i < space; i++) printf(" ");
    printf("[%s, %s] (H=%zu)", vertex->key, vertex->value, vertex->height);
    printf("\n");

    tree_print(vertex->left, space, margin);
}

void tree_vertex_print(TreeVertex *vertex) {
    printf("--------------------------------\n");
    tree_print(vertex, 0, 4);
    printf("--------------------------------\n");
}

TreeVertex *tree_balance(TreeVertex *vertex) {
    tree_height_recalculate(vertex);
    switch (tree_delta(vertex)) {
        case 2:
            if (tree_delta(vertex->left) < 0)
                vertex->left = tree_vertex_rotate_left(vertex->left);
            return tree_vertex_rotate_right(vertex);
        case -2:
            if (tree_delta(vertex->right) > 0)
                vertex->right = tree_vertex_rotate_right(vertex->right);
            return tree_vertex_rotate_left(vertex);
        default:
            return vertex;
    }
}

TreeVertex *tree_vertex_insert(TreeVertex *top, char *key, char *value) {
    if (top == NULL) return new_vertex(key, value);

    int result = strcmp(top->key, key);
    if (result > 0) {
        top->left = tree_vertex_insert(top->left, key, value);
    } else if (result < 0) {
        top->right = tree_vertex_insert(top->right, key, value);
    } else {
        memcpy(top->value, value, STRING_SIZE);
    }

    return tree_balance(top);
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
    free(vertex->key);
    free(vertex->value);
    free(vertex);
}

TreeVertex *tree_destroy(TreeVertex *tree) {
    if (!tree) return NULL;
    tree_vertex_walk(tree, tree_vertex_destroy, OUT_ORDER);
    return NULL;
}

TreeVertex *tree_vertex_find(TreeVertex *vertex, char *key) {
    if (!vertex) return NULL;

    int result = strcmp(vertex->key, key);

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

TreeVertex *tree_delete_by_key(TreeVertex *top, char *key) {
    if (!top) return NULL;
    int result = strcmp(top->key, key);
    if (result < 0)
        top->right = tree_delete_by_key(top->right, key);
    else if (result > 0)
        top->left = tree_delete_by_key(top->left, key);
    else {
        if (!top->right) {
            TreeVertex *temp = top->left;
            tree_vertex_destroy(top);
            return temp;
        }
        TreeVertex *min = tree_get_min(top->right);
        strcpy(top->key, min->key);
        strcpy(top->value, min->value);
        top->right = tree_delete_by_key(top->right, min->key);
    }
    return tree_balance(top);
}

int main(int argc, char *argv[]) {
    TreeVertex *tree = NULL;

    char command[10 + 1], key[20 + 1], value[20 + 1];
    while (scanf("%s", command) > 0) {
        if (strcmp(command, "insert") == 0) {
            scanf("%s", key);
            scanf("%s", value);
            tree = tree_vertex_insert(tree, key, value);
            // tree_vertex_print(tree);
        } else if (strcmp(command, "erase") == 0) {
            scanf("%s", key);
            tree = tree_delete_by_key(tree, key);
            // tree_vertex_print(tree);
        } else if (strcmp(command, "find") == 0) {
            scanf("%s", key);
            TreeVertex *result = tree_vertex_find(tree, key);
            if (!result)
                printf("MISSING\n");
            else
                printf("%s\n", result->value);
        } else if (strcmp(command, "print") == 0) {
            tree_vertex_print(tree);
        }
    }

    tree_destroy(tree);
    return 0;
}