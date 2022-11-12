/*

Задан ориентированный ациклический граф. Требуется определить минимальное количество не пересекающихся по вершинам путей, покрывающих все вершины.

*/

#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

int distance(int N, std::string primary, std::string secondary) {
    std::deque<std::string> primary_queue, secondary_queue;
    std::unordered_map<std::string, int> primary_graph, secondary_graph;

    primary_graph[primary] = secondary_graph[secondary] = 0;
    primary_queue.push_back(primary);
    secondary_queue.push_back(secondary);

    if (primary == secondary)
        return 0;

    std::string temp;
    while (primary_queue.size() > 0 && secondary_queue.size() > 0) {
        std::string a = primary_queue.front(), b = secondary_queue.front();
        int begin = primary_graph[a] + 1, end = secondary_graph[b] + 1;

        if (begin > end) {
            std::swap(begin, end);
            std::swap(primary_graph, secondary_graph);
            std::swap(primary_queue, secondary_queue);
            std::swap(a, b);
        }

        primary_queue.pop_front();
        for (size_t i = 0; i < N - 1; i++) {
            for (size_t j = i + 2; j < N + 1; j++) {
                temp = a;
                std::reverse(temp.begin() + i, temp.begin() + j);
                if (primary_graph.find(temp) == primary_graph.end()) {
                    primary_graph[temp] = begin;
                    primary_queue.push_back(temp);
                }
                if (secondary_graph.find(temp) != secondary_graph.end())
                    return begin + secondary_graph[temp];
            }
        }
    }

    return 0;
}

void reverse(int N, std::string primary, std::string secondary) {
    std::cout << distance(N, primary, secondary);
}

int main() {
    int N;
    std::cin >> N;
    std::string primary, secondary;
    int x;
    for (size_t i = 0; i < N; i++) {
        std::cin >> x;
        primary.push_back(x);
    }
    for (size_t i = 0; i < N; i++) {
        std::cin >> x;
        secondary.push_back(x);
    }

    reverse(N, primary, secondary);
}