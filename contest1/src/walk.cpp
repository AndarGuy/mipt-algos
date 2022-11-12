/*

Прогулка

Взвешенный граф задан своими рёбрами. Вес ребра — положительное целое число, не превосходящее 10 — стоимость прогулки по ребру.

Найдите самый дешёвый маршрут прогулки, который должен начинаться и оканчиваться в одной и той же вершине и который не проходит по одному ребру дважды.

*/

#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

void dijkstra(int from, int to, std::vector<std::vector<std::pair<int, int>>> &connections, std::vector<long> &distance) {
    distance[from] = 0;
    std::queue<int> queue;
    queue.push(from);
    while (!queue.empty()) {
        int x = queue.front();
        queue.pop();
        for (auto pair : connections[x]) {
            int y = pair.first, weight = pair.second;
            if (distance[y] > distance[x] + weight) {
                distance[y] = distance[x] + weight;
                queue.push(y);
            }
        }
        if (x == to) break;
    }
}

void walk(int N, int M, std::vector<std::vector<std::pair<int, int>>> connections, std::vector<std::pair<int, int>> edges) {
    std::vector<long> distance(N, INT32_MAX);
    long shortest_cycle = INT32_MAX;
    for (size_t x = 0; x < N; x++) {
        for (auto pair : connections[x]) {
            int y = pair.first, weight = pair.second;
            connections[x].erase(std::find(connections[x].begin(), connections[x].end(), std::pair<int, int>(y, weight)));
            connections[y].erase(std::find(connections[y].begin(), connections[y].end(), std::pair<int, int>(x, weight)));

            std::fill(distance.begin(), distance.end(), INT32_MAX);
            dijkstra(x, y, connections, distance);
            shortest_cycle = std::min(shortest_cycle, distance[y] + weight);

            connections[x].push_back(std::make_pair(y, weight));
            connections[y].push_back(std::make_pair(x, weight));
        }
    }

    std::cout << shortest_cycle << std::endl;
}

int main() {
    int N, M;
    std::cin >> N >> M;

    std::vector<std::vector<std::pair<int, int>>> connections(N);
    std::vector<std::pair<int, int>> edges(M);

    int x, y, cost;
    for (size_t i = 0; i < M; i++) {
        std::cin >> x >> y >> cost;
        connections[x - 1].push_back({y - 1, cost});
        connections[y - 1].push_back({x - 1, cost});
        edges[i] = {x - 1, y - 1};
    }

    walk(N, M, connections, edges);
}