/*

Дан неориентированный граф без петель и кратных рёбер. Требуется найти все точки сочленения в нем.

*/

#include <iostream>
#include <set>
#include <vector>

enum color {
    WHITE,
    GREY,
    BLACK
};

int timer;
void dfs(int x, int p, std::vector<int> &visited, std::vector<int> &time_in, std::vector<int> &ret, std::vector<std::vector<int>> &connections, std::set<int> &answer) {
    visited[x] = GREY;
    time_in[x] = ret[x] = timer++;
    int children = 0;
    for (auto y : connections[x]) {
        if (y == p) continue;
        if (visited[y]) {
            ret[x] = std::min(ret[x], time_in[y]);
        } else {
            dfs(y, x, visited, time_in, ret, connections, answer);
            ret[x] = std::min(ret[x], ret[y]);
            if (p != -1 && ret[y] >= time_in[x])
                answer.insert(x);
            children++;
        }
    }
    if (p == -1 && children > 1) answer.insert(x);
}

void junctions(int N, int M, std::vector<std::vector<int>> connections) {
    std::vector<int> visited(N, WHITE),
        time_in(N, 0), ret(N, 0);
    std::set<int> answer;
    for (size_t i = 0; i < N; i++) {
        timer = 0;
        if (!visited[i]) dfs(0, -1, visited, time_in, ret, connections, answer);
    }

    std::cout << answer.size() << std::endl;
    for (auto x : answer) {
        std::cout << x + 1 << ' ';
    }
    std::cout << std::endl;
}

int main() {
    int N, M;
    std::cin >> N >> M;
    std::vector<std::vector<int>> connections(N);
    for (size_t i = 0; i < M; i++) {
        int x, y;
        std::cin >> x >> y;
        x--;
        y--;
        connections[x].push_back(y);
        connections[y].push_back(x);
    }

    junctions(N, M, connections);
}