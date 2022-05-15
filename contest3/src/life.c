/*

Жизнь

Клеточные автоматы — одно из важных понятий математического моделирования. Одним
из первых клеточных автоматов была игра Жизнь. Она была изобретена в 1970-м году
американским математиком Конвеем.

У автора игра происходит на бесконечной доске. У нас столько времени и столько
пространства нет, поэтому ограничимся конечной. Игровое поле — двумерно, оно
состоит из клеток, у каждой из которых ровно восемь соседей (на бесконечной
доске). В клетках может зарождаться жизнь (живые клетки) и в клетках жизни может
не быть (мёртвые клетки).

На каждом такте происходит следующий процесс:

если живая клетка имеет двух или трёх живых соседей, то она остаётся жить, в
противном случае она погибает от одиночества или перенаселения; если мёртвая
клетка имеет ровно трёх живых соседей, в ней зарождается жизнь.

*/

#include <assert.h>
#include <stdbool.h>
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
typedef struct _queue {
    int front, rear, size;
    size_t capacity, data_size;
    void *array;
} * Queue;

Queue queue_create(size_t data_size) {
    Queue queue = malloc(sizeof(struct _queue));
    queue->capacity = 1;
    queue->front = queue->size = 0;

    queue->rear = queue->capacity - 1;
    queue->data_size = data_size;
    queue->array = malloc(queue->capacity * queue->data_size);
    return queue;
}

int queue_full(Queue queue) { return (queue->size == queue->capacity); }

int queue_empty(Queue queue) { return (queue->size == 0); }

void queue_expand(Queue queue) {
    queue->array =
        realloc(queue->array, queue->capacity * queue->data_size * 2);
    if (queue->rear < queue->front) {
        memcpy(queue->array + queue->capacity * queue->data_size, queue->array,
               (queue->rear % queue->capacity + 1) * queue->data_size);
        queue->rear = queue->front + queue->size - 1;
    }
    queue->capacity = queue->capacity * 2;
}

void enqueue(Queue queue, void *item) {
    if (queue_full(queue)) queue_expand(queue);
    queue->rear = (queue->rear + 1) % queue->capacity;
    memcpy(queue->array + queue->rear * queue->data_size, item,
           queue->data_size);
    queue->size = queue->size + 1;
}

void *dequeue(Queue queue) {
    if (queue_empty(queue)) return NULL;
    void *item = queue->array + queue->front * queue->data_size;
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

void *queue_front(Queue queue) {
    if (queue_empty(queue)) return NULL;
    return queue->array + queue->front * queue->data_size;
}

void *queue_rear(Queue queue) {
    if (queue_empty(queue)) return NULL;
    return queue->array + queue->rear * queue->data_size;
}

Queue queue_destroy(Queue queue) {
    free(queue->array);
    free(queue);
    return NULL;
}

void queue_print(Queue queue, void (*print_data)(void const *data)) {
    printf("[");
    for (size_t i = 0; i < queue->size; i++) {
        print_data(queue->array +
                   ((queue->front + i) % queue->capacity) * queue->data_size);
        if (i + 1 < queue->size) printf(", ");
    }
    printf("]");
    printf(" ");
    printf(" {%d/%zu, %d:%d}\n", queue->size, queue->capacity, queue->front,
           queue->rear);
}

// --------------------------------------------------- //

struct Point {
    short x;
    short y;
};

ssize_t hash(void *a) {
    struct Point *point = (struct Point *)a;
    return point->x * 5000 + point->y;
}

void print_data(const void *data) {
    struct Point *point = (struct Point *)data;
    printf("(%hd, %hd)", point->x, point->y);
}

int comparator(const void *a, const void *b) {
    struct Point *pointA = (struct Point *)a, *pointB = (struct Point *)b;
    if (pointA->x > pointB->x) return 1;
    if (pointA->x < pointB->x) return -1;
    if (pointA->y > pointB->y) return 1;
    if (pointA->y < pointB->y) return -1;
    return 0;
}

char get_state(HashMap machine, struct Point point) {
    char state;
    if (hashmap_get(machine, &point, &state)) return 0;
    return state;
}

int get_state_xy(HashMap machine, short x, short y) {
    struct Point point = {x, y};
    return get_state(machine, point);
}

int count_relatives(HashMap hashmap, struct Point point) {
    short x = point.x, y = point.y;
    return get_state_xy(hashmap, x - 1, y - 1) +
           get_state_xy(hashmap, x - 1, y + 0) +
           get_state_xy(hashmap, x - 1, y + 1) +
           get_state_xy(hashmap, x + 0, y - 1) +
           get_state_xy(hashmap, x + 0, y + 1) +
           get_state_xy(hashmap, x + 1, y - 1) +
           get_state_xy(hashmap, x + 1, y + 0) +
           get_state_xy(hashmap, x + 1, y + 1);
}

char new_state(HashMap machine, struct Point point) {
    char state = get_state(machine, point);
    int relatives = count_relatives(machine, point);
    if (state && relatives == 2) return 1;
    if (relatives == 3) return 1;
    return 0;
}

int main(int argc, char *argv[]) {
    int N, M, L, K;
    scanf("%d%d%d%d", &N, &M, &L, &K);

    HashMap machine = hashmap_create(5000, sizeof(struct Point), sizeof(char),
                                     hash, comparator);
    Queue alive = queue_create(sizeof(struct Point));

    char state = 1;  // 1 - alive
    struct Point cell;
    for (size_t i = 0; i < L; i++) {
        scanf("%hd%hd", &cell.x, &cell.y);
        hashmap_put(machine, &cell, &state);
        enqueue(alive, &cell);
    }

    for (size_t i = 0; i < K; i++) {
        HashMap new_machine = hashmap_create(5000, sizeof(struct Point),
                                             sizeof(char), hash, comparator);
        int n_alives = alive->size;
        for (size_t j = 0; j < n_alives; j++) {
            struct Point current;
            memcpy(&current, dequeue(alive), sizeof(struct Point));
            // printf("Dequeueing (%hd, %hd) [%d].\n", current.x, current.y,
            //        alive->size);
            // queue_print(alive, print_data);
            for (size_t k = 0; k < 3; k++)
                for (size_t l = 0; l < 3; l++) {
                    short x = current.x + k - 1, y = current.y + l - 1;
                    if (x < 0 || y < 0 || x + 1 > N || y + 1 > M) continue;
                    struct Point to_proccess = {x, y};
                    if (!hashmap_get(new_machine, &to_proccess, &state))
                        continue;
                    char state = new_state(machine, to_proccess);
                    // printf("State for (%hd, %hd) is %hhd.\n", to_proccess.x,
                    //        to_proccess.y, state);
                    hashmap_put(new_machine, &to_proccess, &state);
                    if (state) enqueue(alive, &to_proccess);
                    // queue_print(alive, print_data);
                }
        }
        hashmap_destroy(machine);
        machine = new_machine;
    }

    // printf("\n");

    queue_expand(alive);
    qsort(queue_front(alive), alive->size, alive->data_size, comparator);
    while (!queue_empty(alive)) {
        struct Point *current = dequeue(alive);
        printf("%d %d\n", current->x, current->y);
    }

    hashmap_destroy(machine);
    queue_destroy(alive);

    return 0;
}