/*

Актимер написал на длинной полоске бумаги большое число и решил похвастаться
своему сокоманднику Косте этим достижением. Но только он вышел из комнаты, чтобы
позвать сокомандника, как его киллер Зухра вбежала в комнату и разрезала полоску
бумаги на несколько частей. В результате на каждой части оказалось одна или
несколько идущих подряд цифр.

Теперь Актимер не может вспомнить, какое именно число он написал. Только помнит,
что оно было очень большое. Чтобы утешить сокомандника, Костя решил выяснить,
какое максимальное число могло быть написано на полоске бумаги перед
разрезанием. Помогите ему!

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 150

int comparator(const void *a, const void *b) {
    int lenA = strlen(a), lenB = strlen(b);
    char *newA = malloc(lenA + lenB + 1), *newB = malloc(lenA + lenB + 1);
    strcpy(newA, a);
    strcpy(newB, b);
    strcat(newA, b);
    strcat(newB, a);
    int result = -strcmp(newA, newB);
    // printf("comparing... %s %s %s %s %d\n", a, b, newA, newB, result);
    free(newA);
    free(newB);
    return result;
}

int main(int argc, char *argv[]) {
    char *shards = calloc(100, sizeof(char) * MAX_LINE_LENGTH);
    size_t line = 0;
    for (line = 0;
         fgets(shards + line * MAX_LINE_LENGTH, MAX_LINE_LENGTH, stdin) > 0;
         line++) {
        for (size_t j = 0; j < MAX_LINE_LENGTH; j++) {
            if (*(shards + line * MAX_LINE_LENGTH + j) == '\n')
                *(shards + line * MAX_LINE_LENGTH + j) = 0;
        }
    }

    qsort(shards, line, MAX_LINE_LENGTH, comparator);

    for (size_t i = 0; i < 100; i++) {
        if (*(shards + i * MAX_LINE_LENGTH))
            printf("%s", shards + i * MAX_LINE_LENGTH);
    }

    free(shards);

    return 0;
}