/*

Двудольный граф

Двудольный граф — неориентированный граф, множество вершин которого можно разбить на два непустых подмножества таких, что не существует рёбер, соединяющих пару вершин, принадлежащих одному подмножеству.

Задан неориентированный граф с 2⩽N⩽100 вершинами и M рёбрами. Требуется определить, является ли он двудольным.

*/

#include <iostream>
#include <vector>

using namespace std;

int dfs(int x, int color, vector<vector<int>> &connections, vector<int> &visited) {
    if (visited[x] != -1) {
        if (visited[x] == color)
            return true;
        else if (visited[x] != color)
            return false;
    } else
        visited[x] = color;

    for (int y : connections[x])
        if (!dfs(y, !color, connections, visited)) return false;

    return true;
}

int bipartite(int N, int M, vector<vector<int>> &connections) {
    vector<int> visited(N, -1);
    for (int i = 0; i < N; i++)
        if (visited[i] == -1)
            if (!dfs(i, 0, connections, visited)) return false;

    return true;
}

int main() {
    int N, M;
    cin >> N >> M;
    vector<vector<int>> connections(N);
    for (int i = 0; i < M; i++) {
        int m, n;
        cin >> m >> n;
        connections[m].push_back(n);
        connections[n].push_back(m);
    }

    cout << (bipartite(N, M, connections) ? "YES" : "NO");

    return 0;
}