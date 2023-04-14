/*

Множество множеств

Реализуйте структуру данных для хранения множества множеств.
Должны храниться N+1 множеств натуральных чисел. Множества пронумерованы от 0 до
N включительно. Сами числа строго положительные и могут быть представлены в long
long. Каждое число может храниться сразу в нескольких множествах. Вначале все
множества пустые.

Требуемые операции над этой структурой:

• + x s – добавить число x во множество s

• - x s – удалить число x из множества s. Если такого числа в множестве не было
– ничего не происходит.

• = s – опустошить множество s.

• ? s – вывести на стандартный вывод все элементы множества в порядке
возрастания. Если множество пусто – вывести -1.

• ?? x – вывести на стандартный вывод номера всех множеств, в которых содержится
x в порядке возрастания. Если числа x нет ни в одном из множеств – вывести -1.

*/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _tree_vertex {
    struct _tree_vertex *left, *right;
    size_t height;
    long long int key;
} TreeVertex;

enum TreeWalkType { PRE_ORDER, IN_ORDER, OUT_ORDER };

TreeVertex *new_vertex(long long int key) {
    TreeVertex *vertex = malloc(sizeof(TreeVertex));
    vertex->left = vertex->right = NULL;
    vertex->key = key;
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

TreeVertex *tree_vertex_insert(TreeVertex *top, long long int key) {
    if (top == NULL) return new_vertex(key);

    long long int result = top->key - key;
    if (result > 0) {
        top->left = tree_vertex_insert(top->left, key);
    } else if (result < 0) {
        top->right = tree_vertex_insert(top->right, key);
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
    free(vertex);
}

TreeVertex *tree_destroy(TreeVertex *tree) {
    if (!tree) return NULL;
    tree_vertex_walk(tree, tree_vertex_destroy, OUT_ORDER);
    return NULL;
}

TreeVertex *tree_vertex_find(TreeVertex *vertex, long long int key) {
    if (!vertex) return NULL;

    long long int result = vertex->key - key;

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

TreeVertex *tree_delete_by_key(TreeVertex *top, long long int key) {
    if (!top) return NULL;
    long long int result = top->key - key;
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
        top->key = min->key;
        top->right = tree_delete_by_key(top->right, min->key);
    }
    return tree_balance(top);
}

// ----------------------------------------------- //

void print_data(TreeVertex *a) { printf("%lld ", a->key); }

#define MAX_LENGHT 21

int main(int argc, char *argv[]) {
    int N, M;
    scanf("%d%d", &N, &M);
    N++;

    TreeVertex **forest = calloc(N, sizeof(TreeVertex *));

    char command[3];
    for (size_t i = 0; i < M; i++) {
        scanf("%s", command);
        long long x;
        int tree;
        if (!strcmp(command, "+")) {
            scanf("%lld", &x);
            scanf("%d", &tree);
            forest[tree] = tree_vertex_insert(forest[tree], x);
        } else if (!strcmp(command, "-")) {
            scanf("%lld", &x);
            scanf("%d", &tree);
            forest[tree] = tree_delete_by_key(forest[tree], x);
        } else if (!strcmp(command, "=")) {
            scanf("%d", &tree);
            forest[tree] = tree_destroy(forest[tree]);
        } else if (!strcmp(command, "?")) {
            scanf("%d", &tree);
            if (!forest[tree])
                printf("-1");
            else
                tree_vertex_walk(forest[tree], print_data, IN_ORDER);
            printf("\n");
        } else if (!strcmp(command, "??")) {
            scanf("%lld", &x);
            char found = 0;
            for (size_t i = 0; i < N; i++) {
                if (tree_vertex_find(forest[i], x) > 0) {
                    printf("%zu ", i);
                    found++;
                }
            }
            if (!found) printf("-1");
            printf("\n");
        } else
            continue;
    }

    for (size_t i = 0; i < N; i++) {
        if (forest[i]) forest[i] = tree_destroy(forest[i]);
    }
    free(forest);

    return 0;
}