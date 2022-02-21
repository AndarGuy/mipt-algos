/*

Симметрическая разность множеств

Симметрическая разность множеств — множество элементов, входящих строго в одно
из множеств-операндов.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comparator(const void *a, const void *b) {
    return *((const int *)a) - *((const int *)b);
}

int main(int argc, char *argv[]) {
    int a[100000 + 1], b[100000 + 1];

    size_t length_a = 0;
    for (length_a = 0; true; length_a++) {
        int number;
        scanf("%d", &number);
        if (number == 0) break;
        a[length_a] = number;
    }

    size_t length_b = 0;
    for (length_b = 0; true; length_b++) {
        int number;
        scanf("%d", &number);
        if (number == 0) break;
        b[length_b] = number;
    }

    qsort(a, length_a, sizeof(int), comparator);

    qsort(b, length_b, sizeof(int), comparator);

    int cursor_a = 0, cursor_b = 0;

    while (cursor_a < length_a || cursor_b < length_b) {
        if (cursor_a == length_a) {
            printf("%d ", b[cursor_b]);
            cursor_b++;
        } else if (cursor_b == length_b) {
            printf("%d ", a[cursor_a]);
            cursor_a++;
        } else {
            if (a[cursor_a] > b[cursor_b]) {
                printf("%d ", b[cursor_b]);
                cursor_b++;
            } else if (a[cursor_a] < b[cursor_b]) {
                printf("%d ", a[cursor_a]);
                cursor_a++;
            } else {
                cursor_a++;
                cursor_b++;
            }
        }
    }

    return 0;
}