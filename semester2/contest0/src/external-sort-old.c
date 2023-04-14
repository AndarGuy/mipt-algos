/*

В файле "input.txt" содержатся строки символов, длина каждой строки не превышает
10000 байт. Файл нужно отсортировать в лексикографическом порядке и вывести
результат в файл "output.txt". Вот беда, файл занимает много мегабайт, а в вашем
распоряжение оказывается вычислительная система с памятью всего в 256 килобайт.
Но файл должен быть отсортирован!

*/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct stack {
    size_t top;
    size_t bottom;
    size_t capacity;
    size_t elem_size;
    void *elems;
} Stack;

struct stack *stack_new(size_t elem_size) {
    assert(elem_size);
    const int INITIAL_SIZE = 1;
    struct stack *st = malloc(sizeof(struct stack));
    st->capacity = INITIAL_SIZE;
    st->top = 0;
    st->bottom = 0;
    st->elem_size = elem_size;
    st->elems = calloc(INITIAL_SIZE, elem_size);
    return st;
}

int stack_push(struct stack *st, const void *elem) {
    assert(st);
    assert(elem);
    if (st->top >= st->capacity) {
        void *pointer = realloc(st->elems, st->elem_size * (st->capacity *= 2));
        if (!pointer) return 1;
        st->elems = pointer;
    }
    memcpy(st->elems + st->top * st->elem_size, elem, st->elem_size);
    st->top++;
    return 0;
}

int stack_empty(struct stack const *st) {
    assert(st);
    return st->top == 0;
}

int stack_top(struct stack *st, void *elem) {
    assert(st);
    assert(elem);
    if (stack_empty(st)) return 1;
    memcpy(elem, st->elems + (st->top - 1) * st->elem_size, st->elem_size);
    return 0;
}

int stack_pop(struct stack *st, void *elem) {
    assert(st);
    assert(elem);
    if (stack_empty(st)) return 1;
    stack_top(st, elem);
    st->top--;
    return 0;
}

struct stack *stack_delete(struct stack *st) {
    assert(st);
    free(st->elems);
    free(st);
    return NULL;
}

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
        // hard choice to make, which will be swapped?
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

#define LINE_SIZE 10001

char buffer_fn[64];

FILE *storages[10];

int comparator(const void *s1, const void *s2) {
    return strcmp((const char *)s1, (const char *)s2);
}

int heap_formatter(void *a, void *b) {
    return strcmp(*((const char **)b), *((const char **)a));
}

char *get_storage_filename(size_t chunk) {
    sprintf(buffer_fn, "chunk%zu.txt", chunk);
    return buffer_fn;
}

FILE *get_storage(size_t chunk, char *mode) {
    FILE *file = fopen(get_storage_filename(chunk), mode);
    return file;
}

size_t merge(size_t *chunks, size_t size, void *buffer) {
    struct HeapData {
        char *line;
        size_t chunk;
    };

    Heap *heap = heap_create(sizeof(struct HeapData), size, heap_formatter);

    for (size_t i = 0; i < size; i++) {
        storages[i] = get_storage(chunks[i], "r");
        struct HeapData data = {buffer + i * LINE_SIZE, i};
        if (fgets(buffer + i * LINE_SIZE, LINE_SIZE, storages[i]) != NULL)
            heap_insert(heap, &data);
    }
    FILE *temp = fopen("temp.txt", "w");

    while (heap->size > 0) {
        struct HeapData data;
        heap_pop(heap, &data);
        fputs(data.line, temp);
        if (!feof(storages[data.chunk]) &&
            fgets(data.line, LINE_SIZE, storages[data.chunk]) != NULL) {
            heap_insert(heap, &data);
        }
    }

    for (size_t i = 0; i < size; i++) {
        fclose(storages[i]);
        remove(get_storage_filename(chunks[i]));
    }

    fclose(temp);
    heap_destroy(heap);

    rename("temp.txt", get_storage_filename(chunks[0]));

    return chunks[0];
}

void extern_sort() {
    FILE *input = fopen("input.txt", "r");

    const size_t memory_limit = 250 * 1024;
    // const char character_from = '!', character_to = '~';
    const size_t chunk_capacity = memory_limit / LINE_SIZE;
    const size_t merge_threshold = chunk_capacity / 2;

    Stack *stack = stack_new(sizeof(size_t));
    char *buffer = calloc(memory_limit, sizeof(char));
    size_t levels[5] = {0};
    for (size_t i = 0; !feof(input); i++) {
        size_t chunk = stack->top;
        // printf("%zu\n", chunk);
        size_t line;
        for (line = 0; line < chunk_capacity; line++) {
            if (!fgets(buffer + line * LINE_SIZE, LINE_SIZE, input)) break;
        }
        size_t lines = line;
        qsort(buffer, lines, LINE_SIZE * sizeof(char), comparator);

        FILE *temp = get_storage(chunk, "w");
        for (line = 0; line < lines; line++) {
            fputs(buffer + line * LINE_SIZE, temp);
        }
        fclose(temp);

        stack_push(stack, &chunk);
        levels[0]++;

        for (size_t i = 0; i < 5; i++) {
            if (levels[i] == merge_threshold) {
                stack->top -= merge_threshold;
                size_t merged =
                    merge(stack->elems + stack->top * stack->elem_size,
                          merge_threshold, buffer);
                stack_push(stack, &merged);
                levels[i + 1]++;
                levels[i] = 0;
            }
        }
    }

    size_t merged = merge(stack->elems, stack->top, buffer);

    rename(get_storage_filename(merged), "output.txt");

    free(buffer);
    fclose(input);
    stack_delete(stack);
}

int main() {
    clock_t timer = clock();

    extern_sort();

    printf("Time taken: %.4fs\n", (double)(clock() - timer) / CLOCKS_PER_SEC);

    return 0;
}