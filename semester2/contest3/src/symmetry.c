/*

Центральная симметрия

Имеется множество из 4⩽N⩽100000 точек в декартовом трёхмерном пространстве с
целочисленными координатами в диапазоне от -10^18 до 10^18. Интересно, ли у
этого множества центр симметрии? Это вам и предстоит выяснить.

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

struct Point {
    long long int x;
    long long int y;
    long long int z;
};

ssize_t hash_point(void *a) {
    struct Point *point = *((struct Point **)a);
    return point->x + point->y * 7369 + point->z * 7369 * 7369;
}

int comparator_point(const void *a, const void *b) {
    struct Point *pointA = *((struct Point **)a),
                 *pointB = *((struct Point **)b);
    return !(pointA->x == pointB->x && pointA->y == pointB->y &&
             pointA->z == pointB->z);
}

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);

    struct Point **points = calloc(N, sizeof(struct Point *));

    for (size_t i = 0; i < N; i++) {
        points[i] = calloc(1, sizeof(struct Point));
    }

    HashMap hashmap = hashmap_create(100000, sizeof(struct Point *), 1,
                                     hash_point, comparator_point);

    char value;
    struct Point *temp = calloc(1, sizeof(struct Point));
    __int128_t x = 0, y = 0, z = 0;
    for (size_t i = 0; i < N; i++) {
        scanf("%lld%lld%lld", &points[i]->x, &points[i]->y, &points[i]->z);
        x += points[i]->x;
        y += points[i]->y;
        z += points[i]->z;
        hashmap_put(hashmap, &points[i], &value);
    }

    int answer = 1;

    if (x % N || y % N || z % N) {
        answer = 0;
    } else {
        x /= N;
        y /= N;
        z /= N;
        for (size_t i = 0; i < N; i++) {
            temp->x = x * 2 - points[i]->x;
            temp->y = y * 2 - points[i]->y;
            temp->z = z * 2 - points[i]->z;
            // printf("%d %d %d %d %d %d\n", points[i]->x, points[i]->y,
            //        points[i]->z, temp->x, temp->y, temp->z);
            if (hashmap_get(hashmap, &temp, &value)) {
                answer = 0;
                break;
            }
        }
    }

    if (answer)
        printf("Yes\n");
    else
        printf("No\n");

    free(temp);
    hashmap_destroy(hashmap);
    for (size_t i = 0; i < N; i++) {
        free(points[i]);
    }
    free(points);

    return 0;
}