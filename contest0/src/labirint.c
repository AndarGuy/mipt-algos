/*

Пути в лабиринте

Нахождение кратчайшего пути в лабиринте — известная задача. Мы хотим решить немного более сложную — определить, сколько имеется способов выйти из лабиринта ровно за определённое количество ходов. Конечно, допускается хождение туда-обратно.

Требуется определить, сколько имеется путей длиной ровно K от входа до выхода. Движение может происходить только по свободным клеткам (вход и выход считаются свободными), одни и те же клетки можно посещать любое количество раз. Двигаться можно только по горизонтали и вертикали (до 4-х ходов из клетки, не находящейся на границе).

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void labirint(int N, int M, int K, long long **cache[2], char **labyrinth) {
    long long **temp = calloc(N + 2, sizeof(long long **));
    int entrance_x, entrance_y, exit_x, exit_y;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            if (labyrinth[i][j] == '@') {
                entrance_x = i;
                entrance_y = j;
            }
            if (labyrinth[i][j] == '#') {
                exit_x = i;
                exit_y = j;
            }
        }
    }

    // printf("Exit - (%d, %d), entrance - (%d, %d)\n", exit_x, exit_y, entrance_x, entrance_y);

    cache[0][exit_x + 1][exit_y + 1] = 1;

    for (size_t i = 0; i < K; i++) {
        for (size_t j = 1; j <= N; j++) {
            for (size_t k = 1; k <= M; k++) {
                if (labyrinth[j - 1][k - 1] == '+') continue;
                cache[1][j][k] = cache[0][j - 1][k] + cache[0][j][k - 1] + cache[0][j][k + 1] + cache[0][j + 1][k];
            }
        }
        memcpy(temp, cache[1], (N + 2) * sizeof(long long **));
        memcpy(cache[1], cache[0], (N + 2) * sizeof(long long **));
        memcpy(cache[0], temp, (N + 2) * sizeof(long long **));
    }

    // for (size_t i = 0; i <= N + 1; i++) {
    //     for (size_t j = 0; j <= M + 1; j++) {
    //         printf("%lld ", cache[0][i][j]);
    //     }
    //     printf("\n");
    // }

    printf("%lld", cache[0][entrance_x + 1][entrance_y + 1]);
}

int main(int argc, char *argv[]) {
    int N, M, K;
    scanf("%d%d%d", &N, &M, &K);

    long long **cache[2] = {calloc(N + 2, sizeof(long long **)), calloc(N + 2, sizeof(long long **))};
    for (size_t i = 0; i <= N + 1; i++) {
        cache[0][i] = calloc(M + 2, sizeof(long long));
        cache[1][i] = calloc(M + 2, sizeof(long long));
    }

    char **labyrinth = calloc(N, sizeof(char *));
    for (size_t i = 0; i < N; i++) {
        labyrinth[i] = calloc(M, sizeof(char));
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            scanf("%c", &labyrinth[i][j]);
            while (labyrinth[i][j] == '\n') scanf("%c", &labyrinth[i][j]);
        }
    }

    labirint(N, M, K, cache, labyrinth);

    return 0;
}

//