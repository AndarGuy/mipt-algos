/*

Вдоль прямой дороги расположены деревни. Дорога представляется целочисленной осью, а расположение каждой деревни задается одним целым числом — координатой на этой оси. Никакие две деревни не имеют одинаковых координат. Расстояние между двумя деревнями вычисляется как модуль разности их координат.

В некоторых, не обязательно во всех, деревнях будут построены почтовые отделения. Деревня и расположенное в ней почтовое отделение имеют одинаковые координаты. Почтовые отделения необходимо расположить в деревнях таким образом, чтобы общая сумма расстояний от каждой деревни до ближайшего к ней почтового отделения была минимальной.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEG(x) -(x)
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) NEG(MIN(NEG(x), NEG(y)))

int post(int N, int M, int villages[300]) {
    int **cache = calloc(M, sizeof(int *));
    for (size_t i = 0; i < M; i++) {
        cache[i] = calloc(N, sizeof(int));
    }

    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < i; j++)
            cache[0][i] += abs(villages[i] - villages[j]);

    int **distances = calloc(M, sizeof(int *));
    for (size_t i = 0; i < M; i++) {
        distances[i] = calloc(N, sizeof(int));
    }

    for (size_t i = 1; i < M; i++) {
        for (size_t j = i + 1; j < N; j++) {
            cache[i][j] = 2147483647;
            for (size_t k = 0; k < j; k++) {
                int sum = cache[i - 1][k];
                for (size_t l = k + 1; l < j; l++)
                    sum += MIN(villages[l] - villages[k], villages[j] - villages[l]);
                if (cache[i][j] > sum) {
                    cache[i][j] = sum;
                    distances[i][j] = k;
                }
            }
        }
    }

    int answer = 2147483647, index = 0;
    for (size_t i = M - 1; i < N; i++) {
        int sum = cache[M - 1][i];
        for (size_t j = i + 1; j < N; j++)
            sum += villages[j] - villages[i];
        if (answer > sum) {
            answer = sum;
            index = i;
        }
    }

    int *result = calloc(M, sizeof(int));
    for (size_t i = 0; i < M; i++) {
        result[i] = villages[index];
        index = distances[M - i - 1][index];
    }

    printf("%d\n", answer);

    for (size_t i = 0; i < M; i++) {
        printf("%d ", result[M - i - 1]);
    }

    free(result);
    for (size_t i = 0; i < M; i++) free(distances[i]);
    free(distances);
    for (size_t i = 0; i < M; i++) free(cache[i]);
    free(cache);

    return 0;
}

int main(int argc, char *argv[]) {
    int N, M;
    scanf("%d%d", &N, &M);
    int villages[300];
    for (size_t i = 0; i < N; i++) {
        scanf("%d", &villages[i]);
    }

    post(N, M, villages);

    return 0;
}

//