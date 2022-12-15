/*

Передающие станции

Компания сотовой связи получила лицензии на установку вышек сотовой связи в одном из новых районов города. На каждую вышку нужно установить приёмо-передатчик, который позволит связаться с другими вышками. Для снижения общей стоимости проекта приёмо-передатчики решено закупить оптом с большой скидкой, поэтому на всех вышках они будут однотипные. Выпускается много типов передатчиков, каждый из которых имеет различную мощность, причём, чем больше мощность, тем он дороже и имеет больший радиус охватываемой территории.

Наша задача — подобрать модель передатчика с наименьшей возможной мощностью, исходя из расположения вышек для того, чтобы можно было передавать сообщения между любыми вышками, возможно, ретранслируя их.

*/

#include <array>
#include <cinttypes>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

/// @brief Первая строка содержит количество вышек 2⩽N⩽2000
size_t N;

using WEIGHT [[maybe_unused]] = double;
using VERTEX [[maybe_unused]] = int;
using VERTICES [[maybe_unused]] = std::vector<VERTEX>;
using WEIGHTS [[maybe_unused]] = std::vector<WEIGHT>;

double distance(std::pair<VERTEX, VERTEX> a, std::pair<VERTEX, VERTEX> b) {
    auto [a_x, a_y] = a;
    auto [b_x, b_y] = b;
    return sqrt((a_x - b_x) * (a_x - b_x) + (a_y - b_y) * (a_y - b_y));
}

WEIGHT boruvka(std::vector<std::pair<VERTEX, VERTEX>> &retransmitors) {
    VERTICES components(N);
    for (size_t i = 0; i < N; i++)
        components[i] = i;

    std::vector<std::pair<VERTEX, VERTEX>> nearest_retransmitor(N);

    std::function<VERTEX(VERTEX)> find_root = [&components, &find_root](VERTEX vertex) {
        if (components[vertex] == vertex) return vertex;
        return find_root(components[vertex]);
    };

    std::function<int(VERTEX, VERTEX)> merge = [&components, &find_root](VERTEX vertex_a, VERTEX vertex_b) {
        vertex_a = find_root(vertex_a), vertex_b = find_root(vertex_b);
        if (vertex_a == vertex_b) return 0;
        components[vertex_a] = vertex_b;
        return 1;
    };

    std::pair<VERTEX, VERTEX> EMPTY = {-1, -1};

    size_t n_components = N;
    WEIGHT maximum_distance = 0;
    while (n_components > 1) {
        for (size_t i = 0; i < N; i++)
            nearest_retransmitor[i] = EMPTY;

        for (size_t i = 0; i < N; i++) {
            for (size_t j = i + 1; j < N; j++) {
                VERTEX root_a = find_root(i), root_b = find_root(j);
                if (root_a == root_b) continue;
                if (nearest_retransmitor[root_a] == EMPTY || distance(retransmitors[nearest_retransmitor[root_a].first], retransmitors[nearest_retransmitor[root_a].second]) > distance(retransmitors[i], retransmitors[j])) {
                    nearest_retransmitor[root_a] = {i, j};
                }
                if (nearest_retransmitor[root_b] == EMPTY || distance(retransmitors[nearest_retransmitor[root_b].first], retransmitors[nearest_retransmitor[root_b].second]) > distance(retransmitors[i], retransmitors[j])) {
                    nearest_retransmitor[root_b] = {j, i};
                }
            }
        }

        for (size_t i = 0; i < N; i++) {
            if (nearest_retransmitor[i] == EMPTY) continue;
            auto [a, b] = nearest_retransmitor[i];
            if (merge(a, b)) {
                maximum_distance = std::max(maximum_distance, distance(retransmitors[a], retransmitors[b]));
                n_components--;
            }
        }
    }

    return maximum_distance;
}

int main() {
    std::cin >> N;

    std::vector<std::pair<VERTEX, VERTEX>> retransmitors(N);
    for (size_t i = 0; i < N; i++) {
        VERTEX x, y;
        std::cin >> x >> y;
        retransmitors[i] = {x, y};
    }

    std::cout << boruvka(retransmitors) << std::endl;

    return 0;
}