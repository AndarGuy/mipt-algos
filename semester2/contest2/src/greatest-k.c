/*

Наибольшие из

На вход программы подаётся количество чисел в последовательности 10⩽N⩽10000000 и
число 5⩽K⩽10000.

Следующие N строк содержат числа от -10^18 до 10^18.

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
    // assert(heap->size < heap->capacity - 1);
    if (heap->size < heap->capacity - 1) heap->size++;
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
    heap_peek(heap, data);
    heap_swap(heap, 1, heap->size);
    heap_sift_down(heap, 1);
    heap->size--;
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

void heap_print(Heap *heap, void (*print_data)(void *)) {
    printf("[");
    for (size_t i = 0; i < heap->size; i++) {
        print_data(heap->data + (i + 1) * heap->data_size);
        if (i + 1 < heap->size) printf(", ");
    }
    printf("]\n");
}

// ---------------------------------------------------- //

int comparator_less(void *a, void *b) {
    if (*((long long int *)b) > *((long long int *)a))
        return 1;
    else
        return -1;
}

int comparator_great(const void *a, const void *b) {
    if (*((long long int *)a) < *((long long int *)b))
        return 1;
    else
        return -1;
}

void print_data(void *a) { printf("%lld", *((long long int *)a)); }

int main(int argc, char *argv[]) {
    int N, K;
    scanf("%d%d", &N, &K);
    Heap *heap = heap_create(sizeof(long long int), K, comparator_less);
    long long int temp, peek = 0;
    for (size_t i = 0; i < K; i++) {
        scanf("%lld", &temp);
        heap_insert(heap, &temp);
    }

    // heap_print(heap, print_data);

    for (size_t i = 0; i < N - K; i++) {
        scanf("%lld", &temp);
        heap_peek(heap, &peek);
        if (peek < temp) {
            heap_pop(heap, &peek);
            heap_insert(heap, &temp);
        }
    }

    // heap_print(heap, print_data);

    long long int *result = heap->data + sizeof(long long int);
    qsort(result, K, sizeof(long long int), comparator_great);

    for (size_t i = 0; i < K; i++) {
        printf("%lld\n", result[i]);
    }

    heap_destroy(heap);

    return 0;
}