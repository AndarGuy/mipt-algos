/*

Палиндромы

Палиндром — строка, читающаяся одинаково в любом направлении. Любую строку можно превратить в палиндром, убрав некоторое количество букв.

Ваша задача — определить, сколькими способами можно превратить заданную строку, состоящуюю не более, чем из 30 заглавных букв латинского алфавита, в палиндром.
Удаление букв, находящихся в различных позициях и приводящее к одному и тому же палиндрому — подсчитывается раздельно.
Удаление пустого множества символов тоже учитывается.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30 + 1

int palindromes(size_t N, char string[MAX]) {
    int cache[MAX][MAX] = {0};
    for (size_t i = 0; i < N; i++) cache[i][i] = 1;
    for (size_t i = 1; i < N; i++) {
        for (size_t j = 0; i + j < N; j++) {
            if (string[j] == string[i + j])
                cache[j][i + j] = cache[j + 1][i + j] + cache[j][i + j - 1] + 1;
            else
                cache[j][i + j] = cache[j + 1][i + j] + cache[j][i + j - 1] - cache[j + 1][i + j - 1];
        }
    }

    // for (size_t i = 0; i < N; i++) {
    //     for (size_t j = 0; j < N; j++) {
    //         printf("%d ", cache[i][j]);
    //     }
    //     printf("\n");
    // }

    return cache[0][N - 1];
}

int main(int argc, char *argv[]) {
    char string[MAX];
    scanf("%s", string);
    size_t N = strlen(string);
    printf("%d\n", palindromes(N, string));

    return 0;
}

//