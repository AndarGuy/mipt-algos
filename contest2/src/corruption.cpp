/*

Дороги и взятки

В некой стране имеется N городов, между которыми имеется сеть двунаправленных дорог такая, что из любого города можно доехать в любой другой. Расстояние между городами описывается симметричной матрицей смежности C[N,N]. Лоббисты транспортной компании обратили внимание правительства на то, что на содержание дорог требуются средства и протолкнули закон о платном проезде: весь проезд по территории страны сделать платным; стоимость проезда пропорциональна расстоянию. Так как несознательные жители могли воспользоваться альтернативными путями передвижения, часть дорог решили закрыть совсем. Оппозиция настояла на том, чтобы оставшихся дорог было достаточно для того, чтобы проехать из любого города в любой другой.

Ваша задача — определить сумму длин всех дорог, оставшихся после реализации этого решения.

*/

#include <array>
#include <cinttypes>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

size_t N;

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

using WEIGHT = int;
using VERTEX = int;
const WEIGHT MAX_WEIGHT [[maybe_unused]] = INT32_MAX;
using CONNECTIONS = std::vector<std::vector<std::pair<VERTEX, WEIGHT>>>;
using VERTICES = std::vector<VERTEX>;
using WEIGHTS = std::vector<WEIGHT>;
using QUEUE = CUSTOM_QUEUE<std::pair<WEIGHT, VERTEX>>;

WEIGHT boruvka(CONNECTIONS &connections) {
    QUEUE queue;
    VERTICES visited(N, 0);
    WEIGHTS distance(N, 0);

    queue.push({0, 0});
    visited[0] = true;
    distance[0] = 0;

    WEIGHT answer = 0;

    for (size_t i = 0; i < N && !queue.empty(); i++) {
        auto [weight, x] = queue.top();
        queue.pop();
        answer += weight;
        visited[x] = true;

        for (auto [y, weight] : connections[x]) {
            // std::cout << x << " " << y << " " << weight << std::endl;
            if (weight > distance[y] && !visited[y]) {
                queue.erase({distance[y], y});
                distance[y] = weight;
                queue.push({distance[y], y});
            }
        }
    }

    return answer;
}

void corruption(CONNECTIONS &connections) {
    std::cout << boruvka(connections) << std::endl;
}

int main() {
    std::cin >> N;

    CONNECTIONS connections(N);

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            WEIGHT weight;
            std::cin >> weight;
            if (weight > 0) connections[i].push_back({j, weight});
        }
    }

    corruption(connections);

    return 0;
}