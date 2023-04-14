/*

Путь

В неориентированном графе требуется найти длину кратчайшего пути между двумя вершинами.

*/

#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

void dijkstra(int from, int to, std::vector<std::vector<int>> &connections, std::vector<std::vector<int>> &weights, std::vector<long> &distance) {
    distance[from] = 0;
    std::queue<int> queue;
    queue.push(from);
    while (!queue.empty()) {
        int x = queue.front();
        queue.pop();
        for (auto y : connections[x]) {
            if (distance[y] > distance[x] + weights[x][y]) {
                distance[y] = distance[x] + weights[x][y];
                queue.push(y);
            }
        }
        if (x == to) break;
    }
}

int main() {
    int N;
    std::cin >> N;

    std::vector<std::vector<int>> connections(N);
    std::vector<std::vector<int>> weights(N, std::vector<int>(N, 1));

    int x;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            std::cin >> x;
            if (x) connections[i].push_back(j);
        }
    }

    int from, to;
    std::cin >> from >> to;
    std::vector<long> distance(N, INT32_MAX);
    dijkstra(from - 1, to - 1, connections, weights, distance);

    if (distance[to - 1] != INT32_MAX)
        std::cout << distance[to - 1] << std::endl;
    else
        std::cout << -1 << std::endl;
}