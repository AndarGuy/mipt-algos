/*

Задан ориентированный ациклический граф. Требуется определить минимальное количество не пересекающихся по вершинам путей, покрывающих все вершины.

*/

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

bool dfs(int vertex, std::vector<std::vector<int>> &connections, std::vector<int> &visited, std::vector<int> &matching) {
    if (visited[vertex]) return false;
    visited[vertex] = true;
    for (auto y : connections[vertex]) {
        if (matching[y] < 0 || dfs(matching[y], connections, visited, matching)) {
            matching[y] = vertex;
            return true;
        }
    }
    return false;
}

void coverage(int N, int M, std::vector<std::vector<int>> forwards) {
    std::vector<int> matching(N, -1);
    std::vector<int> visited(N, false);

    for (size_t i = 0; i < N; i++) {
        std::fill(visited.begin(), visited.end(), false);
        dfs(i, forwards, visited, matching);
    }

    int result = 0;
    for (size_t i = 0; i < N; i++)
        if (matching[i] != -1) result++;

    std::cout << N - result << std::endl;
}

int main() {
    int N, M;
    std::cin >> N >> M;
    std::vector<std::vector<int>> forwards(N);
    int x, y;
    for (size_t i = 0; i < M; i++) {
        std::cin >> x >> y;
        forwards[x - 1].push_back(y - 1);
    }

    coverage(N, M, forwards);
}