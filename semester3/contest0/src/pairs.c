/*

На плоскости расположены чётное число N точек, 2 ⩽ N ⩽ 26.

Требуется разбить все точки на пары таким образом, чтобы каждая точка
принадлежала ровно одной паре. Нужно найти такое разбиение,
чтобы сумма всех получившихся отрезков была минимальной.

*/

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEG(x) -(x)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) NEG(MIN(NEG(x), NEG(y)))

double _distance(long long *point_a, long long *point_b) {
    return sqrt((point_a[0] - point_b[0]) * (point_a[0] - point_b[0]) +
                (point_a[1] - point_b[1]) * (point_a[1] - point_b[1]));
}

double pairs(int N, size_t mask, long long **points, double *cache) {
    if (!mask) return 0;
    // check if mask has been already calculated
    if (cache[mask]) return cache[mask];

    cache[mask] = UINT64_MAX;
    size_t high = 63 - __builtin_clzll(mask);
    for (size_t temp = mask - (1 << high), low = 63 - __builtin_clzll(temp); temp; temp = temp - (1 << low), low = 63 - __builtin_clzll(temp)) {
        size_t new_mask = mask - (1 << high) - (1 << low);
        cache[mask] = MIN(cache[mask], _distance(points[high], points[low]) + pairs(N, new_mask, points, cache));
    }

    return cache[mask];
}

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);

    long long **points = calloc(N, sizeof(long long *));
    for (size_t i = 0; i < N; i++) {
        points[i] = calloc(2, sizeof(long long));
        scanf("%lld%lld", &points[i][0], &points[i][1]);
    }

    double *cache = calloc(1 << N, sizeof(double));

    size_t mask = (1ULL << N) - 1ULL;
    // printf("%zu\n", mask);
    printf("%.3lf\n", pairs(N, mask, points, cache));

    return 0;
}

//