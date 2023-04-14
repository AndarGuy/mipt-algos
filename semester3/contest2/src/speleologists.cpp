/*

Спелеологи

Гора состоит из N пещер и M тоннелей между ними. Каждый тоннель имеет определённую длину.

Спелеологи, исследующие гору, находятся в пещере S, выход наружу находится в пещере F.

Продвигаясь по пещерам, спелеологи оставили K не очень хорошо затушенных костров, но вот в какой-то момент времени произошёл толчок и все костры стали дымить. Дым распространяется по всем тоннелям, ведущим от пещеры, одновременно со скоростью 1 метр в секунду. Как только дым доходит до какой-либо пещеры, он начинает распространяться по всем тоннелям, ведущим из неё. Сами спелеологи, почувствовав толчок, стали пробираться к выходу со скоростью 1 метр в секунду. Вопрос состоит в том, успеют ли они добраться до выхода прежде чем столкнутся с дымом. Если спелеологи и дым достигают выхода одновременно, то побеждает дым. Известно, что в начальный момент времени спелеологи находятся в пещере без дыма.

*/

#include <array>
#include <cinttypes>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

/// @brief Первая строка содержит количество пещер N, количество тоннелей M и количество костров K.
size_t N, M, K;

/// @brief Последняя строка — числа S, номер пещеры со спелеологами и F — номер пещеры, являющейся выходом.
size_t S, F;

int escape(std::vector<int> &smoked, std::vector<std::vector<std::pair<int, int>>> &connections) {
    std::vector<int> passed(N, INT32_MAX), visited(N, false);
    std::priority_queue<std::pair<int, int>> queue;

    passed[S] = 0;
    queue.push({0, S});

    while (!queue.empty()) {
        auto [_, x] = queue.top();
        queue.pop();
        visited[x] = true;
        for (auto [y, length] : connections[x]) {
            if (visited[y]) continue;
            if (passed[x] + length + 1 > smoked[y]) continue;
            if (passed[y] > passed[x] + length) {
                passed[y] = passed[x] + length;
                queue.push({-passed[y], y});
            }
        }
    }

    return passed[F];
}

std::vector<int> fire(std::vector<int> &firepits, std::vector<std::vector<std::pair<int, int>>> &connections) {
    std::vector<int> smoked(N, INT32_MAX), visited(N, false);
    std::priority_queue<std::pair<int, int>> queue;
    for (auto burning : firepits) {
        smoked[burning] = 0;
        queue.push({0, burning});
    }

    while (!queue.empty()) {
        auto [_, x] = queue.top();
        queue.pop();
        visited[x] = true;
        for (auto [y, length] : connections[x]) {
            if (visited[y]) continue;
            if (smoked[y] > smoked[x] + length) {
                smoked[y] = smoked[x] + length;
                queue.push({-smoked[y], y});
            }
        }
    }

    return smoked;
}

void speleologists(std::vector<int> firepits, std::vector<std::vector<std::pair<int, int>>> connections) {
    auto smoked = fire(firepits, connections);
    int escaped = escape(smoked, connections);
    if (escaped == INT32_MAX)
        std::cout << -1 << std::endl;
    else
        std::cout << escaped << std::endl;
}

int main() {
    std::cin >> N >> M >> K;

    std::vector<int> firepits(K);
    for (size_t i = 0; i < K; i++) {
        std::cin >> firepits[i];
        firepits[i]--;
    }

    std::vector<std::vector<std::pair<int, int>>> connections(N);
    for (size_t i = 0; i < M; i++) {
        int x, y, w;
        std::cin >> x >> y >> w;
        connections[x - 1].push_back({y - 1, w});
        connections[y - 1].push_back({x - 1, w});
    }

    std::cin >> S >> F;
    S--;
    F--;

    speleologists(firepits, connections);

    return 0;
}