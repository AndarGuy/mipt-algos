/*

Диаметр графа

Дан ориентированный взвешенный граф. Вам необходимо найти пару вершин, кратчайшее расстояние от одной из которых до другой максимально среди всех пар вершин.

*/

#include <array>
#include <cinttypes>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

/// @brief В первой строке вводится единственное число N (1 ⩽ N ⩽ 100) – количество вершин графа.
size_t N;

using WEIGHT = int;
using VERTEX = int;
using CONNECTIONS = std::vector<std::vector<std::pair<VERTEX, WEIGHT>>>;
using VERTICES = std::vector<VERTEX>;
using WEIGHTS = std::vector<WEIGHT>;

WEIGHT distance(VERTEX start, CONNECTIONS connections) {
    VERTICES visited(N, false), distance(N, INT32_MAX);
    distance[start] = 0;

    VERTEX x;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            WEIGHT min_distance = INT32_MAX;
            for (size_t j = 0; j < N; j++) {
                if (visited[j]) continue;
                if (distance[j] < min_distance) {
                    min_distance = distance[j];
                    x = j;
                }
            }

            visited[x] = true;

            for (auto [y, w] : connections[x]) {
                distance[y] = std::min(distance[y], distance[x] + w);
            }
        }
    }

    return distance[x];
}

void diameter(CONNECTIONS connections) {
    WEIGHT max_distance = 0;
    for (size_t i = 0; i < N; i++)
        max_distance = std::max(distance(i, connections), max_distance);

    std::cout << max_distance << std::endl;
}

int main() {
    std::cin >> N;

    CONNECTIONS connections(N);

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            WEIGHT weight;
            std::cin >> weight;
            if (weight == -1) continue;
            connections[i].push_back({j, weight});
        }
    }

    diameter(connections);

    return 0;
}