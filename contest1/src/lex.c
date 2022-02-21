/*

Лексикографическая сортировка

Натуральные числа от 1 до N упорядочены лексикографически. Например, для N=25
результат этого упорядочения будет таким: 1, 10, 11, 12, 13, 14, 15, 16, 17, 18,
19, 2, 20, 21, 22, 23, 24, 25, 3, 4, 5, 6, 7, 8, 9.

Требуется написать программу, которая определит, на каком месте в данной
последовательности оказалось число K.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int N, K;
    scanf("%d%d", &N, &K);

    int number = 1;
    int counter = 0;

    while (true) {
        // printf("%d\n", number);
        if (number == K) {
            printf("%d", counter + 1);
            break;
        }
        if (number * 10 <= N)
            number *= 10;
        else if (number % 10 == 9 || number == N) {
            number /= 10;
            while (number % 10 == 9) number /= 10;
            number++;
        } else {
            number++;
        }
        counter++;
    }

    return 0;
}