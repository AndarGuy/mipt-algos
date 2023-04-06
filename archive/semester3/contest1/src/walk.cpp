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

long dijkstra(int from, std::vector<std::vector<std::tuple<int, int, int>>> &connections, std::vector<long> &distance) {
    std::vector<int> visited(connections.size());
    distance[from] = 0;
    std::queue<std::pair<int, int>> queue[16];
    long answer = INT64_MAX;
    queue[0].push({from, 0});
    size_t queue_size = 1, index = 0;
    while (queue_size) {
        while (queue[index].empty()) {
            index++;
            index %= 16;
        }

        auto pair = queue[index].front();
        int x = pair.first, p = pair.second;
        queue[index].pop();
        queue_size--;

        if (visited[x] != 0)
            continue;

        visited[x] = 1;

        for (auto &pair : connections[x]) {
            int y = std::get<0>(pair), weight = std::get<1>(pair), edge = std::get<2>(pair);
            if (distance[y] > distance[x] + weight) {
                distance[y] = distance[x] + weight;
                queue[distance[y] % 16].push({y, edge});
                queue_size++;
            } else if (p != edge) {
                answer = std::min(answer, distance[x] + distance[y] + weight);
            }
        }
    }
    return answer;
}

void walk(int N, int M, std::vector<std::vector<std::tuple<int, int, int>>> connections) {
    std::vector<long> distance(N, INT32_MAX);
    long shortest_cycle = INT32_MAX;
    for (size_t x = 0; x < N; x++) {
        std::fill(distance.begin(), distance.end(), INT32_MAX);
        shortest_cycle = std::min(shortest_cycle, dijkstra(x, connections, distance));
    }
    std::cout << shortest_cycle << std::endl;
}

int main() {
    int N, M;
    std::cin >> N >> M;

    std::vector<std::vector<std::tuple<int, int, int>>> connections(N);

    int x, y, cost;
    for (size_t i = 0; i < M; i++) {
        std::cin >> x >> y >> cost;
        connections[x - 1].push_back({y - 1, cost, i});
        connections[y - 1].push_back({x - 1, cost, i});
    }

    walk(N, M, connections);
}