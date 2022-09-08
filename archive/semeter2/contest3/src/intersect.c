/*

Пересечение множеств

Задано 2⩽N⩽1000 множеств, каждое из которых содержит ровно 3⩽M⩽10000 чисел,
значения которых могут находиться в интервале -2×109..2×109. N × M ⩽ 106.

Значения элементов каждого множества задаются в произвольном порядке.

Гарантируется, что для одного множества все задаваемые значения различные.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vector {
    unsigned int size;
    unsigned int capacity;
    int *elems;
};
typedef struct vector *Vector;

struct vector *vector_new(size_t elems) {
    struct vector *v = malloc(sizeof(struct vector));
    v->capacity = elems > 0 ? elems : 1;
    v->size = elems;
    v->elems = calloc(v->capacity, sizeof(int));
    return v;
}

size_t vector_size(struct vector const *v) { return v->size; }

int vector_resize(struct vector *v, size_t new_size) {
    v->size = new_size;
    while (v->size > v->capacity) {
        void *pointer =
            realloc(v->elems, sizeof(int) * (v->capacity = v->capacity * 2));
        if (!pointer) return 1;
        v->elems = pointer;
    }
    return 0;
}

int vector_set(struct vector *v, size_t index, int *elem) {
    if (v->size <= index) return 1;
    v->elems[index] = *elem;
    return 0;
}

int vector_push(struct vector *v, int *elem) {
    if (vector_resize(v, v->size + 1)) return 1;
    return vector_set(v, vector_size(v) - 1, elem);
}

int vector_get(const struct vector *v, size_t index, int *elem) {
    if (v->size <= index) return 1;
    *elem = v->elems[index];
    return 0;
}

int vector_empty(struct vector const *v) { return v->size == 0; }

int vector_pop(struct vector *v, int *elem) {
    if (vector_empty(v)) return 1;
    vector_get(v, v->size - 1, elem);
    vector_resize(v, v->size - 1);
    return 0;
}

struct vector *vector_delete(struct vector *v) {
    free(v->elems);
    free(v);
    return NULL;
}

// --------------------------------------------------- //

typedef struct _hashmap {
    ssize_t (*hash)(void *);
    int (*comparator)(const void *, const void *);
    size_t capacity;
    size_t key_size;
    Vector *table;
} * HashMap;

HashMap hashmap_create(size_t capacity, size_t key_size,
                       ssize_t (*hash)(void *)) {
    HashMap created = malloc(sizeof(struct _hashmap));
    created->table = calloc(capacity, sizeof(Vector));
    created->capacity = capacity;
    created->key_size = key_size;
    created->hash = hash;
    return created;
}

int hashmap_find(HashMap hashmap, size_t hash, int *key) {
    Vector node = hashmap->table[hash];
    for (size_t i = 0; i < node->size; i++) {
        int temp;
        vector_get(node, i, &temp);
        if (temp == *key) {
            return i;
        }
    }
    return -1;
}

void hashmap_put(HashMap hashmap, int *key) {
    size_t hash = hashmap->hash(key) % hashmap->capacity;
    Vector node = hashmap->table[hash];

    if (node == NULL) {
        node = hashmap->table[hash] = vector_new(0);
        vector_push(node, key);
    } else {
        int index = hashmap_find(hashmap, hash, key);
        if (index == -1)
            vector_push(node, key);
        else {
            int temp = 0;
            vector_get(node, index, &temp);
            *key = temp;
        }
    }
}

int hashmap_get(HashMap hashmap, void *key) {
    size_t hash = hashmap->hash(key) % hashmap->capacity;
    Vector node = hashmap->table[hash];
    if (node == NULL) return 1;
    int index = hashmap_find(hashmap, hash, key);
    if (index == -1) return 1;
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

ssize_t hash(void *a) {
    int *hash = a;
    return *hash;
}

int main(int argc, char *argv[]) {
    int N, M;
    scanf("%d%d", &N, &M);

    int **arrays = calloc(N, sizeof(int *));
    for (int i = 0; i < N; i++) {
        arrays[i] = calloc(M, sizeof(int));
        for (int j = 0; j < M; j++) {
            scanf("%d", arrays[i] + j);
        }
    }

    int max = 0;
    HashMap hashmap = NULL;
    for (int i = 0; i < N; i++) {
        hashmap = hashmap_create(100000, sizeof(int), hash);
        for (int j = 0; j < M; j++) hashmap_put(hashmap, &arrays[i][j]);

        for (size_t j = i + 1; j < N; j++) {
            int inside = 0;
            for (size_t k = 0; k < M; k++) {
                if (!hashmap_get(hashmap, &arrays[j][k])) inside++;
                max = max > inside ? max : inside;
            }
        }
        hashmap_destroy(hashmap);
    }

    printf("%d\n", max);

    for (size_t i = 0; i < N; i++) {
        free(arrays[i]);
    }
    free(arrays);

    return 0;
}