/*

Последовательность строк

Над прямой X расположено несколько столбиков из звёздочек. Все столбики
находятся рядом, количество звёздочек в них неотрицательно и все звёздочки, если
они есть, начинаются сразу над прямой.

Всё это описывается следующим образом. В первой строке — число столбиков 1 ⩽ N ⩽
1000. Остальные N чисел располагаются в следующей строке и обозначают высоты
столбиков. Высоты столбиков не превосходят 1000.

Например, входные данные из первого примера выглядят так:

*/

#include <stdio.h>

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);
    int columns[1000] = {0};

    for (size_t i = 0; i < N; i++) {
        scanf("%d", columns + i);
    }

    int max_square = 0;
    for (size_t i = 0; i < 1000; i++) {
        int max_seq = 0, current_seq = 0;
        for (size_t j = 0; j < N; j++) {
            if (columns[j] > i) {
                current_seq++;
                max_seq = max_seq > current_seq ? max_seq : current_seq;
            } else {
                current_seq = 0;
            }
        }
        int current_square = max_seq * (i + 1);
        max_square = current_square > max_square ? current_square : max_square;
    }

    printf("%d", max_square);

    return 0;
}