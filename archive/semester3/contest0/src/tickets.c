/*

Счастливые билеты

Номер билета состоит из чётного числа цифр N в M-ричной системе счисления. Счастливым билетом называется билет, сумма цифр первой половины которого равна сумме цифр второй половины.

Для заданных N и M найти количество счастливых билетов. Учтите, их число может быть очень велико.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modules/bigint.h"

#define N_MAX 150
#define M_MAX 40

void tickets(int N, int M, BigInteger **cache) {
    for (size_t i = 0; i < M; i++) cache[0][i].storage[0] = 1;

    for (size_t i = 2; i <= N; i++) {
        for (size_t j = 0; j < i * M - 1; j++) {
            BigInteger sum = {0};
            for (size_t k = 0; k < M; k++) {
                if (k > j) continue;
                sum = big_integer_add(sum, cache[0][j - k]);
            }
            cache[1][j] = sum;
        }
        memcpy(cache[0], cache[1], sizeof(BigInteger) * N * M);
    }

    BigInteger sum = {0};
    for (size_t i = 0; i < N * M; i++) {
        sum = big_integer_add(sum, big_integer_multiply(cache[0][i], cache[0][i]));
    }

    big_integer_print(sum);
}

int main(int argc, char *argv[]) {
    int N, M;
    scanf("%d%d", &N, &M);
    N /= 2;
    BigInteger **cache = calloc(2, sizeof(BigInteger *));
    for (size_t i = 0; i < 2; i++) cache[i] = calloc(N_MAX * M_MAX, sizeof(BigInteger));

    tickets(N, M, cache);

    return 0;
}

// f(x, n + 1) = f(x, n) + f(x - 1, n) + ... + f(x - M, n)