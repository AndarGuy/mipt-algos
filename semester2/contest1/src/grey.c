/*

Коды Грея

Даны все строки длины N, состоящие из нулей и единиц. Говорят, что они образуют
код Грея, если они идут в таком порядке, что каждая следующая строка отличается
от предыдущей ровно в одной позиции.

Пример кода Грея размерности 2:

00 01 11 10

Необходимо вывести код Грея размера N (вход задачи), начиная со строки,
состоящей из одних нулей.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_gray(char *gray, int reversed, int length, int N) {
    if (length == 1) {
        gray[0] = reversed;
        gray[N] = !reversed;
        return;
    }

    get_gray(gray + 1, 0, length - 1, N);

    for (size_t i = 0; i < (1 << (length - 1)); i++) {
        gray[i * N] = reversed;
    }

    get_gray(gray + 1 + N * (1 << (length - 1)), 1, length - 1, N);

    for (size_t i = 0; i < (1 << (length - 1)); i++) {
        gray[N * (1 << (length - 1)) + i * N] = !reversed;
    }
}

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);
    char *gray = calloc((1 << N) * N, sizeof(char));

    get_gray(gray, 0, N, N);

    for (size_t i = 0; i < (1 << N); i++) {
        for (size_t j = 0; j < N; j++) {
            printf("%c", *(gray + i * N + j) + '0');
        }
        printf("\n");
    }

    return 0;
}