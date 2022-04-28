
#include <stdlib.h>
#include <string.h>

typedef struct _vector {
    size_t size;
    size_t capacity;
    size_t elem_size;
    void *elems;
} * Vector;

Vector vector_new(size_t elems, size_t elem_size) {
    Vector v = malloc(sizeof(struct _vector));
    v->capacity = elems > 0 ? elems : 1;
    v->size = elems;
    v->elem_size = elem_size;
    v->elems = calloc(v->capacity, elem_size);
    return v;
}

void vector_set(Vector v, size_t index, void const *elem) {
    if (v->size <= index) return;
    memcpy(v->elems + index * v->elem_size, elem, v->elem_size);
}

void vector_resize(Vector v, size_t new_size) {
    v->size = new_size;
    while (v->size > v->capacity) {
        void *pointer =
            realloc(v->elems, v->elem_size * (v->capacity = v->capacity * 2));
        if (!pointer) return;
        v->elems = pointer;
    }
}

void vector_push(Vector v, void const *elem) {
    vector_resize(v, v->size + 1);
    vector_set(v, v->size - 1, elem);
}

int vector_get(const Vector v, size_t index, void *elem) {
    if (v->size <= index) return 1;
    memcpy(elem, v->elems + index * v->elem_size, v->elem_size);
    return 0;
}

void vector_pop(Vector v, void *elem) {
    if (v->size == 0) return;
    vector_get(v, v->size - 1, elem);
    vector_resize(v, v->size - 1);
}

Vector *vector_delete(Vector v) {
    free(v->elems);
    free(v);
    return NULL;
}

int main() { return 0; }