/*

Разрядная сортировка

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void counting_sort(int *array, int size, int radix_index) {
    int **count = calloc(10, sizeof(int *));
}

int get_max(int *array, int size) {
    int max = 0;
    for (size_t i = 0; i < size; i++) {
        max = max > array[i] ? max : array[i];
    }
    return max;
}

int get_radix(int number, int base) {
    int radix = 0;
    for (; number > 0; radix++, number /= base) {
    }
    return radix;
}

void radix_sort(int *array, int size) {
    int max_radix = get_radix(get_max(array, size), 10);
    for (size_t i = 0; i < max_radix; i++) {
        /* code */
    }
}

int main(int argc, char *argv[]) {
    /* code */

    return 0;
}