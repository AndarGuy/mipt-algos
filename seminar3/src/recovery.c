/*

Потерянная перестановка

Иеремия изучает перестановки множеств. Для каждой перестановки чисел от 1 до N
он составляет таблицу инверсий размером N, где элемент под индексом i содержит
количество инверсий для элемента Ai.

Например, перестановке (3,2,1) соответствует таблицв инверсий (0,1,2), а
перестановке (2,1,3) — таблица инверсий (0,1,0).

Он обнаружил, что не все таблицы инверсий могут существовать и задался вопросом:
можно ли по таблице инверсий восстановить перестановку?

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comparator(const void *a, const void *b) {
    return *((int *)b) - *((int *)a);
}

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);
    int *permutations = calloc(N, sizeof(int));
    int *elements = calloc(N, sizeof(int));

    for (size_t i = 0; i < N; i++) {
        scanf("%d", permutations + i);
    }

    for (size_t i = 0; i < N; i++) {
        if (permutations[i] > i) {
            printf("IMPOSSIBLE\n");
            return 0;
        } else {
            elements[i] = i - permutations[i] + 1;
            for (size_t j = 0; j < i; j++) {
                if (elements[j] >= elements[i]) elements[j]++;
            }
        }
    }

    for (size_t i = 0; i < N; i++) {
        printf("%d ", elements[i]);
    }

    return 0;
}