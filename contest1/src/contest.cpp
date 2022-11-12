/*

Соревнование

Елпидифор решил поучаствовать в соревновании по укладке узоров из плиток на время.
Заданный узор можно выкладывать любым количеством непересекающихся плиток размера 1 × 2 (их можно и поворачивать), и плиток 1 × 1.
Елпидифор способен уложить одну большую плитку за A секунд, а одну маленькую — за B секунд.

Определите наилучшее время, за которое Елпидифор может уложить заданный узор. Узол помещается в прямоугольнике N × M.
Символ точки '.' представляет пустую клетку, звёздочки * — заполненную.

*/

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

std::vector<std::pair<int, int>> get_turns(int x, int y, int N, int M) {
    std::vector<std::pair<int, int>> result;
    for (auto variant : (std::vector<std::pair<int, int>>){{-1, 0}, {1, 0}, {0, 1}, {0, -1}}) {
        int new_x = x + variant.first, new_y = y + variant.second;
        if (0 <= new_x && new_x < N && 0 <= new_y && new_y < M) result.push_back({new_x, new_y});
    }
    return result;
}

bool dfs(int vertex, std::vector<std::vector<int>> &connections, std::vector<int> &visited, std::vector<int> &matching, std::vector<int> &pairs) {
    if (visited[vertex]) return false;
    visited[vertex] = true;
    for (auto y : connections[vertex]) {
        if (matching[y] < 0 || dfs(matching[y], connections, visited, matching, pairs)) {
            matching[y] = vertex;
            pairs[vertex] = y;
            return true;
        }
    }
    return false;
}

void contest(int N, int M, int A, int B, int needed, std::vector<std::vector<int>> connections) {
    std::vector<int> visited(N * M, false);
    std::vector<int> matching(N * M, -1), pairs(N * M, -1);

    if (B + B < A + 1) {
        std::cout << needed * B << std::endl;
        return;
    }

    int next = true;
    while (next) {
        next = false;
        std::fill(visited.begin(), visited.end(), false);
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < M; j++) {
                if ((i + j) % 2) continue;
                int vertex = i * M + j;
                if ((pairs[vertex] < 0) && dfs(vertex, connections, visited, matching, pairs)) next = true;
            }
        }
    }

    int joined = 0;
    for (size_t vertex = 0; vertex < N * M; vertex++)
        if (matching[vertex] != -1) joined++;

    // std::cout << joined << std::endl;

    std::cout << joined * A + (needed - 2 * joined) * B << std::endl;
}

int main() {
    int N, M, A, B;
    std::cin >> N >> M >> A >> B;
    int needed = 0;
    std::vector<std::vector<char>> field(N, std::vector<char>(M));
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            std::cin >> field[i][j];
            if (field[i][j] == '*') needed++;
        }
    }

    std::vector<std::vector<int>> connections(N * M);
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            if (field[i][j] == '.') continue;
            if ((i + j) % 2) continue;
            int vertex = i * M + j;
            for (auto turn : get_turns(i, j, N, M))
                if (field[turn.first][turn.second] == '*') connections[vertex].push_back(turn.first * M + turn.second);
        }
    }

    contest(N, M, A, B, needed, connections);
}