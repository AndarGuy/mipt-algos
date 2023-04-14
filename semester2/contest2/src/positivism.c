/*

Позитивизм

У Иеремии родители — философы. Он часто слышит их разговоры, но мало что в них
понимает. Сам Иеремия учится в информатическом классе физматшколы, и философия
его пока не интересует.

Недавно он услышал новое для себя слово: позитивизм. Он постеснялся спросить
своих родителей, что это слово обозначает, и узнал в Википедии, что-то про
эмпирические и философские исследования. Так как слово ему понравилось, он
загорелся идеей провести какие-то исследования, а так, как недавно в школе они
проходили матрицы, вопрос, что же будет объектом исследования, оказался для него
очевидным.

Он называет позитивной матрицей такую двумерную матрицу, у сумма элементов
каждой из строк неотрицательна и сумма элементов каждого из столбцов тоже
неотрицательна.

Он поставил перед собой задачу установить, каждую ли из матриц можно сделать
позитивной, имея ровно две операции: смену всех знаков на противоположные либо
для всех элементов строки, либо для всех элементов столбца. Операции первого
вида он записывал, как l y, где y — номер инвертируемой строки, а второго — как
c x,
где x — номер инвертируемого столбца. И столбцы и строки нумеруются с нуля.

Он проделал серию экспериментов и убедился, что если решение существует, то оно
может быть не единственным.

Ваша задача состоит в том, чтобы помочь Иеремии, написав программу, которая либо
сообщит, что не существует такой последовательности операций, чтобы матрица
стала позитивной, либо выдаст любую последовательность операций, приводящую в
конечном итоге к позитивной матрице.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sum_row(int **matrix, int row, int M) {
    int sum = 0;
    for (size_t i = 0; i < M; i++) sum += matrix[row][i];
    return sum;
}

int sum_column(int **matrix, int column, int N) {
    int sum = 0;
    for (size_t i = 0; i < N; i++) sum += matrix[i][column];
    return sum;
}

void reverse_row(int **matrix, int row, int M) {
    for (size_t i = 0; i < M; i++) matrix[row][i] = -matrix[row][i];
}

void reverse_column(int **matrix, int column, int N) {
    for (size_t i = 0; i < N; i++) matrix[i][column] = -matrix[i][column];
}

int main(int argc, char *argv[]) {
    int N, M;
    scanf("%d%d", &N, &M);

    int **matrix = malloc(sizeof(int *) * N);
    for (size_t i = 0; i < N; i++) {
        matrix[i] = malloc(sizeof(int) * M);
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            scanf("%d", *(matrix + i) + j);
        }
    }

    for (size_t i = 0; i < M; i++) {
        if (sum_column(matrix, i, N) < 0) {
            reverse_column(matrix, i, N);
            printf("c %zu\n", i);
        }
    }

    for (size_t i = 0; i < N; i++) {
        if (sum_row(matrix, i, M) < 0) {
            reverse_row(matrix, i, M);
            printf("l %zu\n", i);
        }
    }

    for (size_t i = 0; i < M; i++) {
        if (sum_column(matrix, i, N) < 0) {
            reverse_column(matrix, i, N);
            printf("c %zu\n", i);
        }
    }

    for (size_t i = 0; i < N; i++) {
        if (sum_row(matrix, i, M) < 0) {
            reverse_row(matrix, i, M);
            printf("l %zu\n", i);
        }
    }

    for (size_t i = 0; i < N; i++) {
        if (sum_row(matrix, i, M) < 0) {
            printf("Impossible");
            return 0;
        }
    }

    for (size_t i = 0; i < M; i++) {
        if (sum_column(matrix, i, N) < 0) {
            reverse_column(matrix, i, N);
            printf("Impossible");
            return 0;
        }
    }

    for (size_t i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);

    return 0;
}