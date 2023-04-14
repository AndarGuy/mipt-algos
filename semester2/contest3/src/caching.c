/*

Кеширующий сервер

Дормидонт работает в компании, которая занимается обработкой больших данных.
Обрабатываемые данные находятся где-то в распределённой системе. Количество
различных данных в системе ограничено и каждое данное имеет свой номер. Эти
данные регулярно требуются различным клиентам и, поскольку время обращения к ним
достаточно велико, для ускорения обработки информации Дормидонту поручено
написать часть middlware – сервер-посредник, к которому и обращаются теперь
клиенты за данными.

Так как система – распределённая, а сервер — нет, все требуемые данные на сервер
не помещаются, но он имеет возможность запоминать результаты своих запросов к
распределённой системе. Для этого на сервере выделена ограниченная память на N
запросов. Обработать запрос, минуя сервер, нельзя, то есть любой запрос обязан
перед обработкой оказаться на сервере.

К большой радости Дормидонта оказалось, что самые крупные и значимые клиенты
всегда обращаются за одними и теми же данными в одном и том же порядке, так что
у него есть записанная последовательность запросов. Дормидонт придумал такой
алгоритм, что как можно большее количество запросов исполняется из кеша сервера,
без обращения к распределённой системе. Придумаете ли вы что-то подобное?

*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vector {
    size_t size;
    size_t capacity;
    size_t elem_size;
    void *elems;
};
typedef struct vector *Vector;

struct vector *vector_new(size_t elems, size_t elem_size);

size_t vector_size(struct vector const *v);

int vector_push(struct vector *v, const void *elem);

int vector_pop(struct vector *v, void *elem);

int vector_get(struct vector const *v, size_t index, void *elem);

int vector_set(struct vector *v, size_t index, void const *elem);

int vector_resize(struct vector *v, size_t new_size);

int vector_empty(struct vector const *v);

void vector_print(struct vector const *v, void (*pf)(void const *v));

struct vector *vector_delete(struct vector *v);

struct vector *vector_new(size_t elems, size_t elem_size) {
    assert(elem_size);
    struct vector *v = malloc(sizeof(struct vector));
    v->capacity = elems > 0 ? elems : 1;
    v->size = elems;
    v->elem_size = elem_size;
    v->elems = calloc(v->capacity, elem_size);
    return v;
}

size_t vector_size(struct vector const *v) {
    assert(v);
    return v->size;
}

int vector_push(struct vector *v, void const *elem) {
    assert(v);
    assert(elem);
    if (vector_resize(v, v->size + 1)) return 1;
    return vector_set(v, vector_size(v) - 1, elem);
}

int vector_set(struct vector *v, size_t index, void const *elem) {
    assert(v);
    assert(elem);
    if (v->size <= index) return 1;
    memcpy(v->elems + index * v->elem_size, elem, v->elem_size);
    return 0;
}

int vector_resize(struct vector *v, size_t new_size) {
    assert(v);
    v->size = new_size;
    while (v->size > v->capacity) {
        void *pointer =
            realloc(v->elems, v->elem_size * (v->capacity = v->capacity * 2));
        if (!pointer) return 1;
        v->elems = pointer;
    }
    return 0;
}

int vector_pop(struct vector *v, void *elem) {
    assert(v);
    assert(elem);
    if (vector_empty(v)) return 1;
    vector_get(v, v->size - 1, elem);
    vector_resize(v, v->size - 1);
    return 0;
}

int vector_get(const struct vector *v, size_t index, void *elem) {
    assert(v);
    assert(elem);
    if (v->size <= index) return 1;
    memcpy(elem, v->elems + index * v->elem_size, v->elem_size);
    return 0;
}

int vector_empty(struct vector const *v) {
    assert(v);
    return v->size == 0;
}

struct vector *vector_delete(struct vector *v) {
    assert(v);
    free(v->elems);
    free(v);
    return NULL;
}

void vector_print(struct vector const *v, void (*pf)(void const *v)) {
    assert(v);
    assert(pf);
    printf("[");
    for (size_t i = 0; i < v->size; i++) {
        pf(v->elems + i * v->elem_size);
        if (i != v->size - 1) printf(", ");
    }
    printf("]\n");
}

// --------------------------------------------------- //

typedef struct _hashmap {
    ssize_t (*hash)(void *);
    int (*comparator)(const void *, const void *);
    size_t capacity;
    size_t key_size;
    size_t value_size;
    Vector *table;
} * HashMap;

HashMap hashmap_create(size_t capacity, size_t key_size, size_t value_size,
                       ssize_t (*hash)(void *),
                       int (*comparator)(const void *, const void *)) {
    HashMap created = malloc(sizeof(struct _hashmap));
    created->table = calloc(capacity, sizeof(Vector));
    created->capacity = capacity;
    created->key_size = key_size;
    created->value_size = value_size;
    created->hash = hash;
    created->comparator = comparator;
    return created;
}

int hashmap_find(HashMap hashmap, size_t hash, void *key) {
    Vector node = hashmap->table[hash];
    for (size_t i = 0; i < node->size; i++) {
        if (hashmap->comparator(node->elems + i * node->elem_size, key) == 0) {
            return i;
        }
    }
    return -1;
}

void hashmap_put(HashMap hashmap, void *key, void *value) {
    size_t hash = hashmap->hash(key) % hashmap->capacity;
    Vector node = hashmap->table[hash];

    void *data = malloc(hashmap->key_size + hashmap->value_size);
    memcpy(data, key, hashmap->key_size);
    memcpy(data + hashmap->key_size, value, hashmap->value_size);

    if (node == NULL) {
        node = hashmap->table[hash] =
            vector_new(0, hashmap->key_size + hashmap->value_size);
        vector_push(node, data);
    } else {
        int index = hashmap_find(hashmap, hash, key);
        if (index == -1)
            vector_push(node, data);
        else
            memcpy(node->elems + node->elem_size * index + hashmap->key_size,
                   value, hashmap->value_size);
    }

    free(data);
}

int hashmap_get(HashMap hashmap, void *key, void *value) {
    size_t hash = hashmap->hash(key) % hashmap->capacity;
    Vector node = hashmap->table[hash];
    if (node == NULL) return 1;
    int index = hashmap_find(hashmap, hash, key);
    if (index == -1) return 1;
    memcpy(value, node->elems + node->elem_size * index + hashmap->key_size,
           hashmap->value_size);
    return 0;
}

void hashmap_remove(HashMap hashmap, void *key) {
    size_t hash = hashmap->hash(key) % hashmap->capacity;
    Vector node = hashmap->table[hash];
    if (node == NULL) return;
    int index = hashmap_find(hashmap, hash, key);
    if (index == -1) return;
    void *data = malloc(hashmap->key_size + hashmap->value_size);
    vector_pop(node, data);
    vector_set(node, index, data);
    free(data);
}

void hashmap_destroy(HashMap hashmap) {
    for (size_t i = 0; i < hashmap->capacity; i++) {
        if (hashmap->table[i] != 0) vector_delete(hashmap->table[i]);
    }
    free(hashmap->table);
    free(hashmap);
}

// --------------------------------------------------- //

typedef struct _tree_vertex {
    struct _tree_vertex *left, *right;
    size_t height;
    int key;
    unsigned long long int value;
} TreeVertex;

enum TreeWalkType { PRE_ORDER, IN_ORDER, OUT_ORDER };

TreeVertex *new_vertex(int key, unsigned long long int value) {
    TreeVertex *vertex = malloc(sizeof(TreeVertex));
    vertex->left = vertex->right = NULL;
    vertex->key = key;
    vertex->value = value;
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

TreeVertex *tree_vertex_insert(TreeVertex *top, int key,
                               unsigned long long int value) {
    if (top == NULL) return new_vertex(key, value);

    int result = top->key - key;
    if (result > 0) {
        top->left = tree_vertex_insert(top->left, key, value);
    } else if (result < 0) {
        top->right = tree_vertex_insert(top->right, key, value);
    } else {
        top->value = value;
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

TreeVertex *tree_get_max(TreeVertex *vertex) {
    if (!vertex) return NULL;
    if (vertex->right) return tree_get_max(vertex->right);
    return vertex;
}

TreeVertex *tree_delete_by_key(TreeVertex *top, int key) {
    if (!top) return NULL;
    int result = top->key - key;
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
        top->value = min->value;
        top->right = tree_delete_by_key(top->right, min->key);
    }
    return tree_balance(top);
}

void tree_print(TreeVertex *vertex, int space, int margin) {
    if (vertex == NULL) return;

    space += margin;

    tree_print(vertex->right, space, margin);

    for (int i = margin; i < space; i++) printf(" ");
    printf("[%d, %llu]", vertex->key, vertex->value);
    printf("\n");

    tree_print(vertex->left, space, margin);
}

void tree_vertex_print(TreeVertex *vertex) {
    printf("--------------------------------\n");
    tree_print(vertex, 0, 4);
    printf("--------------------------------\n");
}

// ----------------------------------------------- //

ssize_t hash(void *a) {
    unsigned long long int *hash = a;
    return *hash;
}

int comparator(const void *a, const void *b) {
    const unsigned long long int *c = a, *d = b;
    if (*c == *d) return 0;
    return 1;
}

int comparator_great(void *a, void *b) { return *((int *)a) - *((int *)b); }

int main(int argc, char *argv[]) {
    int N, M;
    scanf("%d%d", &N, &M);

    unsigned long long int *requests =
        calloc(M, sizeof(unsigned long long int));
    for (size_t i = 0; i < M; i++) {
        scanf("%llu", requests + i);
    }

    HashMap hashmap = hashmap_create(N, sizeof(unsigned long long int),
                                     sizeof(int), hash, comparator);

    int *next = calloc(M, sizeof(int));
    int position;
    for (size_t i = 0; i < M; i++) {
        int new = M - i - 1;
        unsigned long long int element = requests[new];
        if (hashmap_get(hashmap, &element, &position))
            next[new] = 2147483647 - i;
        else
            next[new] = position;
        hashmap_put(hashmap, &element, &new);
    }

    // for (size_t i = 0; i < M; i++) {
    //     printf("%d ", next[i]);
    // }
    // printf("\n");

    hashmap_destroy(hashmap);

    TreeVertex *tree = NULL;
    int size = 0;
    int operations = 0;
    for (size_t i = 0; i < M; i++) {
        unsigned long long element = requests[i];
        int order = next[i];
        TreeVertex *minimum = tree_get_min(tree), *maximum = tree_get_max(tree);
        // printf("- %llu\n", element);
        if (minimum && minimum->key == i) {
            // in cache
            tree = tree_delete_by_key(tree, minimum->key);
        } else {
            // not in cache
            if (size < N)
                size++;
            else
                tree = tree_delete_by_key(tree, maximum->key);
            operations++;
        }
        tree = tree_vertex_insert(tree, order, element);

        // tree_vertex_print(tree);
    }

    printf("%d\n", operations);

    tree_destroy(tree);
    free(requests);
    free(next);

    return 0;
}