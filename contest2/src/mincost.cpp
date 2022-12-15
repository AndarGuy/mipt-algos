/*

Максимальный поток минимальной стоимости

Задан ориентированный граф, каждое ребро которого обладает пропускной способностью и стоимостью. Найдите максимальный поток минимальной стоимости из вершины с номером 1 в вершину с номером N.

*/

#include <array>
#include <cinttypes>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

/// @brief Первая строка входного файла содержит N и M — количество вершин и количество ребер графа (2 ⩽ N ⩽ 100, 0 ⩽ M ⩽ 1000).
size_t N, M;

using WEIGHT [[maybe_unused]] = long long int;
using VERTEX [[maybe_unused]] = int;
const WEIGHT MAX_WEIGHT [[maybe_unused]] = INT64_MAX;
using VERTICES [[maybe_unused]] = std::vector<VERTEX>;
using WEIGHTS [[maybe_unused]] = std::vector<WEIGHT>;
using CONTIGUITY_MATRIX [[maybe_unused]] = std::vector<WEIGHTS>;
using EDGE [[maybe_unused]] = std::tuple<VERTEX, VERTEX, WEIGHT, WEIGHT, WEIGHT, VERTEX, VERTEX>;
using EDGES [[maybe_unused]] = std::vector<EDGE>;
using CONNECTIONS [[maybe_unused]] = std::vector<EDGES>;

WEIGHT mincost(EDGES &edges) {
    std::vector<VERTEX> used_edges(N, 0);

    auto bellman = [&edges, &used_edges](VERTEX start, VERTEX end) {
        WEIGHTS distance(N, MAX_WEIGHT);
        distance[start] = 0;

        while (true) {
            bool exit = true;
            for (auto [x, y, bandwidth, cost, flow, n_edge, n_reversed_edge] : edges) {
                if (distance[x] != MAX_WEIGHT && distance[y] > distance[x] + cost && bandwidth > flow) {
                    distance[y] = distance[x] + cost;
                    used_edges[y] = n_edge;
                    exit = false;
                }
            }
            if (exit) break;
        }

        return distance[end];
    };

    WEIGHT cost = 0;
    while (bellman(0, N - 1) < MAX_WEIGHT) {
        WEIGHT current_flow = MAX_WEIGHT;
        for (VERTEX vertex = N - 1; vertex; vertex = std::get<0>(edges[used_edges[vertex]])) {
            auto [x, y, bandwidth, cost, flow, n_edge, n_reversed_edge] = edges[used_edges[vertex]];
            current_flow = std::min(current_flow, bandwidth - flow);
        }
        for (VERTEX vertex = N - 1; vertex; vertex = std::get<0>(edges[used_edges[vertex]])) {
            auto [_x, _y, _bandwidth, _cost, _flow, n_edge, n_reversed_edge] = edges[used_edges[vertex]];
            std::get<4>(edges[n_edge]) += current_flow;
            std::get<4>(edges[n_reversed_edge]) -= current_flow;
            cost += current_flow * std::get<3>(edges[n_edge]);
        }
    }
    return cost;
}

int main() {
    std::cin >> N >> M;

    EDGES edges(M * 2);

    for (size_t i = 0; i < M; i++) {
        VERTEX x, y;
        WEIGHT bandwidth, cost;
        std::cin >> x >> y >> bandwidth >> cost;
        edges[i * 2] = {x - 1, y - 1, bandwidth, cost, 0, i * 2, i * 2 + 1};
        edges[i * 2 + 1] = {y - 1, x - 1, 0, -cost, 0, i * 2 + 1, i * 2};
    }

    std::cout << mincost(edges) << std::endl;

    return 0;
}