/*

Перемножение матриц

Известно, что матрицу A размером M × N и матрицу B размером N × K можно перемножить с получением матрицы C размером M × K, используя M*N*K операций перемножения индивидуальных элементов матриц.

Если мы перемножаем цепочку матриц A × B × C, то результат можно получить, как перемножая сначала A и B, затем результат умножить на C, так и перемножив сначала B и C, затем умножив матрицу A на результат.

A · B · C = (A · B) · C = A · (B · C)

Хотя результат от этого не меняется, количество операций может быть различно. Например, для матриц A(20,10), B(10,20), C(20,30) первый способ даст 20*10*20 + 20*20*30 = 16000 операций, а второй — 10*20*30 + 20*10*30 = 12000 операций.

Ваша задача — определить, сколько операций достаточно при оптимальном порядке перемножения матриц.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEG(x) -(x)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) NEG(MIN(NEG(x), NEG(y)))

long long matrices(long long **cache, int *sizes, int i, int j) {
    if (cache[i][j] == 0xFFFFFFFFFFFFFFF) {
        for (int k = i; k < j; k++) {
            long long temp =
                matrices(cache, sizes, k + 1, j) + matrices(cache, sizes, i, k) + sizes[j] * sizes[k] * sizes[i - 1];
            cache[i][j] = MIN(temp, cache[i][j]);
        }
    }
    return cache[i][j];
}

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);
    int *sizes = calloc(N, sizeof(int));

    for (size_t i = 0; i < N; i++) {
        scanf("%d", sizes + i);
    }

    long long **cache = calloc(N, sizeof(long long *));
    for (size_t i = 0; i < N; i++) {
        cache[i] = calloc(N, sizeof(long long));
        for (size_t j = 0; j < N; j++) {
            cache[i][j] = 0xFFFFFFFFFFFFFFF;
        }
        cache[i][i] = 0;
    }

    printf("%lld", matrices(cache, sizes, 1, N - 1));

    // for (size_t i = 0; i < N; i++) {
    //     for (size_t j = 0; j < N; j++) {
    //         printf("%lld ", cache[i][j]);
    //     }
    //     printf("\n");
    // }

    return 0;
}

//