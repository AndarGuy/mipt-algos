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
// #include <time.h>

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

int comparator(const void *s1, const void *s2) {
    return strcmp((const char *)s1, (const char *)s2);
}

int heap_formatter(void *a, void *b) {
    return strcmp(*((const char **)b), *((const char **)a));
}

char filename[64];
char *get_file(char *type, size_t number) {
    sprintf(filename, "%s%zu.txt", type, number);
    return filename;
}

void chunk_sort(FILE *where, size_t chunk_capacity, size_t files_limit,
                void *buffer) {
    FILE **storages = malloc(files_limit * sizeof(FILE *));
    for (size_t file = 0; file < files_limit; file++) {
        storages[file] = fopen(get_file("chunk", file), "w");
    }

    for (size_t chunk = 0; !feof(where); chunk++) {
        size_t line;
        for (line = 0; line < chunk_capacity; line++) {
            if (!fgets(buffer + line * LINE_SIZE, LINE_SIZE, where)) break;
        }
        size_t lines = line;

        qsort(buffer, lines, LINE_SIZE * sizeof(char), comparator);

        for (line = 0; line < lines; line++) {
            fputs(buffer + line * LINE_SIZE, storages[chunk % files_limit]);
        }
    }

    for (size_t file = 0; file < files_limit; file++) {
        fclose(storages[file]);
    }

    free(storages);
}

size_t merge_series(size_t series_size, size_t files_limit, void *buffer) {
    FILE **storages = malloc(files_limit * sizeof(FILE *)),
         **outputs = malloc(files_limit * sizeof(FILE *));

    Heap *heap = heap_create(sizeof(char *), files_limit, heap_formatter);

    for (size_t file = 0; file < files_limit; file++) {
        storages[file] = fopen(get_file("chunk", file), "r");
        outputs[file] = fopen(get_file("temp", file), "w");
    }

    size_t *cursors = malloc(files_limit * sizeof(size_t));
    size_t files_used = 0;
    for (size_t series = 0; true; series++) {
        heap->size = 0;
        for (size_t series = 0; series < files_limit; series++) {
            char *line = buffer + series * LINE_SIZE;
            if (fgets(line, LINE_SIZE, storages[series]) != NULL) {
                heap_insert(heap, &line);
                cursors[series] = 1;
            }
        }
        if (heap->size == 0) break;
        files_used = files_used > heap->size ? files_used : heap->size;
        while (heap->size > 0) {
            char *line;
            heap_pop(heap, &line);
            fputs(line, outputs[series % files_limit]);
            size_t chunk = (line - (char *)buffer) / LINE_SIZE;
            if (cursors[chunk] < series_size &&
                fgets(line, LINE_SIZE, storages[chunk]) != NULL) {
                heap_insert(heap, &line);
                cursors[chunk]++;
            }
        }
    }

    for (size_t i = 0; i < files_limit; i++) {
        fclose(storages[i]);
        fclose(outputs[i]);
        char *chunk_filename = malloc(64);
        strcpy(chunk_filename, get_file("chunk", i));
        rename(get_file("temp", i), chunk_filename);
        free(chunk_filename);
    }

    heap_destroy(heap);
    free(cursors);
    free(storages);
    free(outputs);

    return files_used;
}

void extern_sort() {
    FILE *input = fopen("input.txt", "r");

    const size_t memory_limit = 250 * 1024, files_limit = 10;
    // const char character_from = '!', character_to = '~';
    const size_t chunk_capacity = memory_limit / LINE_SIZE;

    char *buffer = calloc(memory_limit, sizeof(char));

    // printf("--- EXTERNAL SORT ---\n");
    // printf("Memory Limit - %zu B\n", memory_limit);
    // printf("Chunk Capacity - %zu\n", chunk_capacity);
    // printf("Using Files - %zu\n", files_limit);

    chunk_sort(input, chunk_capacity, files_limit, buffer);

    for (size_t files_used = files_limit, series_size = chunk_capacity;
         files_used > 1; series_size *= files_used) {
        files_used = merge_series(series_size, files_used, buffer);
    }

    rename("chunk0.txt", "output.txt");
}

int main() {
    
    extern_sort();

    return 0;
}