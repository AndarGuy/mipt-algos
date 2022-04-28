/*

Очереди

Даны N нестрого возрастающих массивов Ai и M нестрого убывающих массивов Bj. Все
массивы имеют одну и ту же длину L. Далее даны Q запросов вида (i,j), ответ на
запрос — такое k, что max(Aik,Bjk) минимален. Если таких k несколько, можно
вернуть любое.

*/

#define MAX(x, y) x > y ? x : y

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void min_distance(int *matrix, int N, int L, int i, int j, int *min, int *k,
                  int left, int right) {
    int mid = (left + right) / 2;
    int current = MAX(matrix[i * L + mid], matrix[N * L + j * L + mid]);
    if (current < *min) {
        *min = current;
        *k = mid;
    }

    if (right == left) return;

    if (mid < L - 1) {
        int next =
            MAX(matrix[i * L + mid + 1], matrix[N * L + j * L + mid + 1]);
        if (next <= current) {
            min_distance(matrix, N, L, i, j, min, k, mid + 1, right);
        }

        if (next < current) return;
    }
    if (mid > 0) {
        int previous =
            MAX(matrix[i * L + mid - 1], matrix[N * L + j * L + mid - 1]);
        if (previous <= current) {
            min_distance(matrix, N, L, i, j, min, k, left, mid);
        }
    }
}

int main(int argc, char *argv[]) {
    int N, M, L;
    scanf("%d%d%d", &N, &M, &L);

    int *matrix = malloc((N + M) * L * sizeof(int));

    for (size_t i = 0; i < (N + M) * L; i++) {
        scanf("%d", matrix + i);
    }

    int Q;
    int i, j;
    scanf("%d", &Q);

    for (size_t counter = 0; counter < Q; counter++) {
        scanf("%d%d", &i, &j);
        i--;
        j--;
        int min = MAX(matrix[i * L], matrix[N * L + j * L]), k = 0;
        min_distance(matrix, N, L, i, j, &min, &k, 0, L - 1);
        printf("%d\n", k + 1);
    }

    return 0;
}