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

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET 26

int get_bit(int combination, unsigned int bit) {
    return combination & (1 << bit);
}

int get_target(char *letters, unsigned int *mask, int *target) {
    // ищем первое необработанное повторение
    for (size_t i = 0; i < ALPHABET; i++) {
        if (letters[i] > 1) {
            *target = i;
            // printf("Необработанное повторение: %c\n", 'a' + i);
            *mask |= (1 << i);
            return 1;
        }
    }
    return 0;
}

void get_subsets(char *letters, unsigned int mask) {
    // перебираем варианты по маске
    for (int combination = mask; combination < 0x4000000; combination++) {
        combination |= mask;
        // printf("%d\n", combination);
        int printed = 0;
        for (char k = 0; k < ALPHABET + 1; k++) {
            if (get_bit(combination, k)) {
                for (size_t l = 0; l < letters[(int)k]; l++) {
                    printf("%c", 'a' + k);
                    printed = 1;
                }
            }
        }
        if (printed) printf("\n");
    }
}

int get_dub(char *letters, unsigned int *mask, int *target) {
    // ищем любое повторение, кроме выбранного выше
    for (size_t i = 0; i < 26; i++) {
        // если нашли, то уменьшаем его на 1 и добавляем в маску,
        // как обязательное
        if (*target != i && letters[i] > 1) {
            // printf("Уменьшаем %c на 1\n", i + 'a');
            letters[i]--;
            *mask |= (1 << i);
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    char *letters = calloc(32, sizeof(char));

    char letter;
    while ('\n' != (letter = getc(stdin))) {
        letters[letter - 'a']++;
    }

    char *letters_copy = calloc(32, sizeof(char));
    memcpy(letters_copy, letters, 32);

    unsigned int default_mask = 0, mask;
    for (size_t i = 0; i < 26; i++)
        if (!letters[i]) default_mask += (1 << i);
    mask = default_mask;

    int done = 0;
    int found;

    // обработка повторения
    int target = -1;
    int dub = -1;

    printf("\n");

    while (!done) {
        done = 1;

        // перебираем варианты по маске
        get_subsets(letters_copy, mask);

        while (true) {
            if (target < 0) {
                found = get_target(letters, &mask, &target);
                if (!found) {
                    done = true;
                    break;
                }
            }

            done = false;

            if (dub != -1 && letters_copy[dub] != 0) {
                letters_copy[dub]--;
                break;
            }

            found = get_dub(letters_copy, &mask, &target);

            if (found) break;

            if (letters[target] == 1) {
                letters[target] = 0;
                default_mask |= (1 << target);
                mask = default_mask;
                target = -1;
                memcpy(letters_copy, letters, 32);
                continue;
            } else {
                // printf(
                //     "Повторений главного элемента не найдено, уменьшаем на 1
                //     и " "восстанавливаем массив.\n");
                letters[target]--;
                memcpy(letters_copy, letters, 32);
                mask = default_mask;
                mask |= (1 << target);
                break;
            }
        }
    }

    return 0;
}