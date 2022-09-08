/*

Очень быстрая сортировка

Уметь сортировать быстро – полезный навык. Стандартные сортировки в языках Си и
C++ достаточно быстры и универсальны. К сожалению, их универсальность имеет
недостаток: сложность алоритмов этих сортировок составляет O(N log(N)).

Между тем известно, что для некоторых типов данных имеются и сортировки со
сложность по времени O(N).

Вам и предстоит такую написать.

*/

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int get_chunk(unsigned number, unsigned chunk, unsigned chunk_size,
                       unsigned *masks) {
    return (masks[chunk] & number) >> (chunk * chunk_size);
}

void generate_masks(unsigned *masks, unsigned chunks, unsigned chunk_size) {
    for (size_t i = 0; i < chunks; i++) {
        unsigned mask = 0;
        for (size_t j = 0; j < chunk_size; j++) {
            mask |= 1 << (i * chunk_size + j);
        }
        masks[i] = mask;
    }
}

void radix_sort(unsigned *begin, unsigned *valve, size_t size, unsigned chunk,
                unsigned chunk_size, unsigned *masks) {
    unsigned power = 1 << chunk_size;
    unsigned *count = calloc(power, sizeof(unsigned));
    for (size_t i = 0; i < size; i++) {
        count[get_chunk(begin[i], chunk, chunk_size, masks) + 1]++;
    }

    for (size_t i = 1; i < power; i++) {
        count[i] += count[i - 1];
    }

    for (size_t i = 0; i < size; i++) {
        valve[count[get_chunk(begin[i], chunk, chunk_size, masks)]++] =
            begin[i];
    }

    memcpy(begin, valve, size * sizeof(unsigned));

    free(count);
}

void fast_sort(unsigned *begin, unsigned *end) {
    unsigned chunk_size = 16,
             chunks = sizeof(unsigned) * 8 / chunk_size;  // 8 bit per chunk
    unsigned *masks = calloc(chunks, sizeof(unsigned));
    generate_masks(masks, chunks, chunk_size);
    size_t size = end - begin;
    unsigned *valve = calloc(size, sizeof(unsigned));

    for (size_t chunk = 0; chunk < chunks; chunk++) {
        radix_sort(begin, valve, size, chunk, chunk_size, masks);
    }

    free(valve);
    free(masks);
}

int main() {
    unsigned array[8] = {1, 20, 10, 40, 100, 203, 42, 12};
    fast_sort(array, array + 8);
    for (size_t i = 0; i < 8; i++) {
        printf("%d ", array[i]);
    }

    // Now array = {1,1,2,3,4,5,6,9}
}