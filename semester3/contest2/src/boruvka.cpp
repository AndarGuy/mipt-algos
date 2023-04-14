/*

Минимальное остовное дерево: алгоритм Борувки

Неориентированныий взвешенныий граф с количеством вершин 5⩽N⩽10000 описан своими 5⩽M⩽500000 рёбрами. Вес каждого ребра 1⩽Wi⩽1000000.

Гарантируется, что имеется минимальное остовное дерево, связывающее все вершины графа. Требуется найти его общий вес, то есть сумму всех его рёбер.

Вершины графа нумеруются, начиная с 1.

*/

#include <array>
#include <cinttypes>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

/// @brief Неориентированныий взвешенныий граф с количеством вершин 5⩽N⩽10000 описан своими 5⩽M⩽500000 рёбрами.
size_t N, M;

using WEIGHT [[maybe_unused]] = int;
using VERTEX [[maybe_unused]] = int;
const WEIGHT MAX_WEIGHT [[maybe_unused]] = INT32_MAX;
using CONNECTIONS [[maybe_unused]] = std::vector<std::vector<std::pair<VERTEX, WEIGHT>>>;
using VERTICES [[maybe_unused]] = std::vector<VERTEX>;
using WEIGHTS [[maybe_unused]] = std::vector<WEIGHT>;
using CONTIGUITY_MATRIX [[maybe_unused]] = std::vector<WEIGHTS>;
using EDGE [[maybe_unused]] = std::tuple<VERTEX, VERTEX, WEIGHT>;
using EDGES [[maybe_unused]] = std::vector<EDGE>;

WEIGHT boruvka(EDGES &edges) {
    VERTICES components(N);
    for (size_t i = 0; i < N; i++)
        components[i] = i;

    WEIGHTS nearest_edge(N);

    std::function<VERTEX(VERTEX)> find_root = [&components, &find_root](VERTEX vertex) {
        if (components[vertex] == vertex) return vertex;
        return find_root(components[vertex]);
    };

    std::function<int(VERTEX, VERTEX)> merge = [&components, &find_root](VERTEX vertex_a, VERTEX vertex_b) {
        vertex_a = find_root(vertex_a), vertex_b = find_root(vertex_b);
        if (vertex_a == vertex_b) return 0;
        components[vertex_a] = vertex_b;
        return 1;
    };

    WEIGHT answer = 0, n_components = N;
    while (n_components > 1) {
        std::fill(nearest_edge.begin(), nearest_edge.end(), -1);
        for (size_t edge = 0; edge < M; edge++) {
            auto [x, y, weight] = edges[edge];
            VERTEX root_x = find_root(x), root_y = find_root(y);
            if (root_x == root_y) continue;
            if (nearest_edge[root_x] < 0 || std::get<2>(edges[nearest_edge[root_x]]) > weight) {
                nearest_edge[root_x] = edge;
            }
            if (nearest_edge[root_y] < 0 || std::get<2>(edges[nearest_edge[root_y]]) > weight) {
                nearest_edge[root_y] = edge;
            }
        }

        for (size_t i = 0; i < N; i++) {
            if (nearest_edge[i] < 0) continue;
            auto [x, y, weight] = edges[nearest_edge[i]];
            if (merge(x, y)) {
                answer += weight;
                n_components--;
            }
        }
    }

    return answer;
}

int main() {
    std::cin >> N >> M;

    EDGES edges(M);

    for (size_t i = 0; i < M; i++) {
        VERTEX x, y;
        WEIGHT weight;
        std::cin >> x >> y >> weight;
        edges[i] = {x - 1, y - 1, weight};
    }

    std::cout << boruvka(edges) << std::endl;

    return 0;
}