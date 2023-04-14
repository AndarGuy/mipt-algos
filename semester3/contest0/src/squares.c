/*

Сумма квадратов

Давно ищется доказательство проблемы Гольдбаха, гласящей, что каждой чётное число, начиная с 4-х можно представить суммой двух простых чисел.

Нам предстоит решить задачу попроще: найти минимальное число квадратов, сумма которых равняется заданному числу.

*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 1000000 + 1

int squares(int N, int *cache) {
    int min = MAX;
    int bound = sqrt((double)N);
    for (size_t i = bound; i > 0; i--) {
        int previous = cache[N - i * i];
        if (previous < min) min = previous;
    }

    return cache[N] = min + 1;
}

int main() {
    int N;
    scanf("%d", &N);
    int *cache = calloc(MAX, sizeof(int));
    cache[0] = 0;
    for (size_t i = 1; i < sqrt(N); i++) {
        cache[i * i] = 1;
    }

    for (size_t i = 2; i < N; i++) {
        squares(i, cache);
    }

    int result = squares(N, cache);

    printf("%d\n", result);

    return 0;
}

/*

f(x) = f(x - 1) + f(x - 2) + f(x - 3) + f(x - 4) + f(x - 5) + f(x - 6)

*/