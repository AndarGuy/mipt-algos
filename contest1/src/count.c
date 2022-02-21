/*

Считаем комментарии

Комментарием в языке Object Pascal является любой текст, находящийся между
последовательностью символов, начинающих комментарий определённого вида и
последовательностью символов, заканчивающей комментарий этого вида.

Виды комментариев могут быть следующие:

Начинающиеся с набора символов (* и заканчивающиеся набором символов *).
Начинающиеся с символа { и заканчивающиеся символом }.
Начинающиеся с набора символов // и заканчивающиеся символом новой строки
.
Еще в языке Object Pascal имеются литеральные строки, начинающиеся символом
одиночной кавычки ' (ASCII код 39) и заканчивающиеся этим же символом. В
корректной программе строки не могут содержать символа перехода на новую строку.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { EXIT, ANY, STRING, COMMENT_1, COMMENT_2, COMMENT_3 };

void worker(char *text, size_t length, int *counters) {
    int needle = 0;
    enum State state = ANY;

    while (state) {
        if (needle == length) state = EXIT;
        switch (state) {
            case ANY:
                if (text[needle] == '\'') {
                    state = STRING;
                }
                if (text[needle] == '{') {
                    state = COMMENT_2;
                }
                if (text[needle] == '/' && text[needle + 1] == '/') {
                    state = COMMENT_3;
                }
                if (text[needle] == '(' && text[needle + 1] == '*') {
                    state = COMMENT_1;
                }
                break;
            case STRING:
                if (text[needle] == '\'') {
                    state = ANY;
                    counters[3]++;
                }
                break;
            case COMMENT_1:
                if (text[needle] == '*' && text[needle + 1] == ')') {
                    state = ANY;
                    counters[0]++;
                }
                break;
            case COMMENT_2:
                if (text[needle] == '}') {
                    state = ANY;
                    counters[1]++;
                }
                break;
            case COMMENT_3:
                if (text[needle] == '\n') {
                    state = ANY;
                    counters[2]++;
                }
                break;
            case EXIT:
                break;
        }
        needle++;
    }
}

int main(int argc, char *argv[]) {
    char *text = calloc(1 << 20, sizeof(char));
    size_t length = 0;
    for (length = 0; (text[length] = fgetc(stdin)) > 0; length++) {
    }

    int counters[4] = {0};

    worker(text, length, counters);

    for (size_t i = 0; i < 4; i++) {
        printf("%d ", counters[i]);
    }

    return 0;
}