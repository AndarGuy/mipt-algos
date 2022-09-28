/*

Расстояние редактирования

На вход подаются два слова, каждое в отдельной строке длиной не более 1000. Необходимо найти расстояние редактирования между ними, которое определяется следующим образом: два слова записываются друг под другом и выравнивают их путём добавления разделителя. Каждое использование разделителя и каждое несовпадение двух букв в одной позиции приводит к увеличению штрафа на единицу. Расстояние редактирования — минимально возможна величина штрафа.

В приводимом примере выравниваются слова SNOWY и SUNNY. Штраф равен трём.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOUND 1000 + 1

#define NEG(x) -(x)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) NEG(MIN(NEG(x), NEG(y)))

void levenstein(char *a, char *b, int **cache) {
    size_t a_length = strlen(a), b_length = strlen(b);
    for (size_t i = 0; i <= a_length; i++) {
        cache[0][i] = i;
    }
    for (size_t i = 0; i <= b_length; i++) {
        cache[i][0] = i;
    }

    for (size_t i = 1; i <= b_length; i++) {
        for (size_t j = 1; j <= a_length; j++) {
            cache[i][j] = MIN(cache[i - 1][j] + 1, MIN(cache[i][j - 1] + 1, b[i - 1] == a[j - 1] ? cache[i - 1][j - 1] : cache[i - 1][j - 1] + 1));
        }
    }

    // for (size_t i = 0; i <= b_length; i++) {
    //     for (size_t j = 0; j <= a_length; j++) {
    //         printf("%d ", cache[i][j]);
    //     }
    //     printf("\n");
    // }

    printf("%d\n", cache[b_length][a_length]);
}

int main(int argc, char *argv[]) {
    char a[BOUND], b[BOUND];
    scanf("%s%s", a, b);
    int **cache = calloc(BOUND, sizeof(int *));
    for (size_t i = 0; i < BOUND; i++) {
        cache[i] = calloc(BOUND, sizeof(int));
    }

    levenstein(a, b, cache);

    return 0;
}

//