/*

Дана непустая строка S длиной N символов. Будем считать, что элементы строки
нумеруются от 1 до N.

Для каждой i-й позиции строки S определим подстроку, заканчивающуюся в этой
позиции, которая совпадает с некоторым началом всей строки S и имеет длину,
меньшую, чем i (т.е. не равна i-му префиксу исходной строки). Значением
префикс-функции P(i) будем считать длину этой подстроки.

Требуется для всех i от 1 до N вычислить значение P(i).

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *string = (char *)calloc(1000000 + 1, sizeof(char));
    scanf("%s", string);
    int length = strlen(string);
    int *pi = (int *)calloc(length + 1, sizeof(int));
    int max = 0, index = 0;
    for (size_t k = 0; k < length; k++) {
        char *s = string + k;
        for (int i = 1; i < length - k; i++) {
            int j = pi[i - 1];
            while (j > 0 && s[i] != s[j]) j = pi[j - 1];
            if (s[i] == s[j]) j++;
            if (j > max) {
                max = j;
                index = k + i - max + 1;
            }
            pi[i] = j;
        }
    }

    for (size_t i = 0; i < max; i++) {
        printf("%c", string[index + i]);
    }

    free(string);
    free(pi);

    return 0;
}