/*

Поиск множеств

В первой строке файла содержится три числа: N - количество эталонных множеств, M
- размер каждого из множеств и K - количество пробных множеств.

Каждое из множеств содержит целые числа от 0 до 108, числа могут повторяться.

Требуется для каждого из пробных множеств вывести в отдельной строке 1, если это
множество в точности совпадает с каким-либо из эталонных множеств и 0, если оно
ни с одним не совпадает, то есть, выведено должно быть в точности K строк.

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

int vector_push(struct vector *v, const void *elem);

int vector_pop(struct vector *v, void *elem);

int vector_get(struct vector const *v, size_t index, void *elem);

int vector_set(struct vector *v, size_t index, void const *elem);

int vector_resize(struct vector *v, size_t new_size);

int vector_empty(struct vector const *v);

void vector_print(struct vector const *v, void (*pf)(void const *v));

struct vector *vector_delete(struct vector *v);

struct vector *vector_new(size_t elems, size_t elem_size) {
    struct vector *v = malloc(sizeof(struct vector));
    v->capacity = elems > 0 ? elems : 1;
    v->size = elems;
    v->elem_size = elem_size;
    v->elems = calloc(v->capacity, elem_size);
    return v;
}

int vector_push(struct vector *v, void const *elem) {
    if (vector_resize(v, v->size + 1)) return 1;
    return vector_set(v, v->size - 1, elem);
}

int vector_set(struct vector *v, size_t index, void const *elem) {
    if (v->size <= index) return 1;
    memcpy(v->elems + index * v->elem_size, elem, v->elem_size);
    return 0;
}

int vector_resize(struct vector *v, size_t new_size) {
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
    if (vector_empty(v)) return 1;
    vector_get(v, v->size - 1, elem);
    vector_resize(v, v->size - 1);
    return 0;
}

int vector_get(const struct vector *v, size_t index, void *elem) {
    if (v->size <= index) return 1;
    memcpy(elem, v->elems + index * v->elem_size, v->elem_size);
    return 0;
}

int vector_empty(struct vector const *v) { return v->size == 0; }

struct vector *vector_delete(struct vector *v) {
    free(v->elems);
    free(v);
    return NULL;
}

void vector_print(struct vector const *v, void (*pf)(void const *v)) {
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

// --------------------------------------------------- //

ssize_t set_hash(void *a) {
    ssize_t hash = 0;
    ssize_t power = 1;
    for (int *i = a; *i > 0; i++, power *= 31) {
        hash += *i * power;
    }
    return hash;
}

int set_compare(void *a, void *b) {
    int *i = a, *j = b;
    while (*i && *j) {
        if (*i != *j) return 1;
        i++;
        j++;
    }
    if (*i || *j) return 1;
    return 0;
}

int int_compare(const void *a, const void *b) {
    return *((int *)a) - *((int *)b);
}

int main() {
    // N - количество эталонных множеств, M - размер каждого из множеств и K -
    // количество пробных множеств.
    int N, M, K;
    scanf("%d%d%d", &N, &M, &K);

    HashMap hashmap =
        hashmap_create(100000, (M + 1) * sizeof(int), sizeof(char), set_hash,
                       (int (*)(const void *, const void *))set_compare);

    int *temp = calloc(M + 1, sizeof(int));
    char value = 0;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            scanf("%d", temp + j);
            temp[j]++;
        }
        temp[M] = 0;
        qsort(temp, M, sizeof(int), int_compare);
        hashmap_put(hashmap, temp, &value);
    }

    for (size_t i = 0; i < K; i++) {
        for (size_t j = 0; j < M; j++) {
            scanf("%d", temp + j);
            temp[j]++;
        }
        temp[M] = 0;
        qsort(temp, M, sizeof(int), int_compare);
        if (!hashmap_get(hashmap, temp, &value))
            printf("1\n");
        else
            printf("0\n");
    }

    free(temp);
    hashmap_destroy(hashmap);

    return 0;
}