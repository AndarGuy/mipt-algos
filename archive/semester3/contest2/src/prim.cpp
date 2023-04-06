/*

Неориентированныий взвешенныий граф с количеством вершин 5⩽N⩽10000 описан своими 5⩽M⩽500000 рёбрами. Вес каждого ребра 1⩽Wi⩽1000000.

Гарантируется, что имеется минимальное остовное дерево, связывающее все вершины графа. Требуется найти его общий вес, то есть сумму всех его рёбер.

Вершины графа нумеруются, начиная с 1.

*/

#include <array>
#include <cinttypes>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

size_t N, M;

template <typename T>
class CUSTOM_QUEUE : public std::priority_queue<T, std::vector<T>> {
   public:
    void erase(const T &value) {
        auto iterator = std::find(this->c.begin(), this->c.end(), value);

        if (iterator == this->c.end())
            return;

        if (iterator == this->c.begin())
            this->pop();

        else {
            this->c.erase(iterator);
            std::make_heap(this->c.begin(), this->c.end(), this->comp);
        }
    }
};

using WEIGHT = long long int;
using VERTEX = int;
const WEIGHT MAX_WEIGHT = INT64_MAX;
using CONNECTIONS = std::vector<std::vector<std::pair<VERTEX, WEIGHT>>>;
using VERTICES = std::vector<VERTEX>;
using WEIGHTS = std::vector<WEIGHT>;
using QUEUE = CUSTOM_QUEUE<std::pair<WEIGHT, VERTEX>>;

WEIGHT boruvka(CONNECTIONS &connections) {
    QUEUE queue;
    VERTICES visited(N, 0);
    WEIGHTS distance(N, MAX_WEIGHT);

    queue.push({0, 0});
    visited[0] = true;
    distance[0] = 0;

    WEIGHT answer = 0;

    for (size_t i = 0; i < N && !queue.empty(); i++) {
        auto [weight, x] = queue.top();
        queue.pop();
        answer += -weight;
        visited[x] = true;

        for (auto [y, weight] : connections[x]) {
            // std::cout << x << " " << y << " " << weight << std::endl;
            if (weight < distance[y] && !visited[y]) {
                queue.erase({-distance[y], y});
                distance[y] = weight;
                queue.push({-distance[y], y});
            }
        }
    }

    return answer;
}

void prim(CONNECTIONS &connections) {
    std::cout << boruvka(connections) << std::endl;
}

int main() {
    std::cin >> N >> M;

    CONNECTIONS connections(N);

    for (size_t i = 0; i < M; i++) {
        VERTEX x, y;
        WEIGHT weight;
        std::cin >> x >> y >> weight;
        x--;
        y--;
        connections[x].push_back({y, weight});
        connections[y].push_back({x, weight});
    }

    prim(connections);

    return 0;
}