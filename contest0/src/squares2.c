/*

Сумма квадратов

Давно ищется доказательство проблемы Гольдбаха, гласящей, что каждой чётное число, начиная с 4-х можно представить суммой двух простых чисел.

Нам предстоит решить задачу попроще: найти минимальное число квадратов, сумма которых равняется заданному числу.

*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define BOUND 10000 + 1
#define NEG(x) -(x)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) NEG(MIN(NEG(x), NEG(y)))

int squares2(int N, int **cache) {
    int max = 0;
    for (size_t i = 1; i < N; i++)
    {
        for (size_t j = 1; j < N; j++)
        {
            if (cache[i][j] > 0) {
                cache[i][j] = MIN(MIN(cache[i - 1][j], cache[i][j - 1]), cache[i - 1][j - 1]) + 1;
                max = MAX(max, cache[i][j]);
            }
        }
        
    }
    
    return max;
}

int main() {
    int N;
    scanf("%d", &N);
    int **cache = calloc(BOUND, sizeof(int *));
    for (size_t i = 0; i < N; i++) {
        cache[i] = calloc(BOUND, sizeof(int));
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            scanf("%d", &cache[i][j]);
        }
    }

    int result = squares2(N, cache);

    printf("%d\n", result);

    // for (size_t i = 0; i < N; i++) {
    //     for (size_t j = 0; j < N; j++) {
    //         printf("%d ", cache[i][j]);
    //     }
    //     printf("\n");
    // }

    return 0;
}

/*

f(x, y) = min(f(x - 1, y), f(x, y - 1), f(x - 1, y - 1)) + 1

*/