/*

Игра в восемьнашки

Многим знакома игра пятнашки. Она представляет из себя коробочку 4*4, в которой находятся 15 квадратных фишек, пронуменованных от 1 до 15. Задача состоит в том, чтобы не вытаскивая фишки, передвигать их таким образом, чтобы выстроить их в порядке возрастания.

В данной задаче доска поменьше - 3*3 и фишек там 8.

Требуется найти минимальное количество ходов, требующихся для того, чтобы передвинуть фишки в правильное положение.

*/

#include <array>
#include <cinttypes>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

#define N 3
#define M 3

std::pair<int, int> get_gap(std::array<std::array<char, M>, N>& state) {
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < M; j++)
            if (!state[i][j]) return {i, j};
    return {0, 0};
}

std::vector<std::array<std::array<char, M>, N>> get_turns(std::array<std::array<char, M>, N>& state) {
    std::vector<std::array<std::array<char, M>, N>> result;
    auto gap = get_gap(state);
    int x = gap.first, y = gap.second;
    if (x > 0) {
        std::array<std::array<char, M>, N> turn = state;
        std::swap(turn[x - 1][y], turn[x][y]);
        result.push_back(turn);
    }
    if (y > 0) {
        std::array<std::array<char, M>, N> turn = state;
        std::swap(turn[x][y - 1], turn[x][y]);
        result.push_back(turn);
    }
    if (x + 1 < N) {
        std::array<std::array<char, M>, N> turn = state;
        std::swap(turn[x + 1][y], turn[x][y]);
        result.push_back(turn);
    }
    if (y + 1 < M) {
        std::array<std::array<char, M>, N> turn = state;
        std::swap(turn[x][y + 1], turn[x][y]);
        result.push_back(turn);
    }
    return result;
}

// void display(std::array<std::array<char, M>, N>& state) {
//     std::cout << "---" << std::endl;
//     for (size_t i = 0; i < N; i++) {
//         for (size_t j = 0; j < M; j++) {
//             std::cout << ((int)state[i][j]);
//         }
//         std::cout << std::endl;
//     }
//     std::cout << "---" << std::endl;
// }

int bfs(std::array<std::array<char, M>, N> state, std::array<std::array<char, M>, N>& required, std::map<std::array<std::array<char, M>, N>, int>& visited) {
    std::queue<std::array<std::array<char, M>, N>> queue;
    queue.push(state);

    while (!queue.empty()) {
        auto state = queue.front();
        queue.pop();
        if (required == state) return visited[state];
        for (auto turn : get_turns(state)) {
            if (!visited[turn]) {
                visited[turn] = visited[state] + 1;
                queue.push(turn);
            }
        }
    }

    return -1;
}

void eight_game(std::array<std::array<char, M>, N> state, std::array<std::array<char, M>, N> required) {
    std::map<std::array<std::array<char, M>, N>, int> visited;
    std::cout << bfs(state, required, visited) << std::endl;
}

int main() {
    std::array<std::array<char, M>, N> required = {{{1, 2, 3}, {4, 5, 6}, {7, 8, 0}}};
    std::array<std::array<char, M>, N> state;

    for (size_t i = 0; i < N; i++) {
        std::string temp;
        std::getline(std::cin, temp);
        for (size_t j = 0; j < M; j++) {
            state[i][j] = temp[j];
            if (state[i][j] == ' ')
                state[i][j] = 0;
            else
                state[i][j] -= '0';
        }
    }

    eight_game(state, required);

    return 0;
}