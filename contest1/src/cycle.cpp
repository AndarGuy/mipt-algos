/*

Циклы

Дан ориентированный невзвешенный граф без петель и кратных рёбер. Необходимо определить есть ли в нём циклы, и если есть, то вывести любой из них.

*/

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

enum {
    WHITE,
    GREY,
    BLACK
};

bool dfs(int x, vector<vector<int>> &connections, vector<int> &visited, vector<bool> &marks, vector<int> &backlog, vector<int> &cycle, stack<int> &stack) {
    stack.push(x);

    while (!stack.empty()) {
        int x = stack.top();
        visited[x] = GREY;
        if (marks[x]) {
            visited[x] = BLACK;
            stack.pop();
        } else {
            marks[x] = true;
            for (auto y : connections[x]) {
                if (visited[y] == GREY) {
                    for (int start = y, end = x; end != backlog[start]; end = backlog[end]) cycle.push_back(end + 1);
                    return true;
                } else if (!visited[y]) {
                    stack.push(y);
                    backlog[y] = x;
                }
            }
        }
    }

    return false;
}

void cycle(int N, int M, vector<vector<int>> &connections) {
    vector<int> visited(N, WHITE), backlog(N, -1), cycle;
    vector<bool> marks(N, false);
    stack<int> stack;
    for (int i = 0; i < N; i++) {
        cycle.clear();
        if (!visited[i]) {
            if (dfs(i, connections, visited, marks, backlog, cycle, stack)) {
                std::cout << "YES" << std::endl;
                std::reverse(cycle.begin(), cycle.end());
                for (auto x : cycle) std::cout << x << ' ';
                std::cout << std::endl;
                return;
            }
        }
    }

    std::cout << "NO" << std::endl;
}

int main() {
    int N, M;
    cin >> N >> M;
    vector<vector<int>> connections(N), backwards(N);
    for (int i = 0; i < M; i++) {
        int m, n;
        cin >> m >> n;
        m--;
        n--;
        connections[m].push_back(n);
        backwards[n].push_back(m);
    }

    cycle(N, M, connections);

    return 0;
}