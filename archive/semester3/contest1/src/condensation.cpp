/*

Конденсация

Требуется найти количество ребер в конденсации ориентированного графа. Примечание: конденсация графа не содержит кратных ребер.

*/

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

void condensation(int N, int M, std::vector<std::vector<int>> connections, std::vector<std::vector<int>> backwards) {
    std::vector<int> visited(N, false), sorted, colors(N, -1);

    std::function<void(int)> topsort = [&connections, &visited, &sorted, &topsort](int vertex) -> void {
        visited[vertex] = true;
        for (int y : connections[vertex])
            if (!visited[y]) topsort(y);
        sorted.push_back(vertex);
    };
    std::function<void(int, int)> paint = [&colors, &backwards, &paint](int vertex, int color) -> void {
        colors[vertex] = color;
        for (int y : backwards[vertex])
            if (colors[y] == -1) paint(y, color);
    };

    for (int x = 0; x < N; x++)
        if (!visited[x]) topsort(x);

    std::reverse(sorted.begin(), sorted.end());
    int c = 0;
    for (auto x : sorted)
        if (colors[x] == -1) paint(x, c++);

    std::set<std::pair<int, int>> answer;
    for (size_t x = 0; x < N; x++) {
        for (auto y : connections[x]) {
            if (colors[x] != colors[y]) answer.insert({colors[x], colors[y]});
        }
    }

    std::cout << answer.size() << std::endl;
}

int main() {
    int N, M;
    std::cin >> N >> M;
    std::vector<std::vector<int>> connections(N), backwards(N);
    for (size_t i = 0; i < M; i++) {
        int x, y;
        std::cin >> x >> y;
        x--;
        y--;
        connections[x].push_back(y);
        backwards[y].push_back(x);
    }

    condensation(N, M, connections, backwards);
}