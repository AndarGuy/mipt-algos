/*

* Description *

*/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        // printf("SWAP!\n");
        heap_swap(heap, i, i / 2);
    }
}

void heap_pop(Heap *heap, void *data) {
    assert(heap->size > 0);
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
        // printf("SWAP!\n");
        heap_swap(heap, index, choice);
        index = choice;
    }
}

void heap_print(Heap *heap, void (*print_data)(void *)) {
    printf("[");
    for (size_t i = 0; i < heap->size; i++) {
        print_data(heap->data + (i + 1) * heap->data_size);
        if (i + 1 < heap->size) printf(", ");
    }
    printf("]\n");
}

int comparator_less(void *a, void *b) {
    // printf("Compairing %d and %d!\n", *((int *)a), *((int *)b));
    return *((int *)b) - *((int *)a);
}

int comparator_great(void *a, void *b) { return *((int *)a) - *((int *)b); }

void print_data(void *data) { printf("%d", *((int *)data)); }

enum ComparatorType { LESSER, GREATER };

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);
    Heap **heaps = malloc(sizeof(Heap *) * N);

    unsigned command = 0;
    while (scanf("%d", &command) > 0) {
        int heap;
        scanf("%d", &heap);
        if (command == 0) {
            int capacity, type;
            scanf("%d%d", &capacity, &type);
            heaps[heap] = heap_create(
                sizeof(int), capacity,
                type == GREATER ? comparator_great : comparator_less);
        } else if (command == 1) {
            int elem;
            scanf("%d", &elem);
            heap_insert(heaps[heap], &elem);
        } else if (command == 2) {
            int elem;
            heap_peek(heaps[heap], &elem);
            printf("%d\n", elem);
        } else if (command == 3) {
            int elem;
            heap_pop(heaps[heap], &elem);
        } else if (command == 4) {
            printf("%zu\n", heaps[heap]->size);
        } else if (command == 5) {
            int src;
            scanf("%d", &src);
            heap_merge(heaps[heap], heaps[src]);
            heaps[src]->size = 0;
        } else if (command == 6) {
            heap_destroy(heaps[heap]);
        }
        // heap_print(heaps[heap], print_data);
    }

    free(heaps);

    return 0;
}