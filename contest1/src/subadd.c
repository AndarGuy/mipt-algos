/*

Суммы на подмассивах

Имеется массив A из 1⩽N⩽100000 чисел, 10-9⩽Ai⩽109.

К этому массиву Q раз, 1⩽Q⩽100000 применяют следующее: для каждого элемента
массива от элемента L до элемента R включительно прибавляют число X.

Требуется вывести этот массив после всех таких преобразований.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    long long N, Q;
    scanf("%lld%lld", &N, &Q);

    long long *num = calloc(N + 1, sizeof(long long));

    for (size_t i = 0; i < N; i++) {
        long long value;
        scanf("%lld", &value);
        num[i] += value;
        num[i + 1] -= value;
    }

    for (size_t i = 0; i < Q; i++) {
        long long L, R, X;
        scanf("%lld%lld%lld", &L, &R, &X);
        num[L] += X;
        num[R + 1] -= X;
    }

    long long current = 0;

    for (size_t i = 0; i < N; i++) {
        current += num[i];
        printf("%lld ", current);
    }

    return 0;
}