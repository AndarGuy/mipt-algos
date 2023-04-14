/*

Наибольшая сумма

Массивы a и b оба содержат по 4 ⩽ N ⩽ 200000 элементов. Каждый из элементов по
абсолютной величине не превосходит 1000000009.

Нужно найти максимальную их всех возможных сумм ai+bj. Вы скажете: задача
легчайшая, нужно просто найти максимальные значения массивов a и b и сложить их.

Добавим к задаче обязательное условие: индекс i в сумме ai+bj не должен
превосходить индекса j.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);
    int *a = calloc(N, sizeof(int)), *b = calloc(N, sizeof(int)),
        *c = calloc(N, sizeof(int)), *d = calloc(N, sizeof(int));

    for (size_t i = 0; i < N; i++) {
        scanf("%d", a + i);
    }

    for (size_t i = 0; i < N; i++) {
        scanf("%d", b + i);
    }

    // precount

    for (size_t i = 0; i < N; i++) {
        if (i > 0) c[i] = a[i] > a[c[i - 1]] ? i : c[i - 1];
    }

    d[N - 1] = N - 1;
    for (size_t i = 0; i < N; i++) {
        int index = N - i - 1;
        if (i > 0) d[index] = b[index] > b[d[index - 1]] ? index : d[index - 1];
    }

    // algorithm

    int max_i = -1, max_j = -1;
    int max_sum = -2147483648;
    for (size_t i = 0; i < N; i++) {
        if (a[c[i]] + b[d[i]] > max_sum) {
            max_sum = a[c[i]] + b[d[i]];
            max_i = c[i];
            max_j = d[i];
        }
    }

    printf("%d %d", max_i, max_j);

    return 0;
}