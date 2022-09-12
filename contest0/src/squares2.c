/*

Сумма квадратов

Давно ищется доказательство проблемы Гольдбаха, гласящей, что каждой чётное число, начиная с 4-х можно представить суммой двух простых чисел.

Нам предстоит решить задачу попроще: найти минимальное число квадратов, сумма которых равняется заданному числу.

*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 10000 + 1

int squares2(int N, int *cache) {
    int max = 0;

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            scanf("%d", &cache[i][j]);
        }
    }

    return cache[N] = min + 1;
}

int main() {
    int N;
    scanf("%d", &N);
    int **cache = calloc(MAX, sizeof(int *));
    for (size_t i = 0; i < N; i++) {
        cache[i] = calloc(MAX, sizeof(int));
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            scanf("%d", &cache[i][j]);
        }
    }

    for (size_t i = 2; i < N; i++) {
        squares(i, cache);
    }

    int result = squares2(N, cache);

    printf("%d\n", result);

    return 0;
}

/*

f(x) = f(x - 1) + f(x - 2) + f(x - 3) + f(x - 4) + f(x - 5) + f(x - 6)

*/