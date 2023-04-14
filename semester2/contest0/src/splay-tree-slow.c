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

typedef struct _tree_vertex {
    struct _tree_vertex *parent;
    struct _tree_vertex *left, *right;
    void *data;
} TreeVertex;

typedef struct _tree {
    struct _tree_vertex *top;
    size_t data_size;
    int (*comparator)(const void *, const void *);
} Tree;

enum TreeWalkType { PRE_ORDER, IN_ORDER, OUT_ORDER };

/**
 * @brief constructor of splay tree data structure.
 *
 * @param data_size size of data to store in tree
 * @param comparator comparator for data stored in structure
 * @return Tree* pointer to a newly created structure
 */
Tree *tree_create(size_t data_size,
                  int (*comparator)(const void *, const void *)) {
    Tree *tree = malloc(sizeof(Tree));
    tree->top = NULL;
    tree->data_size = data_size;
    tree->comparator = comparator;
    return tree;
}

TreeVertex *new_vertex(size_t data_size, TreeVertex *parent, void *data) {
    TreeVertex *vertex = malloc(sizeof(TreeVertex));
    vertex->parent = parent;
    vertex->left = vertex->right = NULL;
    vertex->data = malloc(data_size);
    memcpy(vertex->data, data, data_size);
    return vertex;
}

TreeVertex *tree_vertex_insert(size_t data_size,
                               int (*comparator)(const void *, const void *),
                               TreeVertex *top, void *data) {
    if (top == NULL) return new_vertex(data_size, NULL, data);

    if (comparator(top->data, data) > 0) {
        if (top->left == NULL) {
            top->left = new_vertex(data_size, top, data);
            return top->left;
        } else
            return tree_vertex_insert(data_size, comparator, top->left, data);
    } else {
        if (top->right == NULL) {
            top->right = new_vertex(data_size, top, data);
            return top->right;
        } else
            return tree_vertex_insert(data_size, comparator, top->right, data);
    }
}

TreeVertex *tree_vertex_rotate_right(TreeVertex *vertex) {
    assert(vertex != NULL);
    TreeVertex *p = vertex, *x = vertex->left;
    assert(x != NULL);
    x->parent = p->parent;
    if (p->parent != NULL) {
        if (p->parent->left == p)
            p->parent->left = x;
        else if (p->parent->right == p)
            p->parent->right = x;
    }
    p->parent = x;
    p->left = x->right;
    if (p->left) p->left->parent = p;
    x->right = p;
    return x->parent;
}

TreeVertex *tree_vertex_rotate_left(TreeVertex *vertex) {
    assert(vertex != NULL);
    TreeVertex *p = vertex, *x = vertex->right;
    assert(x != NULL);
    x->parent = p->parent;
    if (p->parent != NULL) {
        if (p->parent->left == p)
            p->parent->left = x;
        else if (p->parent->right == p)
            p->parent->right = x;
    }
    p->parent = x;
    p->right = x->left;
    if (p->right) p->right->parent = p;
    x->left = p;
    return x->parent;
}

TreeVertex *tree_splay(TreeVertex *vertex) {
    if (vertex == NULL) return NULL;
    while (vertex->parent != NULL) {
        if (vertex->parent->parent == NULL) {
            if (vertex == vertex->parent->left)
                vertex->parent = tree_vertex_rotate_right(vertex->parent);
            else {
                vertex->parent = tree_vertex_rotate_left(vertex->parent);
            }
        } else {
            if (vertex == vertex->parent->left &&
                vertex->parent == vertex->parent->parent->left) {
                vertex->parent->parent =
                    tree_vertex_rotate_right(vertex->parent->parent);
                vertex->parent = tree_vertex_rotate_right(vertex->parent);
            } else if (vertex == vertex->parent->right &&
                       vertex->parent == vertex->parent->parent->right) {
                vertex->parent->parent =
                    tree_vertex_rotate_left(vertex->parent->parent);
                vertex->parent = tree_vertex_rotate_left(vertex->parent);
            } else if (vertex == vertex->parent->right &&
                       vertex->parent == vertex->parent->parent->left) {
                vertex->parent = tree_vertex_rotate_left(vertex->parent);
                vertex->parent = tree_vertex_rotate_right(vertex->parent);
            } else {
                vertex->parent = tree_vertex_rotate_right(vertex->parent);
                vertex->parent = tree_vertex_rotate_left(vertex->parent);
            }
        }
    }
    return vertex;
}

void tree_insert(Tree *tree, void *data) {
    TreeVertex *inserted =
        tree_vertex_insert(tree->data_size, tree->comparator, tree->top, data);
    tree->top = tree_splay(inserted);
}

void tree_vertex_walk(TreeVertex *top, void (*work)(TreeVertex *),
                      enum TreeWalkType type) {
    if (type == PRE_ORDER) work(top);
    if (top->left != NULL) tree_vertex_walk(top->left, work, type);
    if (type == IN_ORDER) work(top);
    if (top->right != NULL) tree_vertex_walk(top->right, work, type);
    if (type == OUT_ORDER) work(top);
}

void tree_walk(Tree *tree, void (*work)(TreeVertex *), enum TreeWalkType type) {
    tree_vertex_walk(tree->top, work, type);
}

