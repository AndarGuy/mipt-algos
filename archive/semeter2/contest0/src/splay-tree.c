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
    int data;
    long long sum;
} TreeVertex;

enum TreeWalkType { PRE_ORDER, IN_ORDER, OUT_ORDER };

TreeVertex *new_vertex(TreeVertex *parent, int data) {
    TreeVertex *vertex = malloc(sizeof(TreeVertex));
    vertex->parent = parent;
    vertex->left = vertex->right = NULL;
    vertex->data = data;
    vertex->sum = data;
    return vertex;
}

TreeVertex *tree_vertex_insert(TreeVertex *top, int data) {
    if (top == NULL) return new_vertex(NULL, data);

    if (top->data > data) {
        if (top->left == NULL) {
            top->left = new_vertex(top, data);
            return top->left;
        } else
            return tree_vertex_insert(top->left, data);
    } else {
        if (top->right == NULL) {
            top->right = new_vertex(top, data);
            return top->right;
        } else
            return tree_vertex_insert(top->right, data);
    }
}

void recalculate_sum(TreeVertex *vertex) {
    vertex->sum = vertex->data;
    if (vertex->left != NULL) vertex->sum += vertex->left->sum;
    if (vertex->right != NULL) vertex->sum += vertex->right->sum;
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
    recalculate_sum(vertex);
    recalculate_sum(x);
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
    recalculate_sum(vertex);
    recalculate_sum(x);
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

void tree_vertex_walk(TreeVertex *top, void (*work)(TreeVertex *),
                      enum TreeWalkType type) {
    if (type == PRE_ORDER) work(top);
    if (top->left != NULL) tree_vertex_walk(top->left, work, type);
    if (type == IN_ORDER) work(top);
    if (top->right != NULL) tree_vertex_walk(top->right, work, type);
    if (type == OUT_ORDER) work(top);
}

TreeVertex *tree_destroy(TreeVertex *tree) {
    tree_vertex_walk(tree, (void (*)(TreeVertex *))free, OUT_ORDER);
    return NULL;
}

TreeVertex *tree_vertex_merge(TreeVertex *t1, TreeVertex *t2) {
    if (t1 == NULL) return t2;
    if (t2 == NULL) return t1;
    TreeVertex *p = t2->parent;
    t2->parent = NULL;
    t2 = tree_splay(tree_vertex_insert(t2, t1->data));
    t2->parent = p;
    t2->left = tree_vertex_merge(t1->left, t2->left);
    if (t2->left) t2->left->parent = t2;
    t2->right = tree_vertex_merge(t1->right, t2->right);
    if (t2->right) t2->right->parent = t2;
    free(t1);
    return t2;
}

TreeVertex *tree_vertex_pop(TreeVertex *vertex) {
    if (vertex == NULL) return NULL;

    if (vertex->left) vertex->left->parent = NULL;
    if (vertex->right) vertex->right->parent = NULL;

    TreeVertex *top = tree_vertex_merge(vertex->left, vertex->right);
    free(vertex);
    return top;
}

int tree_vertex_find(TreeVertex *vertex, int data) {
    if (!vertex) return 0;

    if (data > vertex->data) {
        return tree_vertex_find(vertex->right, data);
    } else if (data < vertex->data) {
        return tree_vertex_find(vertex->left, data);
    } else
        return 1;
}

void tree_print(TreeVertex *vertex, int space, int margin) {
    if (vertex == NULL) return;

    space += margin;

    tree_print(vertex->right, space, margin);

    for (int i = margin; i < space; i++) printf(" ");
    printf("%d", vertex->data);
    printf("\n");

    tree_print(vertex->left, space, margin);
}

void tree_vertex_print(TreeVertex *vertex) {
    printf("--------------------------------\n");
    tree_print(vertex, 0, 4);
    printf("--------------------------------\n");
}

void print_int(void *a) { printf("%d ", *((int *)a)); }

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);

    TreeVertex *tree = NULL;

    long last_sum = 0;
    char command;
    for (size_t i = 0; i < N; i++) {
        scanf("%c", &command);
        if (command == '+') {
            int number;
            scanf("%d", &number);
            number = (number + last_sum) % 1000000000;
            if (!tree_vertex_find(tree, number)) {
                // printf("Not found! Adding!\n");
                tree = tree_splay(tree_vertex_insert(tree, number));
            }
            // tree_print_levels(tree, print_int);
            // tree_vertex_print(tree);
            last_sum = 0;
        } else if (command == '?') {
            int min, max;
            scanf("%d%d", &min, &max);
            min--;

            TreeVertex *temp;

            temp = tree_splay(tree_vertex_insert(tree, min));
            // tree_vertex_print(temp);
            TreeVertex *above_min = temp->right, *below_min = temp->left;
            if (above_min) above_min->parent = NULL;
            if (below_min) below_min->parent = NULL;
            free(temp);

            // printf("AFTER INSERTED MIN!\n");
            // tree_vertex_print(inserted_min, print_int);

            temp = tree_splay(tree_vertex_insert(above_min, max));
            TreeVertex *below_max = temp->left, *above_max = temp->right;
            if (below_max) below_max->parent = NULL;
            if (above_max) above_max->parent = NULL;
            free(temp);

            // printf("AFTER INSERTED MAX!\n");
            // tree_vertex_print(inserted_max, print_int);

            last_sum = below_max ? below_max->sum : 0;
            printf("%ld\n", last_sum);

            tree = tree_vertex_merge(below_min,
                                     tree_vertex_merge(below_max, above_max));

            // printf("AFTER MIN RESTORED!\n");
            // tree_vertex_print(tree->top, print_int);

        } else
            i--;
    }

    tree_destroy(tree);
    return 0;
}