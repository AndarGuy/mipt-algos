/*

Наибольшая возрастающая последовательность

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int xlas(int N, int *array) {
}

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);

    int *array = calloc(N, sizeof(int));
    for (size_t i = 0; i < N; i++) {
        scanf("%d", array + i);
    }

    return 0;
}

//