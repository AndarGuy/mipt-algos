/*

Хеш-таблица

Реализуйте структуру данных хеш-таблица. Она должна по заданному ключу находить
значение, которое этому ключу соответствует.

Ключ и значение таблицы — строка из строчных букв латинского алфавита не более
20 символов.

В каждой строке стандартного ввода находится одна из пяти возможных команд:

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

ssize_t hash_func(void *a) {
    ssize_t hash = 0;
    ssize_t power = 1;
    for (char *i = a; *i > 0; i++, power *= 31) {
        hash += *i * power;
    }
    return hash;
}

#define MAX_LENGHT 21

int main(int argc, char *argv[]) {
    HashMap hashmaps[10] = {0};
    char key[MAX_LENGHT] = {0}, value[MAX_LENGHT] = {0};

    char command[MAX_LENGHT] = {0};
    while (scanf("%s", command) > 0) {
        int num = 0;
        scanf("%d", &num);
        if (strcmp(command, "create") == 0) {
            hashmaps[num] =
                hashmap_create(100000, MAX_LENGHT, MAX_LENGHT, hash_func,
                               (int (*)(const void *, const void *))strcmp);
        } else if (strcmp(command, "insert") == 0) {
            scanf("%s%*c%s", key, value);
            if (hashmaps[num] == NULL) continue;
            hashmap_put(hashmaps[num], key, value);
        } else if (strcmp(command, "find") == 0) {
            scanf("%s", key);
            if (hashmaps[num] == NULL ||
                hashmap_get(hashmaps[num], key, value) != 0)
                printf("MISSING\n");
            else
                printf("%s\n", value);
        } else if (strcmp(command, "erase") == 0) {
            scanf("%s", key);
            if (hashmaps[num] == NULL) continue;
            hashmap_remove(hashmaps[num], key);
        } else if (strcmp(command, "destroy") == 0) {
            hashmap_destroy(hashmaps[num]);
            hashmaps[num] = NULL;
        }
    }

    for (size_t i = 0; i < 10; i++) {
        if (hashmaps[i] > 0) {
            hashmap_destroy(hashmaps[i]);
        }
    }

    return 0;
}