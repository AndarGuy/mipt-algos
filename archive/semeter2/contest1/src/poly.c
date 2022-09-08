/*

Вычисление полинома

Быстрое вычисление полинома — важная составляющая многих алгоритмов.

В данной задаче нужно вычислить значение полинома anxn+an-1xn-1+...+a2x2+a1x1+a0

Так как степени и коэффициенты могут быть достаточно большими, чтобы результат
был представим в виде элементарного числа, будем вычислять вычислить значение
полинома по модулю MOD. Сделать это предлагается для нескольких значений
аргумента.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    unsigned long N = 0, M = 0, MOD = 0;
    scanf("%ld%ld%ld", &N, &M, &MOD);
    unsigned long *coeficients = calloc(N + 1, sizeof(unsigned long));
    // unsigned long *arguments = calloc(M, sizeof(unsigned long));

    for (size_t i = 0; i < N + 1; i++) {
        scanf("%ld", coeficients + i);
    }

    for (size_t i = 0; i < M; i++) {
        unsigned long argument = 0, result = 0, power = 1;
        scanf("%ld", &argument);
        for (size_t j = 0; j < N + 1; j++) {
            unsigned long coeficient = coeficients[N - j];
            result = result + coeficient * power % MOD;
            result %= MOD;
            power *= argument;
            power %= MOD;
        }

        printf("%lu\n", result % MOD);
    }

    return 0;
}