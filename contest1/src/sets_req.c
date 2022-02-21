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
#include <time.h>

void subsets(char *letters, char *mask, int depth, int length,
             unsigned char *dublicates, int elements) {
    if (depth + 1 == length) {
        for (size_t i = 0; i < elements; i++) {
            printf("%c", mask[i]);
        }
        printf("\n");
        return;
    }

    unsigned char dublicated = dublicates[(unsigned char)letters[depth]];

    subsets(letters, mask, depth + dublicated, length, dublicates, elements);

    for (size_t i = 1; i < dublicated + 1; i++) {
        mask[elements + i - 1] = letters[depth + i - 1];
        subsets(letters, mask, depth + dublicated, length, dublicates,
                elements + i);
    }
}

int main(int argc, char *argv[]) {
    char *letters = calloc(1024, sizeof(char)),
         *mask = calloc(1024, sizeof(char));
    unsigned char *dublicates = calloc(255, sizeof(char));

    fgets(letters, 1024, stdin);

    // clock_t tStart = clock();

    int length = strlen(letters);
    for (size_t i = 0; i < length; i++) {
        dublicates[(unsigned char)letters[i]]++;
    }

    // printf("Dublicated counted and sorted for: %.5fs\n",
    //        (double)(clock() - tStart) / CLOCKS_PER_SEC);

    subsets(letters, mask, 0, length, dublicates, 0);

    // printf("Time taken: %.5fs\n", (double)(clock() - tStart) /
    // CLOCKS_PER_SEC);

    return 0;
}