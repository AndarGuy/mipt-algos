/*

Компоненты связности

Вам задан неориентированный граф c N вершинами и M ребрами. В графе отсутствуют петли и кратные ребра.

Определите компоненты связности заданного графа.

*/

#include <iostream>
#include <vector>

void dfs(int x, int color, std::vector<std::vector<int>> &connections, std::vector<int> &visited) {
    if (visited[x]) return;
    visited[x] = color;
    for (int y : connections[x]) dfs(y, color, connections, visited);
}

void components(int N, int M, std::vector<std::vector<int>> &connections) {
    std::vector<int> visited(N, 0);
    int sub = 0;
    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            dfs(i, sub + 1, connections, visited);
            sub++;
        }
    }
    std::cout << sub << std::endl;
    for (size_t i = 0; i < N; i++) {
        std::cout << visited[i] << " ";
    }
}

int main() {
    int N, M;
    std::cin >> N >> M;
    std::vector<std::vector<int>> connections(N);
    for (int i = 0; i < M; i++) {
        int m, n;
        std::cin >> m >> n;
        m--;
        n--;
        connections[m].push_back(n);
        connections[n].push_back(m);
    }

    components(N, M, connections);

    return 0;
}