/*

Путешествие коня

На доске размером N×N, 4⩽N⩽26 на поле, заданном координатами, расположен шахматный конь. Его цель — попасть в наименьшее число ходов на другое поле, координаты которого тоже заданы.

Вы должны определить наименьшее число ходов, за которые конь может попасть на нужное ему поле и любую возможную последовательность ходов, которая приведёт к результату.

*/

#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

std::vector<std::pair<int, int>> get_turns(int x, int y, int N) {
    std::vector<std::pair<int, int>> variants = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
    std::vector<std::pair<int, int>> result;
    for (std::pair<int, int> variant : variants) {
        int new_x = x + variant.first, new_y = y + variant.second;
        if (0 <= new_x && new_x < N && 0 <= new_y && new_y < N) result.push_back({new_x, new_y});
    }
    return result;
}

void dfs(int x, int y, int step, std::vector<std::vector<int>> &plate) {
    plate[x][y] = step;
    for (std::pair<int, int> turn : get_turns(x, y, plate.size())) {
        if (plate[turn.first][turn.second] > step + 1) dfs(turn.first, turn.second, step + 1, plate);
    }
}

void knight(int N, int x1, int y1, int x2, int y2) {
    std::vector<std::vector<int>> plate(N, std::vector<int>(N, INT32_MAX));

    dfs(x1, y1, 1, plate);

    std::cout << plate[x2][y2] - 1 << std::endl;

    std::function<void(int, int)> print_trace = [&plate, &print_trace](int x, int y) -> void {
        if (plate[x][y] == 1) return;
        for (std::pair<int, int> turn : get_turns(x, y, plate.size())) {
            if (plate[turn.first][turn.second] + 1 == plate[x][y]) {
                print_trace(turn.first, turn.second);
                break;
            }
        }
        std::cout << (char)('A' + x) << y + 1 << std::endl;
    };

    print_trace(x2, y2);

    std::cout << std::endl;

    // for (size_t i = 0; i < N; i++) {
    //     for (size_t j = 0; j < N; j++) {
    //         std::cout << plate[i][j] << ' ';
    //     }
    //     std::cout << std::endl;
    // }
}

int main() {
    int N;
    std::cin >> N;

    char buffer;
    std::cin >> buffer;
    int x1 = buffer - 'A', y1;
    std::cin >> y1;
    y1--;
    std::cin >> buffer;
    int x2 = buffer - 'A', y2;
    std::cin >> y2;
    y2--;

    knight(N, x1, y1, x2, y2);

    return 0;
}