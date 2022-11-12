/*

Коврики

Имеется прямоугольная область, состоящая из 6⩽N⩽1000 на 6⩽M⩽1000 одинаковых квадратных клеток. Часть клеток свободна, часть закрашена. Ковриком называется максимальное множество закрашенных клеток, имеющих общую границу. Коврики размером 1×1 тоже допустимы.

Требуется по заданной раскраске области определить количество находящихся на ней ковриков.

*/

#include <iostream>
#include <vector>

std::vector<std::pair<int, int>> get_turns(int x, int y, int N, int M) {
    std::vector<std::pair<int, int>> result;
    for (auto variant : (std::vector<std::pair<int, int>>){{-1, 0}, {1, 0}, {0, 1}, {0, -1}}) {
        int new_x = x + variant.first, new_y = y + variant.second;
        if (0 <= new_x && new_x < N && 0 <= new_y && new_y < M) result.push_back({new_x, new_y});
    }
    return result;
}

void dfs(int x, int y, int N, int M, std::vector<std::vector<char>> &field) {
    if (field[x][y] == '-') return;
    if (field[x][y] == '.') return;
    field[x][y] = '-';
    for (auto turn : get_turns(x, y, N, M)) dfs(turn.first, turn.second, N, M, field);
}

void carpets(int N, int M, std::vector<std::vector<char>> &field) {
    int carpets = 0;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            if (field[i][j] == '+') {
                dfs(i, j, N, M, field);
                carpets++;
            }
        }
    }

    std::cout << carpets << std::endl;
}

int main() {
    int N, M;
    std::cin >> N >> M;
    std::vector<std::vector<char>> field(N, std::vector<char>(N, 0));

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            std::cin >> field[i][j];
        }
    }

    carpets(N, M, field);

    return 0;
}