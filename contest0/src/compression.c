
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

int vector_contains(struct vector *v, const void *);

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

int vector_contains(struct vector *v, const void *what) {
    size_t diff = what - v->elems;
    return diff >= 0 && diff <= v->size * v->elem_size;
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

// --------------------------------------------------------- //

struct set *set_new(size_t capacity);

int set_insert(struct set *s, size_t elem);

int set_erase(struct set *s, size_t elem);

int set_find(struct set const *s, size_t elem);

struct set *set_delete(struct set *s);

int set_empty(struct set const *s);

ssize_t set_findfirst(struct set const *s, size_t start);

size_t set_size(struct set const *s);

typedef struct set {
    char *array;
    size_t capacity;
} Set;

struct set *set_new(size_t capacity) {
    struct set *s = (struct set *)malloc(sizeof(struct set));
    s->capacity = capacity;
    s->array = (char *)calloc(s->capacity / 8 + 1, sizeof(char));
    return s;
}

int set_insert(struct set *s, size_t elem) {
    if (elem >= s->capacity) return 1;
    s->array[elem / 8] |= (1 << (elem % 8));
    return 0;
}

int set_erase(struct set *s, size_t elem) {
    if (elem >= s->capacity) return 1;
    s->array[elem / 8] &= (unsigned char)~((char)(1 << (elem % 8)));
    return 0;
}

int set_find(struct set const *s, size_t elem) {
    return s->array[elem / 8] & (1 << (elem % 8));
}

struct set *set_delete(struct set *s) {
    free(s->array);
    free(s);
    return NULL;
}

int set_empty(struct set const *s) { return set_findfirst(s, 0) != -1; }

ssize_t set_findfirst(struct set const *s, size_t start) {
    for (int i = start; i < s->capacity; i++) {
        if (set_find(s, i)) return i;
    }
    return -1;
}

size_t set_size(struct set const *s) {
    size_t size = 0;
    for (int i = 0; i < s->capacity; i++) {
        if (set_find(s, i)) size++;
    }

    return size;
}

// --------------------------------------------------------- //

typedef struct _entry {
    unsigned weight;
    unsigned left;
    unsigned right;
} Entry;

typedef struct _last_entry {
    unsigned weight;
    unsigned data;
} LastEntry;

typedef struct _encode {
    unsigned data;
    Set *encoding;
} Encode;

int heap_formatter(void *a, void *b) {
    return (*((Entry **)b))->weight - (*((Entry **)a))->weight;
}

int dict_comparator(const void *a, const void *b) {
    return *((unsigned *)a) - *((unsigned *)b);
}

unsigned *count_chunks(const char *text_buffer, size_t text_size,
                       size_t chunk_size, size_t chunk_capacity) {
    assert(chunk_size <= sizeof(size_t));

    unsigned *valve = calloc(chunk_capacity, sizeof(unsigned));

    size_t data = 0;
    for (size_t chunk = 0; chunk < text_size; chunk += chunk_size) {
        memcpy(&data, text_buffer + chunk, chunk_size);
        valve[data]++;
    }

    return valve;
}

void create_dictionary(Vector *dictionary, Vector *entries, Vector *data,
                       size_t actual_size, unsigned index, Set *encoding,
                       unsigned length) {
    if (index < actual_size) {
        Set *encoded = set_new(length);
        memcpy(encoded->array, encoding->array, length / 8 + 1);
        Encode encode = {index, encoded};
        // printf("• dictionary record for '%c' – %d\n",
        //        *((char *)data->elems + index * data->elem_size),
        //        encoded->array[0]);
        vector_push(dictionary, &encode);
        return;
    }

    Entry *entry = entries->elems + (index - actual_size) * entries->elem_size;

    set_erase(encoding, length);
    create_dictionary(dictionary, entries, data, actual_size, entry->left,
                      encoding, length + 1);
    set_insert(encoding, length);
    create_dictionary(dictionary, entries, data, actual_size, entry->right,
                      encoding, length + 1);
}

Set *search_dictionary(Vector *dictionary, Vector *data, void *target) {
    unsigned left = 0, right = vector_size(dictionary);

    printf("--- SEARCH DICTIONARY ---\n");

    printf("Trying to find '%c%c'...\n", *((char *)target),
           *((char *)target + 1));

    while (right - left > 0) {
        unsigned index = (left + right) / 2;
        Encode *encode = dictionary->elems + index * dictionary->elem_size;
        printf(
            "• iteration: right – %u, left – %u, index – %u, occured - "
            "'%c%c'\n ",
            right, left, index,
            *((char *)data->elems + encode->data * data->elem_size),
            *((char *)data->elems + encode->data * data->elem_size + 1));
        int comparison = memcmp(data->elems + encode->data * data->elem_size,
                                target, data->elem_size);
        if (comparison > 0) {
            right = index;
        } else if (comparison < 0) {
            left = index;
        } else {
            return encode->encoding;
        }
    }
    return NULL;
}

Entry *haffman(Vector *entries, Vector *bottom_entries,
               size_t actual_capacity) {
    // Pour bottom entries to heap
    Heap *heap = heap_create(sizeof(void *), actual_capacity, heap_formatter);
    for (size_t index = 0; index < actual_capacity; index++) {
        LastEntry *entry =
            bottom_entries->elems + index * bottom_entries->elem_size;
        // printf("• entry data %u, weight %u\n", entry->data, entry->weight);
        heap_insert(heap, &entry);
    }

    LastEntry *peek;
    heap_peek(heap, &peek);
    printf("Peek weight: %u\n", peek->weight);
    printf("Peek data: %u\n", peek->data);

    vector_resize(entries, actual_capacity);
    entries->size = 0;

    while (heap->size > 1) {
        LastEntry *a, *b;
        heap_pop(heap, &a);
        heap_pop(heap, &b);
        Entry new;
        new.weight = a->weight + b->weight;
        new.left = vector_contains(bottom_entries, a)
                       ? a->data
                       : actual_capacity + ((((void *)a) - entries->elems) /
                                            entries->elem_size);
        new.right = vector_contains(bottom_entries, b)
                        ? b->data
                        : actual_capacity + ((((void *)b) - entries->elems) /
                                             entries->elem_size);
        // printf("• merging entries: (%u, %u) and (%u, %u)\n", a->weight,
        //        new.left, b->weight, new.right);
        vector_push(entries, &new);
        Entry *in_vector =
            entries->elems + (entries->size - 1) * entries->elem_size;
        heap_insert(heap, &in_vector);
    }

    Entry *result;
    heap_peek(heap, &result);
    heap_destroy(heap);
    return result;
}

// encode text from text_buffer using dictionary
Set *encode(const char *text_buffer, size_t chunk_size, size_t text_size,
            Vector *dictionary, Vector *data) {
    printf("--- ENCODE ---\n");
    Set *bits = set_new(text_size * 8);
    size_t counter = 0;
    for (size_t chunk = 0; chunk < text_size; chunk += chunk_size) {
        Set *temp =
            search_dictionary(dictionary, data, (void *)text_buffer + chunk);
        for (size_t bit = 0; bit < temp->capacity; bit++) {
            if (set_find(temp, bit)) set_insert(bits, counter);
            counter++;
        }
        // printf("• encoding chunk #%zu\n", chunk);
    }
    bits->capacity = counter;
    return bits;
}

char *compress(const char *text_buffer, size_t text_size,
               size_t *compressed_size) {
    printf("--- COMPRESSOR ---\n");
    size_t chunk_size = 2, chunk_capacity = 1 << (chunk_size << 3);

    // printf("Compressing string: %s\n", text_buffer);
    printf("Chunk size: %zu\n", chunk_size);
    printf("Alphabet size: %zu\n", chunk_capacity);
    Vector *actual_data = vector_new(0, chunk_size);
    Vector *bottom_entries = vector_new(0, sizeof(LastEntry));
    Vector *entries = vector_new(0, sizeof(Entry));
    unsigned *chunks =
        count_chunks(text_buffer, text_size, chunk_size, chunk_capacity);
    printf("Counting symbols...\n");
    for (size_t data = 0; data < chunk_capacity; data++) {
        if (!chunks[data]) continue;
        printf("• symbol '%c%c', occured %u times\n", *((char *)&data),
               *((char *)&data + 1), chunks[data]);
        vector_push(actual_data, &data);
        LastEntry entry = {chunks[data], vector_size(actual_data) - 1};
        vector_push(bottom_entries, &entry);
    }
    free(chunks);

    size_t actual_capacity = vector_size(actual_data);
    printf("Actual alphabet size: %zu\n", actual_capacity);

    haffman(entries, bottom_entries, actual_capacity);
    bottom_entries = vector_delete(bottom_entries);
    // printf("%u\n", result->weight);

    Vector *dictionary = vector_new(0, sizeof(Encode));

    Set *temp = set_new(512);
    create_dictionary(dictionary, entries, actual_data, actual_capacity,
                      actual_capacity + vector_size(entries) - 1, temp, 0);
    set_delete(temp);
    qsort(dictionary->elems, vector_size(dictionary), dictionary->elem_size,
          dict_comparator);
    printf("[");
    for (size_t i = 0; i < vector_size(dictionary); i++) {
        Encode encode;
        vector_get(dictionary, i, &encode);
        // printf(
        //     "%c%c",
        //     *(actual_data->elems + encode.data * actual_data->elem_size),
        //     *(actual_data->elems + encode.data * actual_data->elem_size +
        //     1));
    }
    printf("]\n");

    printf("Created dictionary with size: %zu\n", vector_size(dictionary));

    Set *encoded =
        encode(text_buffer, chunk_size, text_size, dictionary, actual_data);

    printf("Encoded/decoded length – %zu/%zu\n", encoded->capacity,
           text_size * 8);

    // copy everything to buffer
    size_t data_size = actual_data->size * actual_data->elem_size,
           heap_size = entries->size * entries->elem_size,
           encoded_size = encoded->capacity / 8 + 1, number_of_secitons = 3,
           section_size = sizeof(unsigned);
    size_t copied = 0;
    void *compressed = malloc(data_size + heap_size + encoded_size +
                              section_size * number_of_secitons);
    // copy the chunk size
    memcpy(compressed + copied, &chunk_size, section_size);
    copied += section_size;
    // copy the data
    memcpy(compressed + copied, &data_size, section_size);
    copied += section_size;
    memcpy(compressed, actual_data->elems, data_size);
    copied += data_size;
    // copy the heap
    memcpy(compressed + copied, &heap_size, section_size);
    copied += section_size;
    memcpy(compressed, entries->elems, heap_size);
    copied += heap_size;
    // copy the encoded string
    memcpy(compressed + copied, &encoded_size, section_size);
    copied += section_size;
    memcpy(compressed + copied, encoded->array, encoded_size);
    copied += encoded_size;

    printf("--- SUMMARY ---\n");
    printf("----------------------------------------------------------\n");
    printf("| chunk_size | heap_size | heap | encoded_size | encoded |\n");
    printf("----------------------------------------------------------\n");
    printf("| %10zu | %9zu | %4zu | %12zu | %7zu |\n", section_size,
           section_size, data_size + heap_size, section_size, encoded_size);
    printf("----------------------------------------------------------\n");

    *compressed_size = copied;

    return compressed;
}

// char *decompress(const char *compressed_data, size_t compressed_size,
//                  size_t *uncompressed_size) {
//     typedef struct _section {
//         unsigned size;
//         void *section;
//     } Section;

//     printf("--- DECOMPRESSOR ---\n");

//     size_t data_size, heap_size, encoded_size, section_size =
//     sizeof(unsigned); size_t scanned = 0; memcpy(&data_size, compressed_data
//     + scanned, section_size); scanned += section_size; const char *data =
//     compressed_data + scanned; scanned += data_size; memcpy(&heap_size,
//     compressed_data + scanned, section_size); scanned += section_size; void
//     *heap;
// }

int main() {
    FILE *input = fopen("input.txt", "r");
    Vector *text = vector_new(0, sizeof(char));
    char c;
    while ((c = fgetc(input)) > 0) {
        vector_push(text, &c);
    }
    size_t compressed_size;
    size_t non_compressed_size = strlen(text->elems);
    char *compressed_text =
        compress(text->elems, strlen(text->elems), &compressed_size);
    printf("Compressed/non-compressed – %zu/%zu (-%0.1f%%)\n", compressed_size,
           non_compressed_size,
           ((float)compressed_size) / non_compressed_size * 100);

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