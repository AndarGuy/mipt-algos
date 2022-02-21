/*

Пересечение двух множеств

В массивах A размером N и B размером M значения строго возрастают, Требуется
найти пересечение этих массивов.

2 ⩽ N,M ⩽ 100000. Элементы массива не превосходят по абсолютной величине
100000000;

*/

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Создать новое пустое множество, значения элементов которого могут
 * лежать в границах от 0 до capacity-1 включительно. Вернуть указатель на него.
 *
 * @param capacity Максимальная вместимость структуры
 * @return struct set* Указатель на структуру
 */
struct set *set_new(size_t capacity);

/**
 * @brief Добавить во множество s элемент elem. Если элемент существовал,
 * множество не изменится.
 *
 * @param s Указатель на структуру
 * @param elem Элемент, который необходимо добавить в структуру
 * @return int Вернуть 0, если операция корректна, т.е. elem < capacity, иначе
 * вернуть 1.
 */
int set_insert(struct set *s, size_t elem);

/**
 * @brief Поиск элемента в структуре
 *
 * @param s Указатель на структуру
 * @param elem Элемент, который необходимо найти в структуре
 * @return int Вернуть 1, если элемент присутствует в множестве и 0 в противном
 * случае.
 */
int set_find(struct set const *s, size_t elem);

struct set {
    char *array;
    size_t capacity;
};

struct set *set_new(size_t capacity) {
    struct set *s = (struct set *)malloc(sizeof(struct set));
    s->capacity = capacity;
    s->array = (char *)calloc(s->capacity / 8 + 1, sizeof(char));
    return s;
}

int set_insert(struct set *s, size_t elem) {
    if (elem >= s->capacity) return 1;
    s->array[elem / 8] |= (1 << (elem % 8));
    return 0;
}

int set_find(struct set const *s, size_t elem) {
    return s->array[elem / 8] & (1 << (elem % 8));
}

int main(int argc, char *argv[]) {
    int N, M;
    scanf("%d%d", &N, &M);

    struct set *numbers = set_new(100000000);

    for (size_t i = 0; i < N; i++) {
        int number;
        scanf("%d", &number);
        set_insert(numbers, number);
    }

    for (size_t i = 0; i < M; i++) {
        int number;
        scanf("%d", &number);
        if (set_find(numbers, number)) printf("%d ", number);
    }

    return 0;
}