long tree_sum(TreeVertex *vertex) {
    if (vertex == NULL) return 0;
    return *((int *)vertex->data) + tree_sum(vertex->left) +
           tree_sum(vertex->right);
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

TreeVertex *tree_vertex_merge(TreeVertex *t1, TreeVertex *t2, size_t data_size,
                              int (*comparator)(const void *, const void *)) {
    if (t1 == NULL) return t2;
    if (t2 == NULL) return t1;
    TreeVertex *p = t2->parent;
    t2->parent = NULL;
    t2 = tree_splay(tree_vertex_insert(data_size, comparator, t2, t1->data));
    t2->parent = p;
    t2->left = tree_vertex_merge(t1->left, t2->left, data_size, comparator);
    if (t2->left) t2->left->parent = t2;
    t2->right = tree_vertex_merge(t1->right, t2->right, data_size, comparator);
    if (t2->right) t2->right->parent = t2;
    tree_vertex_destroy(t1);
    return t2;
}

TreeVertex *tree_vertex_pop(TreeVertex *vertex, size_t data_size,
                            int (*comparator)(const void *, const void *)) {
    if (vertex == NULL) return NULL;

    if (vertex->left) vertex->left->parent = NULL;
    if (vertex->right) vertex->right->parent = NULL;

    TreeVertex *top =
        tree_vertex_merge(vertex->left, vertex->right, data_size, comparator);
    tree_vertex_destroy(vertex);
    return top;
}

void tree_pop(Tree *tree) {
    TreeVertex *top =
        tree_vertex_pop(tree->top, tree->data_size, tree->comparator);
    tree->top = top;
    if (tree->top) tree->top->parent = NULL;
}

int tree_vertex_find(TreeVertex *vertex, void *data,
                     int (*comparator)(const void *, const void *)) {
    if (!vertex) return 0;

    int result = comparator(data, vertex->data);
    if (result > 0) {
        return tree_vertex_find(vertex->right, data, comparator);
    } else if (result < 0) {
        return tree_vertex_find(vertex->left, data, comparator);
    } else
        return 1;
}

size_t tree_vertex_get_depth(TreeVertex *vertex) {
    size_t left = 0, right = 0;
    if (vertex->left != NULL) left = tree_vertex_get_depth(vertex->left);
    if (vertex->right != NULL) right = tree_vertex_get_depth(vertex->right);
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
        if (vertex->left != NULL)
            tree_vertex_print_level(vertex->left, print_data, level,
                                    current + 1);
        if (vertex->right != NULL)
            tree_vertex_print_level(vertex->right, print_data, level,
                                    current + 1);
    }
}

void tree_print(TreeVertex *vertex, int space, int margin,
                void (*print_data)(void *data)) {
    if (vertex == NULL) return;

    space += margin;

    tree_print(vertex->right, space, margin, print_data);

    for (int i = margin; i < space; i++) printf(" ");
    print_data(vertex->data);
    // printf("(A=%d,P=%d)", *((int *)&vertex) % 10000,
    //        *((int *)&(vertex->parent)) % 10000);
    printf("\n");

    tree_print(vertex->left, space, margin, print_data);
}

void tree_vertex_print(TreeVertex *vertex, void (*print_data)(void *data)) {
    printf("--------------------------------\n");
    tree_print(vertex, 0, 4, print_data);
    printf("--------------------------------\n");
}

long tree_segment_size(Tree *tree, int min, int max) { return 0; }

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

    long last_sum = 0;
    char command;
    for (size_t i = 0; i < N; i++) {
        scanf("%c", &command);
        if (command == '+') {
            int number;
            scanf("%d", &number);
            number = (number + last_sum) % 1000000000;
            if (!tree_vertex_find(tree->top, &number, comparator)) {
                // printf("Not found! Adding!\n");
                tree_insert(tree, &number);
            }
            // tree_print_levels(tree, print_int);
            tree_vertex_print(tree->top, print_int);
            last_sum = 0;
        } else if (command == '?') {
            int min, max;
            scanf("%d%d", &min, &max);
            min--;

            TreeVertex *inserted_min = tree_vertex_insert(
                tree->data_size, tree->comparator, tree->top, &min);
            inserted_min = tree_splay(inserted_min);
            TreeVertex *above_min = inserted_min->right;
            if (above_min) above_min->parent = NULL;

            // printf("AFTER INSERTED MIN!\n");
            // tree_vertex_print(inserted_min, print_int);

            TreeVertex *inserted_max = tree_vertex_insert(
                tree->data_size, tree->comparator, above_min, &max);
            inserted_max = tree_splay(inserted_max);
            TreeVertex *below_max = inserted_max->left;

            // printf("AFTER INSERTED MAX!\n");
            // tree_vertex_print(inserted_max, print_int);

            last_sum = tree_sum(below_max);
            printf("%ld\n", last_sum);

            inserted_min->right = tree_vertex_pop(inserted_max, tree->data_size,
                                                  tree->comparator);
            if (inserted_min->right) inserted_min->right->parent = inserted_min;

            // printf("AFTER MAX RESTORED!\n");
            // tree_vertex_print(inserted_min, print_int);

            tree->top = tree_vertex_pop(inserted_min, tree->data_size,
                                        tree->comparator);
            if (tree->top) tree->top->parent = NULL;

            // printf("AFTER MIN RESTORED!\n");
            // tree_vertex_print(tree->top, print_int);

        } else
            i--;
    }

    tree_destroy(tree);
    return 0;
}