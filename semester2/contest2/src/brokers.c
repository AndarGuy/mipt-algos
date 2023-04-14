/*

Брокеры

В стране Бурляндии фирма «Котлетный рай» имеет много отделений, работающих
сравнительно автономно. После неких экономических преобразований такая форма
функционирования оказалась невыгодной и фирма решила сливать капиталы отделений,
образуя укрупнённые департаменты, отвечающие за несколько отделений сразу. Цель
фирмы — слить все отделения в один громадный департамент, владеющий всеми
капиталами.

Первая проблема заключается в том, что по законам Бурляндии операция слияния
капиталов отделений должна проводиться государственной брокерской службой,
которая не может производить более одной операции слияния в одной фирме
одновременно.

Вторая проблема состоит в том, что брокерская служба берёт за свои услуги один
процент всех средств, получившихся в результате слияния двух подразделений. Для
выплаты таких процентов у фирмы имеется отдельный фонд и общий капитал фирмы не
меняется. Важно спланировать порядок операций слияния таким образом, чтобы фирма
потратила на слияние как можно меньшую сумму.

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
    void *temp = malloc(heap->data_size);
    memcpy(temp, heap->data + index * heap->data_size, heap->data_size);
    size_t choice = index;
    while (true) {
        size_t left = choice + choice, right = left + 1;
        if (left < heap->size &&
            heap->comparator(heap->data + left * heap->data_size, temp) > 0)
            choice = left;
        if (right < heap->size &&
            heap->comparator(heap->data + right * heap->data_size,
                             heap->data + choice * heap->data_size) > 0)
            choice = right;
        if (choice == index) break;
        memcpy(heap->data + index * heap->data_size,
               heap->data + choice * heap->data_size, heap->data_size);
        memcpy(heap->data + choice * heap->data_size, temp, heap->data_size);
        index = choice;
    }
    free(temp);
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

int comparator_less(void *a, void *b) {
    if (*((int *)b) > *((int *)a)) return 1;
    return -1;
}

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);

    Heap *heap = heap_create(sizeof(size_t), N, comparator_less);

    size_t temp;
    for (size_t i = 0; i < N; i++) {
        scanf("%zu", &temp);
        heap_insert(heap, &temp);
    }

    size_t operations = 0;
    while (heap->size != 1) {
        heap_pop(heap, &temp);
        size_t *peek = heap->data + heap->data_size;
        *peek += temp;
        operations += *peek;
        heap_sift_down(heap, 1);
    }

    printf("%.2f\n", operations / 100.0);

    return 0;
}