/*

Самый тяжёлый прямоугольник

В прямоугольнике с целочисленными координатами вершин N×M находятся числа от -109 до 109.

Назовём весом прямоугольника со сторонами, параллельными осям координат, сумму всех чисел, которые лежат внутри него.

Найти вес самого тяжёлого из прямоугольников, которые можно построить на основе оригинального.

*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEG(x) -(x)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) NEG(MIN(NEG(x), NEG(y)))

void heaviest(int N, int M, long long **cache) {
    for (size_t i = 1; i <= M; i++) {
        cache[1][i] += cache[1][i - 1];
    }

    for (size_t i = 1; i <= N; i++) {
        cache[i][1] += cache[i - 1][1];
    }

    for (size_t i = 2; i <= N; i++) {
        for (size_t j = 2; j <= M; j++) {
            cache[i][j] += cache[i - 1][j] + cache[i][j - 1] - cache[i - 1][j - 1];
        }
    }

    // for (size_t i = 0; i <= N; i++) {
    //     for (size_t j = 0; j <= M; j++) {
    //         printf("%d ", cache[i][j]);
    //     }
    //     printf("\n");
    // }

    long long max = INT64_MIN;
    for (size_t i = 0; i <= N; i++) {
        for (size_t j = 0; j <= M; j++) {
            for (size_t k = i; k <= N; k++) {
                for (size_t l = j; l <= M; l++) {
                    long long sum = cache[k][l] - cache[k][j] - cache[i][l] + cache[i][j];
                    max = sum > max ? sum : max;
                }
            }
        }
    }

    printf("%lld\n", max);
}

int main(int argc, char *argv[]) {
    int N, M;
    scanf("%d%d", &N, &M);

    long long **cache = calloc(N + 1, sizeof(long long *));
    for (size_t i = 0; i < N + 1; i++) {
        cache[i] = calloc(M + 1, sizeof(long long));
    }

    for (size_t i = 1; i <= N; i++) {
        for (size_t j = 1; j <= M; j++) {
            scanf("%lld", &cache[i][j]);
        }
    }

    heaviest(N, M, cache);

    return 0;
}