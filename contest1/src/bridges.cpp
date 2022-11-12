/*

Дан неориентированный граф без петель и кратных рёбер. Требуется в нём найти все мосты.

*/

#include <iostream>
#include <set>
#include <vector>

#define MAXN 20000

enum {
    WHITE,
    GREY,
    BLACK
};

void dfs(int x, int p, std::vector<int> &visited, std::vector<int> &time_in, std::vector<int> &ret, std::vector<std::vector<int>> &connections, std::set<int> &answer, std::vector<std::pair<int, int>> &edges) {
    static int timer = 0;
    visited[x] = GREY;
    time_in[x] = ret[x] = timer++;
    for (auto y : connections[x]) {
        if (y == p) continue;
        if (visited[y]) {
            ret[x] = std::min(ret[x], time_in[y]);
        } else {
            dfs(y, x, visited, time_in, ret, connections, answer, edges);
            ret[x] = std::min(ret[x], ret[y]);
            if (ret[y] > time_in[x]) {
                auto find = std::find(edges.begin(), edges.end(), std::pair<int, int>{x, y});
                if (find != edges.end())
                    answer.insert(find - edges.begin() + 1);
                else
                    answer.insert(std::find(edges.begin(), edges.end(), std::pair<int, int>{y, x}) - edges.begin() + 1);
            }
        }
    }
}

void bridges(int N, int M, std::vector<std::pair<int, int>> &edges, std::vector<std::vector<int>> &connections) {
    std::vector<int> visited(N, WHITE),
        time_in(N, 0), ret(N, 0);
    std::set<int> answer;
    for (size_t i = 0; i < N; i++) {
        if (!visited[i]) dfs(i, -1, visited, time_in, ret, connections, answer, edges);
    }

    std::cout << answer.size() << std::endl;
    for (auto x : answer) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;
}

int main() {
    int N, M;
    std::cin >> N >> M;
    std::vector<std::pair<int, int>> edges(M);
    std::vector<std::vector<int>> connections(N);
    for (size_t i = 0; i < M; i++) {
        int x, y;
        std::cin >> x >> y;
        x--;
        y--;
        connections[x].push_back(y);
        connections[y].push_back(x);
        edges[i] = {x, y};
    }

    bridges(N, M, edges, connections);
}