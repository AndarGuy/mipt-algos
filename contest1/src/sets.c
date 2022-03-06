/*

Подмножества

Множество задано строкой, то есть каждая буква есть элемент множества.

Но это множество — не совсем простое. Элементы в нём могут повторяться.

Два подмножества считаются одинаковыми, если все элементы одного множества
совпадают с элементами другого. Например, множества, представленные строками abc
и cba совпадают. Совпадают также множества abra и raba.

Ваша задача по заданной строке, представляющей исходное множество, вывести все
различные его подмножества, каждое на отдельной строке вывода. Выводить можно в
произвольном порядке. Выход не должен содержать совпадающие подмножества.

Пустое множество тоже является подмножеством исходного.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Вывод всевозможных перестановок массива символов
 *
 * @param letters Массив символов
 * @param valve Хранилище текущей перестановки
 * @param depth Номер фиксируемого элемента
 * @param length Длина массива символов
 * @param duplicates Словарь повторяющихся символов
 * @param elements Размер текущей перестановки
 */
void subsets(char *letters, char *valve, int depth, int length,
             char *duplicates, int elements) {
    if (depth + 1 == length) {
        for (size_t i = 0; i < elements; i++) {
            printf("%c", valve[i]);
        }
        printf("\n");
        return;
    }

    char dublicated = duplicates[(int)letters[depth]];

    subsets(letters, valve, depth + dublicated, length, duplicates, elements);

    for (size_t i = 1; i < dublicated + 1; i++) {
        valve[elements + i - 1] = letters[depth + i - 1];
        subsets(letters, valve, depth + dublicated, length, duplicates,
                elements + i);
    }
}

int main(int argc, char *argv[]) {
    /**
     * @brief letters - массив символов, valve - хранилище текущей перестановки
     *
     */
    char *letters = calloc(1024, sizeof(char)),
         *valve = calloc(1024, sizeof(char));

    /**
     * @brief Словарь повторяющихся символов
     *
     */
    char *duplicates = calloc(127, sizeof(char));

    fgets(letters, 1024, stdin);

    int length = strlen(letters);
    for (size_t i = 0; i < length; i++) {
        duplicates[(int)letters[i]]++;
    }

    int counter = 0;
    for (size_t i = '0'; i < 'z' + 1; i++) {
        for (size_t j = 0; j < duplicates[i]; j++) {
            letters[counter] = i;
            counter++;
        }
    }

    subsets(letters, valve, 0, length, duplicates, 0);

    free(letters);
    free(valve);
    free(duplicates);

    return 0;
}

/**
 * @complexity O(2^N + N)
 *
 * @memory 1024 + 1024 + 255 = 2,25 кб
 */