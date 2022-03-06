/*

Максимальная сумма подмассива

Имеется массив V целых чисел, состоящий из 1⩽N⩽108 элементов, −2×109⩽Vi⩽2×109.
Требуется найти наибольшую из возможных сумм всех подмассивов.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);

    long long max = -9223372036854775807LL;
    long long sum = 0;
    for (size_t i = 0; i < N; i++) {
        long long current = 0;
        scanf("%lld", &current);
        sum += current;
        max = max > sum ? max : sum;
        sum = sum > 0 ? sum : 0;
    }

    printf("%lld", max);

    return 0;
}