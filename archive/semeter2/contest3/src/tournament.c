/*

Турнирная таблица индивидуалов

В индивидуальном первенстве по Дарт участникам необходимо было набрать
максимальное число очков за попытку. Количество попыток не ограничивалось. В
протокол комиссии попадало имя участника и результат за проделанную попытку.
Итоговым результатом участника считалась лучшая попытка. Если результаты
совпадают, первым считается игрок показавший данный результат раньше. Помогите
организаторам выстроить участников по убыванию.

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

int hashmap_put(HashMap hashmap, void *key, void *value) {
    size_t hash = hashmap->hash(key) % hashmap->capacity;
    Vector node = hashmap->table[hash];

    void *data = malloc(hashmap->key_size + hashmap->value_size);
    memcpy(data, key, hashmap->key_size);
    memcpy(data + hashmap->key_size, value, hashmap->value_size);

    int result = 0;

    if (node == NULL) {
        node = hashmap->table[hash] =
            vector_new(0, hashmap->key_size + hashmap->value_size);
        vector_push(node, data);

    } else {
        int index = hashmap_find(hashmap, hash, key);
        if (index == -1) {
            vector_push(node, data);
        } else {
            memcpy(node->elems + node->elem_size * index + hashmap->key_size,
                   value, hashmap->value_size);
        }
    }

    free(data);
    return result;
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

int hashmap_remove(HashMap hashmap, void *key) {
    size_t hash = hashmap->hash(key) % hashmap->capacity;
    Vector node = hashmap->table[hash];
    if (node == NULL) return 1;
    int index = hashmap_find(hashmap, hash, key);
    if (index == -1) return 1;
    void *data = malloc(hashmap->key_size + hashmap->value_size);
    vector_pop(node, data);
    vector_set(node, index, data);
    free(data);
    return 0;
}

void hashmap_destroy(HashMap hashmap) {
    for (size_t i = 0; i < hashmap->capacity; i++) {
        if (hashmap->table[i] != 0) vector_delete(hashmap->table[i]);
    }
    free(hashmap->table);
    free(hashmap);
}

ssize_t hash_func(void *a) {
    ssize_t hash = 0;
    ssize_t power = 1;
    for (char *i = a; *i > 0; i++, power *= 31) {
        hash += *i * power;
    }
    return hash;
}

// --------------------------------------------------- //

#define MAX_LENGHT 42

struct Score {
    int value;
    int turn;
};

struct Record {
    char player[MAX_LENGHT];
    struct Score score;
};

int record_comparator(const void *a, const void *b) {
    struct Record *recordA = (struct Record *)a;
    struct Record *recordB = (struct Record *)b;
    if (recordA->score.value > recordB->score.value)
        return -1;
    else if (recordA->score.value < recordB->score.value)
        return 1;
    else if (recordA->score.turn < recordB->score.turn)
        return -1;
    else if (recordA->score.turn > recordB->score.turn)
        return 1;
    else
        return 0;
}

int main(int argc, char *argv[]) {
    int M, N;
    scanf("%d", &M);
    scanf("%d", &N);

    struct Record *records = calloc(N, sizeof(struct Record));
    for (int i = 0; i < N; i++) {
        scanf("%s", records[i].player);
        scanf("%d", &records[i].score.value);
        records[i].score.turn = i;
    }

    qsort(records, N, sizeof(struct Record), record_comparator);

    HashMap hashmap =
        hashmap_create(10000, MAX_LENGHT, 1, hash_func,
                       (int (*)(const void *, const void *))strcmp);

    char buffer;
    for (size_t i = 0; i < N; i++) {
        buffer = 0;
        hashmap_get(hashmap, records[i].player, &buffer);
        if (buffer) continue;
        printf("%s\n", records[i].player);
        buffer = 1;
        hashmap_put(hashmap, records[i].player, &buffer);
    }

    hashmap_destroy(hashmap);
    free(records);

    return 0;
}