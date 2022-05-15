#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int N, k;
int *matrix, *priority;

int comparator(const void *a, const void *b) {
    for (int i = 0; i < k; i++) {
        int p = priority[i];
        // printf("%d %d %d\n", p, ((const int *) a)[p], ((const int *) b)[p]);
        if (((const int *) a)[p] < ((const int *) b)[p]) return 1;
        else if ((((const int *) a)[p] > ((const int *) b)[p])) return -1;
    }
    return 0;
}

int main() {
    scanf("%d%d", &N, &k);
    
    matrix = (int*) calloc((k + 1) * N, sizeof(int));
    priority = (int*) calloc(k, sizeof(int));
    char *names = (char*) calloc(128 * N, sizeof(char));

    for (int i = 0; i < k; i++) {
        scanf("%d", priority + i);
    }
    
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < k + 1; j++) {
            if (j == 0) {
                scanf("%s", names + i * 128);
                matrix[i * (k + 1) + j] = i;
            }
            else scanf("%d", matrix + i * (k + 1) + j);
        }
    }

    qsort(matrix, N, sizeof(int) * (k + 1), comparator);

    for (int i = 0; i < N; i++) {
        printf("%s\n", names + matrix[i * (k + 1)] * 128);
    }

    free(matrix);
    free(priority);
    free(names);

    return 0;
}