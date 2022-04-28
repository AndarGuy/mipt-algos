/*

Поиск медиан

Медиана — элемент массива, который находился бы в этом массиве после сортировки
на среднем месте. При нумерации элементов массива размером N с единицы, номер
медианного элемента вычисляется по выражению (N+1)/2.

Известно, что имеются алгоритмы поиск медианы в массиве с линейной сложностью.
Вам предстоит решать эту задачу много раз, поэтому стоило бы уменьшить сложность
этой операции.

*/

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
    int temp;
    temp = ((int *)heap->data)[a];
    ((int *)heap->data)[a] = ((int *)heap->data)[b];
    ((int *)heap->data)[b] = temp;
}

void heap_peek(Heap *heap, void *data) {
    *((int *)data) = ((int *)heap->data)[1];
}

void heap_insert(Heap *heap, void *data) {
    if (heap->size < heap->capacity - 1) heap->size++;
    ((int *)heap->data)[heap->size] = *((int *)data);
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
    heap_peek(heap, data);
    heap_swap(heap, 1, heap->size);
    heap_sift_down(heap, 1);
    heap->size--;
}

void heap_sift_down(Heap *heap, size_t index) {
    int temp = ((int *)heap->data)[index];
    size_t choice = index;
    while (true) {
        size_t left = choice + choice, right = left + 1;
        if (left < heap->size &&
            heap->comparator(heap->data + left * heap->data_size, &temp) > 0)
            choice = left;
        if (right < heap->size &&
            heap->comparator(heap->data + right * heap->data_size,
                             heap->data + choice * heap->data_size) > 0)
            choice = right;
        if (choice == index) break;
        ((int *)heap->data)[index] = ((int *)heap->data)[choice];
        ((int *)heap->data)[choice] = temp;
        index = choice;
    }
}

Heap *heap_build(void *array, size_t size, size_t data_size, size_t capacity,
                 int (*comparator)(void *, void *)) {
    Heap *heap = heap_create(data_size, capacity, comparator);
    heap->size = size;
    memcpy(heap->data + data_size, array, size * data_size);
    for (size_t i = 0; i < size / 2 + 1; i++) {
        heap_sift_down(heap, size / 2 - i);
    }
    return heap;
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

int comparator_lesser(void *a, void *b) {
    if (*((int *)b) > *((int *)a)) return 1;
    return -1;
}

int comparator_greater(void *a, void *b) {
    if (*((int *)a) > *((int *)b)) return 1;
    return -1;
}

int main(int argc, char *argv[]) {
    size_t N;
    scanf("%zu", &N);

    Heap *min = heap_create(sizeof(int), N, comparator_greater),
         *max = heap_create(sizeof(int), N, comparator_lesser);

    enum COMMAND { ADD, MEDIAN };
    size_t command;
    for (size_t i = 0; i < N; i++) {
        scanf("%zu", &command);
        if (command == ADD) {
            int element, peek;
            scanf("%d", &element);

            heap_peek(max, &peek);
            if (element > peek) {
                heap_insert(max, &element);
                if (max->size - min->size > 0) {
                    heap_pop(max, &peek);
                    heap_insert(min, &peek);
                }
                continue;
            }

            heap_peek(min, &peek);
            if (element < peek) {
                heap_insert(min, &element);
                if (min->size - max->size > 1) {
                    heap_pop(min, &peek);
                    heap_insert(max, &peek);
                }
                continue;
            }

            if (min->size > max->size) {
                heap_insert(max, &element);
            } else {
                heap_insert(min, &element);
            }

        } else if (command == MEDIAN) {
            int median;
            heap_peek(min, &median);
            printf("%d\n", median);
        }
    }

    heap_destroy(min);
    heap_destroy(max);

    return 0;
}