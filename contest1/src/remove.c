/*

Удаление комментариев

Елпидифор пишет новую программу, которая точно устанавливает факт заимствования
чужого кода. Для этого он разбивает процесс анализа программы на этапы. Так как
комментарии не влияют на алгоритм программы, обработка начинается с удаления
всех комментариев. Помогите ему!

Однострочным комментарием является комментарий, начинающийся с // и
заканчивающийся переводом строки или концом файла.

Многострочным комментарием называется комментарий, начинающийся с / * и
заканчивающийся * /, даже если он помещается на одной строке текста.

Обратите внимание на то, что в C и C++ ещё имеются строки, внутри которых
комментариев не бывает!

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum State { EXIT, ANY, STRING_1, STRING_2, COMMENT_1, COMMENT_2 };

void worker(char *text, size_t length) {
    int needle = 0;
    enum State state = ANY;

    while (state) {
        if (needle == length) state = EXIT;
        switch (state) {
            case ANY:
                if (text[needle] == '\'') {
                    state = STRING_1;
                }
                if (text[needle] == '\"') {
                    state = STRING_2;
                }
                if (text[needle] == '/' && text[needle + 1] == '/') {
                    state = COMMENT_2;
                    text[needle] = '\n';
                    text[needle + 1] = 1;
                }
                if (text[needle] == '/' && text[needle + 1] == '*') {
                    state = COMMENT_1;
                    text[needle] = ' ';
                    text[needle + 1] = 1;
                }
                break;
            case STRING_1:
                if (text[needle] == '\'') {
                    state = ANY;
                }
                break;
            case STRING_2:
                if (text[needle] == '\"') {
                    state = ANY;
                }
                break;
            case COMMENT_1:
                if (text[needle] == '*' && text[needle + 1] == '/') {
                    state = ANY;
                    text[needle + 1] = 1;
                }
                text[needle] = 1;
                break;
            case COMMENT_2:
                if (text[needle] == '\n') {
                    state = ANY;
                }
                text[needle] = 1;
                break;
            case EXIT:
                break;
        }
        needle++;
    }
}

int main(int argc, char *argv[]) {
    char *text = calloc(0x100000, sizeof(char));
    size_t length = 0;
    for (length = 0; (text[length] = fgetc(stdin)) != -1; length++) {
    }

    worker(text, length);

    for (size_t i = 0; i < length; i++) {
        if (text[i] != '\1') printf("%c", text[i]);
    }

    return 0;
}