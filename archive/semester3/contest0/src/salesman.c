/*

Путешествие продавца

Как обычно, кому-то надо продать что-то во многих городах и для этого отправлен торговый представитель. Имеются города, представленные как M множеств (столбцов) по N городов (строк) в каждом.

Продавец должен посетить ровно по одному городу из каждого множества, затратив на это как можно меньшую сумму денег. Он может выбрать начало своего путешествия и это должен быть город их первого множества. Затем он должен посетить город из второго множества и так до последнего. Число, которое находится в i-й строке и j-м столбце означает стоимость перемещения из предыдущего места в этот город. Имеется ограничение на перемещения: он может перемещаться из города в i-й строке только в города следующего столбца, находящиеся в одной из строк i-1, i, i+1, если такие строки существуют.

Иногда, чтобы заставить посетить продавца какой-то город, ему доплачивают, то есть, стоимость перемещения может быть отрицательной.

Требуется определить наименьшую стоимость маршрута и сам маршрут.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEG(x) -(x)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) NEG(MIN(NEG(x), NEG(y)))

void salesman(int N, int M, int **cache) {
    for (size_t i = 1; i < M; i++) {
        for (size_t j = 0; j < N; j++) {
            if (j == 0)
                cache[j][M - i - 1] += MIN(cache[j + 1][M - i], cache[j][M - i]);
            else if (j + 1 == N)
                cache[j][M - i - 1] += MIN(cache[j - 1][M - i], cache[j][M - i]);
            else
                cache[j][M - i - 1] += MIN(MIN(cache[j - 1][M - i], cache[j + 1][M - i]), cache[j][M - i]);
        }
    }

    // for (size_t i = 0; i < N; i++) {
    //     for (size_t j = 0; j < M; j++) {
    //         printf("%d ", cache[i][j]);
    //     }
    //     printf("\n");
    // }

    int index = 0;
    for (size_t i = 0; i < N; i++) {
        if (cache[i][0] < cache[index][0]) {
            index = i;
        }
    }

    int min = cache[index][0];

    printf("%d ", index + 1);

    for (size_t i = 1; i < M; i++) {
        int min = 0;
        if (index == 0) {
            index = cache[index][i] <= cache[index + 1][i] ? index : index + 1;
        } else if (index + 1 == N) {
            index = cache[index][i] < cache[index - 1][i] ? index : index - 1;
        } else {
            min = MIN(cache[index][i], MIN(cache[index + 1][i], cache[index - 1][i]));
            if (min == cache[index - 1][i])
                index = index - 1;
            else if (min != cache[index][i])
                index = index + 1;
        }

        printf("%d ", index + 1);
    }

    printf("\n");

    printf("%d", min);
}

int main(int argc, char *argv[]) {
    int N, M;
    scanf("%d%d", &N, &M);
    int **cache = calloc(150 + 1, sizeof(int *));
    for (size_t i = 0; i < N; i++) {
        cache[i] = calloc(1000 + 1, sizeof(int));
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            scanf("%d", &cache[i][j]);
        }
    }

    salesman(N, M, cache);

    return 0;
}

// f(x, y) = min(f(x - 1, y - 1), f(x - 1, y), f(x - 1, y + 1))