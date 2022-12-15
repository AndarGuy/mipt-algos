/*

Кратчайшие пути

Ориентированный взвешенный граф с N вершинами задан своими M рёбрами Ei, возможно отрицательного веса. Требуется найти все кратчайшие пути от вершины S до остальных вершин. Если граф содержит отрицательные циклы, вывести слово IMPOSSIBLE. Если от вершины S до какой-либо из вершин нет маршрута, то в качестве длины маршрута вывести слово UNREACHABLE.

Вершины графа нумеруются, начиная с нуля.

*/

#include <array>
#include <cinttypes>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

/// @brief Ориентированный взвешенный граф с N вершинами задан своими M рёбрами. Требуется найти все кратчайшие пути от вершины S до остальных вершин.
size_t N, M, S;

using WEIGHT = int;
using VERTEX = int;
const WEIGHT MAX_WEIGHT = INT32_MAX;
using CONNECTIONS = std::vector<std::vector<std::pair<VERTEX, WEIGHT>>>;
using VERTICES = std::vector<VERTEX>;
using WEIGHTS = std::vector<WEIGHT>;
using QUEUE = std::priority_queue<std::pair<WEIGHT, VERTEX>>;

void IMPOSSIBLE() {
    std::cout << "IMPOSSIBLE" << std::endl;
    exit(0);
}

WEIGHTS dijkstra(CONNECTIONS &connections) {
    VERTICES distance(N, MAX_WEIGHT), visited(N, false);
    QUEUE queue;

    distance[S] = 0;
    queue.push({0, S});

    while (!queue.empty()) {
        auto [_, x] = queue.top();
        queue.pop();
        visited[x] = true;
        for (auto [y, length] : connections[x]) {
            if (visited[y] && distance[y] > distance[x] + length) IMPOSSIBLE();
            if (visited[y]) continue;
            if (distance[y] > distance[x] + length) {
                distance[y] = distance[x] + length;
                queue.push({-distance[y], y});
            }
        }
    }

    return distance;
}

WEIGHTS bellman(CONNECTIONS &connections) {
    VERTICES distance(N, MAX_WEIGHT);
    distance[S] = 0;

    for (size_t i = 0; i < N; i++) {
        for (size_t x = 0; x < N; x++) {
            for (auto [y, weight] : connections[x]) {
                if (distance[x] != MAX_WEIGHT && distance[x] + weight < distance[y]) {
                    distance[y] = distance[x] + weight;
                }
            }
        }
    }

    for (size_t x = 0; x < N; x++) {
        for (auto [y, weight] : connections[x]) {
            if (distance[x] != MAX_WEIGHT && distance[x] + weight < distance[y]) {
                IMPOSSIBLE();
            }
        }
    }

    return distance;
}

void shortest(CONNECTIONS &connections) {
    WEIGHTS weights = bellman(connections);
    for (auto weight : weights) {
        if (weight == MAX_WEIGHT)
            std::cout << "UNREACHABLE"
                      << " ";
        else
            std::cout << weight << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::cin >> N >> M >> S;

    CONNECTIONS connections(N);

    for (size_t i = 0; i < M; i++) {
        VERTEX x, y;
        WEIGHT weight;
        std::cin >> x >> y >> weight;
        connections[x].push_back({y, weight});
    }

    shortest(connections);

    return 0;
}