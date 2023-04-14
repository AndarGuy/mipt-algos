/*

Скутер

У Еремея есть электроскутер и он хочет доехать от дома до института, затратив как можно меньше энергии. Весь город расположен на холмистой местности и разделён на квадраты. Для каждого перекрёстка известна его высота в метрах над уровнем моря. Если ехать от перекрёстка с большей высотой до смежного с ним перекрёстка с меньшей высотой, то электроэнергию можно не тратить, а если наоборот, то расход энергии равен разнице высот между перекрёстками.

Помогите ему спланировать маршрут, чтобы он затратил наименьшее количество энергии от дома до института. Дом находится в левом верхнем углу, институт --- в правом нижнем.

*/

#include <array>
#include <cinttypes>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

size_t N, M;

std::vector<std::pair<int, int>> get_turns(int x, int y) {
    std::vector<std::pair<int, int>> result;
    for (auto variant : (std::vector<std::pair<int, int>>){{-1, 0}, {1, 0}, {0, 1}, {0, -1}}) {
        int new_x = x + variant.first, new_y = y + variant.second;
        if (0 <= new_x && new_x < N && 0 <= new_y && new_y < M) result.push_back({new_x, new_y});
    }
    return result;
}

int dijkstra(std::vector<std::vector<int>> &map) {
    std::vector<std::vector<int>> visited(N, std::vector<int>(M, false)), distance(N, std::vector<int>(M, INT32_MAX));
    distance[0][0] = 0;
    std::priority_queue<std::tuple<int, int, int>> queue;
    queue.push({0, 0, 0});
    while (!queue.empty()) {
        auto [_, x, y] = queue.top();
        queue.pop();
        visited[x][y] = true;
        if (x == N - 1 && y == M - 1) break;
        for (auto [n_x, n_y] : get_turns(x, y)) {
            if (visited[n_x][n_y]) continue;
            int weight = std::max(0, map[n_x][n_y] - map[x][y]);
            if (distance[n_x][n_y] > distance[x][y] + weight) {
                distance[n_x][n_y] = distance[x][y] + weight;
                queue.push({-distance[n_x][n_y], n_x, n_y});
            }
        }
    }

    return distance[N - 1][M - 1];
}

void scooter(std::vector<std::vector<int>> &map) {
    std::cout << dijkstra(map) << std::endl;
}

int main() {
    std::cin >> N >> M;
    std::vector<std::vector<int>> map(N, std::vector<int>(M));

    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < M; j++)
            std::cin >> map[i][j];

    scooter(map);

    return 0;
}