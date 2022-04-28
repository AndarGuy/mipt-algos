/*

Подсчёт частот

На вход подаётся строка, состоящая из букв A-Z. Необходимо вывести все буквы,
которые встречаются в строке в порядке убывания частот вместе с частотой. В
случае равенства частот первой должна идти буква, которая встречается в алфавите
раньше.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Entry {
    char letter;
    unsigned count;
};

int comparator(const void *_a, const void *_b) {
    const struct Entry *a = _a, *b = _b;
    if (a->count != b->count)
        return b->count - a->count;
    else
        return a->letter - b->letter;
}

int main(int argc, char *argv[]) {
    char *line = malloc(60000000);
    fgets(line, 60000000, stdin);

    struct Entry letters[30];
    for (char i = 'A'; i <= 'Z'; i++) {
        letters[i - 'A'] = (struct Entry){i, 0};
    }

    for (char *c = line; *c > 0; c++) {
        letters[*c - 'A'].count++;
    }

    qsort(letters, 'Z' - 'A' + 1, sizeof(struct Entry), comparator);

    for (char i = 'A'; i <= 'Z'; i++) {
        if (letters[i - 'A'].count > 0) {
            printf("%c %d\n", letters[i - 'A'].letter, letters[i - 'A'].count);
        }
    }

    free(line);

    return 0;
}