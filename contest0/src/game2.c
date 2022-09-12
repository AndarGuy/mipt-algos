/*

Игра с фишкой

Два игрока играют фишкой на клетчатой доске в следующую игру:

В начальном положении фишка стоит в левом нижнем углу доски, на поле с координатами (1,1).

Игроки ходят по очереди и ход заключается в передвижении фишки на любое количество клеток вверх, вправо или по диагонали вверх/вправо.

Перед началом игры судья выбирает из коллекции досок доску произвольного размера (в коллекции нет досок, состоящих из одного квадратика). В правый верхний угол помещается приз.
Тот игрок, который попал своим ходом на клетку, где находится приз, объявляется победителем.

Определите, какой игрок одержит победу при наилучшей игре обеих сторон для заданного размера доски.

*/

#include <stdio.h>

int game2(int turn, int x, int y, int X, int Y, int cache[300 + 1][300 + 1]) {
    int next = turn == 1 ? 2 : 1;
    int result = 0;
    for (size_t i = x + 1; i < X; i++) {
        if (cache[i][y] == 0)
            result = game2(next, i, y, X, Y, cache);
        else
            result = cache[i][y] == 2 ? turn : next;

        if (result == turn) {
            cache[x][y] = 1;
            return turn;
        }
    }

    for (size_t i = y + 1; i < Y; i++) {
        if (cache[x][i] == 0)
            result = game2(next, x, i, X, Y, cache);
        else
            result = cache[x][i] == 2 ? turn : next;

        if (result == turn) {
            cache[x][y] = 1;
            return turn;
        }
    }

    for (size_t i = 1; x + i < X && y + i < Y; i++) {
        if (cache[x + i][y + i] == 0)
            result = game2(next, x + i, y + i, X, Y, cache);
        else
            result = cache[x + i][y + i] == 2 ? turn : next;

        if (result == turn) {
            cache[x][y] = 1;
            return turn;
        }
    }

    cache[x][y] = 2;
    return next;
}

int main() {
    int X, Y;
    scanf("%d%d", &X, &Y);
    int cache[300 + 1][300 + 1];
    cache[X - 1][Y - 1] = 2;

    printf("%s\n", game2(1, 0, 0, X, Y, cache) == 1 ? "F" : "S");

    // for (size_t i = 0; i < X; i++) {
    //     for (size_t j = 0; j < Y; j++) {
    //         printf("%d ", cache[i][j]);
    //     }
    //     printf("\n");
    // }

    return 0;
}

/*

F[X] = !F[X - 1]

*/