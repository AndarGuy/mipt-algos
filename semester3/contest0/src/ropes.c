/*

Верёвочки

В ось X вбито N гвоздей. Требуется соединить эти гвозди верёвочками так, чтобы у каждого гвоздя была хотя бы одна верёвочка и чтобы сумма всех верёвочек была минимальной.

2⩽N⩽10000

-40000⩽Xi⩽40000

*/

#include <stdio.h>
#include <stdlib.h>

#define MIN(A, B) A < B ? A : B

int comparator(const void *a, const void *b) {
    return *((int*) a) - *((int*) b);
}

int ropes(int N, int *nails) {
    int *cache = calloc(N + 1, sizeof(int));
    cache[2] = nails[1] - nails[0];
    if (N > 2) cache[3] = cache[2] + nails[2] - nails[1];

    for (size_t i = 4; i <= N; i++) {
        cache[i] = MIN(cache[i - 1] + nails[i - 1] - nails[i - 2], cache[i - 2] + nails[i - 1] - nails[i - 2]);
    }

    // for (size_t i = 0; i < N; i++)
    // {
    //     printf("%d ", cache[i]);
    // }

    int result = cache[N];
    free(cache);
    return result;
}

int main() {
    int N;
    scanf("%d", &N);

    int *nails = calloc(N, sizeof(int));
    for (int i = 0; i < N; i++) scanf("%d", nails + i);
    qsort(nails, N, sizeof(int), comparator);

    printf("%d", ropes(N, nails));

    free(nails);

    return 0;
}

/* 

f(x) = MIN(f(x - 1) + расстояние между новым гвоздем и последним, f(x - 2) + расстояние между новым гвоздем и последним)

*/