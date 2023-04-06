/*

Наибольшая возрастающая последовательность

*/

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEG(x) -(x)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) NEG(MIN(NEG(x), NEG(y)))

void xlas(int N, int* array) {
    int* cache = calloc(N + 1, sizeof(int));
    int *current = calloc(N + 1, sizeof(int)), *previous = calloc(N + 1, sizeof(int));

    cache[0] = current[0] = INT32_MIN;
    for (size_t i = 1; i < N + 1; i++) cache[i] = INT32_MAX;

    int index = 0, max_length = 0;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++)
            if (cache[j] > array[i]) {
                index = j;
                break;
            }
        if (cache[index - 1] < array[i] && array[i] < cache[index]) {
            current[index] = i;
            previous[i] = current[index - 1];
            max_length = MAX(max_length, index);
            cache[index] = array[i];
        }
    }

    int* result = calloc(N + 1, sizeof(int));
    int size = 0;
    for (int answer = current[max_length]; answer != INT32_MIN; answer = previous[answer], size++) {
        result[size] = answer;
    }

    printf("%d\n", size);

    for (size_t i = 0; i < size; i++) {
        printf("%d ", result[size - i - 1]);
    }

    free(cache);
    free(current);
    free(previous);
    free(result);
}

int main(int argc, char* argv[]) {
    int N;
    scanf("%d", &N);

    int* array = calloc(N, sizeof(int));
    for (size_t i = 0; i < N; i++) {
        scanf("%d", array + i);
    }

    xlas(N, array);

    free(array);

    return 0;
}

//