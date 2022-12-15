/*

Перекрытие туннелей

Илон Маск, наконец-то, перебрался на Марс. К его неудовольствию, планета оказалась заселена цивилизацией враждебных роботов. Под поверхностью была обнаружена разветвлённая сеть бункеров, связанных между собой двунаправленными туннелями. Попытка изловить роботов и перепрограммировать их на дружественный лад пока не увенчалась успехом — и это всё потому, что роботы могут перемещаться по туннелям с очень большой скоростью и подпитываться энергией в специальных бункерах. Все бункера, между которыми можно перемещаться по туннелям, составляют укрепрайон, а сила укрепрайона зависит от количества бункеров в нём.

К счастью для прибывших, доставленной с Земли техники оказалось достаточно для того, чтобы иметь возможность перекрывать туннели, изолируя таким образом бункера друг от друга. План подмарсья был составлен. Оказалось, что всего имеется 10⩽V⩽10000 бункеров, 10⩽E⩽100000 туннелей.

У переселенцев имеется сценарий, по которому они собираются перекрывать туннели. Будет перекрыто 1⩽M⩽100000 туннелей. Перекрытие туннеля может привести к увеличению количества укрепрайонов, но, так как каждый из них будет слабее целого, к этому и стремятся.

*/

#include <array>
#include <cinttypes>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

/// @brief Три числа, количество бункеров V, количество туннелей E и запланированное количество перекрываемых туннелей M.
size_t V, E, M;

using VERTEX [[maybe_unused]] = int;
using VERTICES [[maybe_unused]] = std::vector<VERTEX>;
using CONNECTIONS [[maybe_unused]] = std::vector<VERTICES>;
using EDGE [[maybe_unused]] = std::pair<VERTEX, VERTEX>;

struct EDGE_HASH {
    inline std::size_t operator()(const EDGE &edge) const {
        auto [x, y] = edge;
        return x * x + y;
    }
};

using EDGES_SET [[maybe_unused]] = std::unordered_set<EDGE, EDGE_HASH>;
using EDGES [[maybe_unused]] = std::vector<EDGE>;

void destroy(EDGES_SET &ways, EDGES &destroyed) {
    VERTICES backlog;
    VERTICES components(V);

    for (size_t i = 0; i < V; i++) {
        components[i] = i;
    }

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

    size_t n_components = V;

    for (auto [x, y] : ways) {
        if (merge(x, y))
            n_components--;
    }

    backlog.push_back(n_components);

    std::reverse(destroyed.begin(), destroyed.end());

    for (auto [x, y] : destroyed) {
        if (merge(x, y)) n_components--;
        backlog.push_back(n_components);
    }

    std::reverse(backlog.begin(), backlog.end());

    for (auto x : backlog) std::cout << x << std::endl;
}

int main() {
    std::cin >> V >> E >> M;

    EDGES_SET ways;
    EDGES destroyed;

    for (size_t i = 0; i < E; i++) {
        VERTEX x, y;
        std::cin >> x >> y;
        ways.insert({x, y});
    }

    for (size_t i = 0; i < M; i++) {
        VERTEX x, y;
        std::cin >> x >> y;
        destroyed.push_back({x, y});
        ways.erase({x, y});
        ways.erase({y, x});
    }

    destroy(ways, destroyed);

    return 0;
}