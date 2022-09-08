/*

Слияние

На вход подаётся число N, равное размерам двух неубывающих последовательностей.

Далее следуют N элементов первой последовательности и N элементов второй
последовательности.

Требуется вывести последние девять цифр суммы элементов последовательности,
которая получилась бы из всех элементов первой и второй последовательностей,
стоящих на нечётных местах.

Имеется ограничение по памяти, её хватит только на один массив. Итоговая
последовательность тоже не должна формироваться в памяти а учитываться по мере
получения её элементов.

В задаче, данной в качестве примера, итоговая слитая последовательность
получается следующей:

1 1 3 3 7 7 8 9 9 10

В качестве ответа берётся сумма 1 + 3 + 7 + 8 + 9 = 28

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);
    long long *A = malloc(sizeof(long long) * N);
    for (size_t i = 0; i < N; i++) {
        scanf("%lld", A + i);
    }

    size_t cursorA = 0, cursorB = 0;
    long long sum = 0, current;

    scanf("%lld", &current);
    cursorB++;

    for (size_t i = 0; cursorA < N || cursorB <= N; i++) {
        size_t iteration = cursorA + cursorB;
        if (cursorA >= N) {
            if (iteration % 2) sum += current % 1000000000;
            if (cursorB < N) scanf("%lld", &current);
            cursorB++;
        } else if (cursorB > N) {
            if (iteration % 2) sum += A[cursorA] % 1000000000;
            cursorA++;
        } else if (current < A[cursorA]) {
            if (iteration % 2) sum += current % 1000000000;
            scanf("%lld", &current);
            cursorB++;
        } else {
            if (iteration % 2) sum += A[cursorA] % 1000000000;
            cursorA++;
        }

        // printf("sum - %lld\n", sum);
    }

    printf("%lld", sum % 1000000000);

    return 0;
}