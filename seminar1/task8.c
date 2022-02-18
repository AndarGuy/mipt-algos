/* 
*Последовательность*
*/

#include <stdio.h>
#include <stdlib.h>

unsigned long *multiply(unsigned long matrix_a[4], unsigned long matrix_b[4], unsigned long M) {
    unsigned long *result = calloc(4, sizeof(unsigned long));
    result[0] = (matrix_a[0] * matrix_b[0] + matrix_a[1] * matrix_b[2]) % M;
    result[1] = (matrix_a[0] * matrix_b[1] + matrix_a[1] * matrix_b[3]) % M;
    result[2] = (matrix_a[2] * matrix_b[0] + matrix_a[3] * matrix_b[2]) % M;
    result[3] = (matrix_a[2] * matrix_b[1] + matrix_a[3] * matrix_b[3]) % M;
    return result;
}

unsigned long *fib(unsigned long a[4], unsigned long N, unsigned long M) {
    if (N <= 1) return a;
    unsigned long *power = fib(a, N / 2, M);
    unsigned long *result = multiply(power, power, M);
    if (N % 2) return multiply(result, a, M);
    else return result;
}

int main() {
    unsigned long A0, A1, N, M;
    scanf("%lu%lu%lu%lu", &A0, &A1, &N, &M);
    unsigned long *result = multiply(fib((unsigned long[]) {1, 1, 1, 0}, N, M), (unsigned long[]) {A0 + A1, A1, A1, A0}, M);
    printf("%lu", result[3]);

    free(result);
    return 0;
}