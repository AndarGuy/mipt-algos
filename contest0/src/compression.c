
#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --------------------------------------------------------- //

typedef struct _heap {
    void *data;
    int (*comparator)(void *, void *);
    size_t data_size;
    size_t capacity;
    size_t size;
} Heap;

Heap *heap_create(size_t data_size, size_t capacity,
                  int (*comparator)(void *, void *));

void heap_insert(Heap *heap, void *data);

void heap_destroy(Heap *heap);

void heap_swap(Heap *heap, size_t a, size_t b);

void heap_peek(Heap *heap, void *data);

void heap_sift_up(Heap *heap, size_t index);

void heap_sift_down(Heap *heap, size_t index);

void heap_print(Heap *heap, void (*print_data)(void *));

Heap *heap_create(size_t data_size, size_t capacity,
                  int (*comparator)(void *, void *)) {
    Heap *heap = malloc(sizeof(Heap));
    heap->data_size = data_size;
    heap->capacity = capacity + 1;
    heap->size = 0;
    heap->comparator = comparator;
    heap->data = malloc(heap->data_size * heap->capacity);
    return heap;
}

void heap_destroy(Heap *heap) {
    free(heap->data);
    free(heap);
}

void heap_swap(Heap *heap, size_t a, size_t b) {
    void *temp = malloc(heap->data_size);
    memcpy(temp, heap->data + a * heap->data_size, heap->data_size);
    memcpy(heap->data + a * heap->data_size, heap->data + b * heap->data_size,
           heap->data_size);
    memcpy(heap->data + b * heap->data_size, temp, heap->data_size);
    free(temp);
}

void heap_peek(Heap *heap, void *data) {
    assert(heap->size > 0);
    memcpy(data, heap->data + heap->data_size, heap->data_size);
}

void heap_insert(Heap *heap, void *data) {
    assert(heap->size < heap->capacity - 1);
    heap->size++;
    memcpy(heap->data + heap->size * heap->data_size, data, heap->data_size);
    heap_sift_up(heap, heap->size);
}

void heap_sift_up(Heap *heap, size_t index) {
    for (size_t i = index;
         i > 1 && heap->comparator(heap->data + i * heap->data_size,
                                   heap->data + (i / 2) * heap->data_size) > 0;
         i /= 2) {
        heap_swap(heap, i, i / 2);
    }
}

void heap_pop(Heap *heap, void *data) {
    assert(heap->size > 0);
    if (data != NULL) heap_peek(heap, data);
    heap_swap(heap, 1, heap->size);
    heap_sift_down(heap, 1);
    heap->size--;
}

void heap_merge(Heap *dest, Heap *source) {
    assert(dest->data_size == source->data_size);
    for (size_t i = 0; i < source->size; i++) {
        heap_insert(dest, source->data + (i + 1) * source->data_size);
    }
}

void heap_sift_down(Heap *heap, size_t index) {
    while (true) {
        size_t left = index * 2, right = left + 1;
        size_t choice = index;
        if (left < heap->size &&
            heap->comparator(heap->data + left * heap->data_size,
                             heap->data + choice * heap->data_size) > 0)
            choice = left;
        if (right < heap->size &&
            heap->comparator(heap->data + right * heap->data_size,
                             heap->data + choice * heap->data_size) > 0)
            choice = right;
        if (choice == index) break;
        heap_swap(heap, index, choice);
        index = choice;
    }
}

// --------------------------------------------------------- //

typedef struct vector {
    size_t size;
    size_t capacity;
    size_t elem_size;
    void *elems;
} Vector;

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

// --------------------------------------------------------- //

int heap_formatter(void *a, void *b) {
    return (*((Entry **)a))->weight - (*((Entry **)b))->weight;
}

unsigned *count_chunks(const char *text_buffer, size_t text_size,
                       size_t chunk_size, size_t chunk_capacity) {
    assert(chunk_size <= sizeof(size_t));

    unsigned *valve = calloc(1 << (chunk_size << 3), sizeof(unsigned));

    size_t data = 0;
    for (size_t chunk = 0; chunk < text_size; chunk += chunk_size) {
        memcpy(&data, text_buffer + chunk, chunk_size);
        valve[data]++;
    }

    return valve;
}

char *compress(const char *text_buffer, size_t text_size,
               size_t *compressed_size) {
    size_t chunk_size = 1, chunk_capacity = 1 << (chunk_size << 3);

    typedef struct _entry {
        size_t data;
        unsigned weight;
        struct _entry *left;
        struct _entry *right;
    } Entry;

    Vector *vector = vector_new(1, sizeof(Entry));
    unsigned *chunks =
        count_chunks(text_buffer, text_size, chunk_size, chunk_capacity);
    for (size_t data = 0; data < chunk_capacity; data++) {
        if (!chunks[data]) continue;
        Entry entry = {data, chunks[data], NULL, NULL};
        vector_push(vector, &entry);
    }
    free(chunks);

    size_t actual_capacity = vector_size(vector);

    Heap *heap = heap_create(sizeof(Entry *), chunk_capacity, heap_formatter);
    for (size_t index = 0; index < actual_capacity; index++) {
        Entry *entry = vector->elems + index * vector->elem_size;
        heap_insert(heap, &entry);
    }

    while (heap->size > 1) {
        Entry *a, *b;
        heap_pop(heap, &a);
        heap_pop(heap, &b);
        Entry new = {a->weight + b->weight, a, b};
        vector_push(vector, &new);
        Entry *in_vector =
            vector->elems + (vector->size - 1) * vector->elem_size;
        heap_insert(heap, &in_vector);
    }

    Entry *result;
    heap_pop(heap, &result);
    printf("%u\n", result->weight);

    return NULL;
}

// char *decompress(const char *compressed_data, size_t compressed_size,
//                  size_t *uncompressed_size) {
//     size_t out_size = *(size_t *)compressed_data;
//     char *out = (char *)malloc(out_size);
//     memcpy(out, compressed_data + sizeof(size_t), out_size);
//     *uncompressed_size = out_size;
//     return out;
// }

int main() {
    char *text = strdup("abra shvabra cadabra");
    size_t compressed_size;
    char *compressed_text = compress(text, strlen(text), &compressed_size);

    // size_t uncompressed_size;
    // char *uncompressed_text =
    //     decompress(compressed_text, compressed_size, &uncompressed_size);
    // assert(uncompressed_size == strlen(text));
    // assert(memcmp(text, uncompressed_text, uncompressed_size) == 0);
    // free(uncompressed_text);
    free(compressed_text);
    free(text);
    return 0;
}

// int main() {
//     size_t index = 0;
//     unsigned char a = 1;
//     unsigned short b = 1234;
//     memcpy(&index, &b, 2);
//     printf("%zu\n", index);
//     return 0;
// }