/*

Задача о назначениях

Дана целочисленная матрица C размера N × N. Требуется выбрать N ячеек так, чтобы в каждой строке и каждом столбце была выбрана ровно одна ячейка и сумма значений в выбранных ячейках было минимальна.

*/

#include <array>
#include <cinttypes>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

/// @brief Первая строка входного файла содержит N (2 ⩽ N ⩽ 300).
size_t N;

using WEIGHT [[maybe_unused]] = int;
using VERTEX [[maybe_unused]] = int;
const WEIGHT MAX_WEIGHT [[maybe_unused]] = INT32_MAX;
using VERTICES [[maybe_unused]] = std::vector<VERTEX>;
using WEIGHTS [[maybe_unused]] = std::vector<WEIGHT>;
using CONTIGUITY_MATRIX [[maybe_unused]] = std::vector<WEIGHTS>;
using EDGE [[maybe_unused]] = std::tuple<VERTEX, VERTEX, WEIGHT, WEIGHT, WEIGHT, VERTEX, VERTEX>;
using EDGES [[maybe_unused]] = std::vector<EDGE>;
using CONNECTIONS [[maybe_unused]] = std::vector<EDGES>;

VERTEX hungarian(CONTIGUITY_MATRIX &contiguity, VERTICES &match) {
    VERTICES u(N), v(N), way(N), visited(N, false);
    WEIGHTS min(N, MAX_WEIGHT);
    for (size_t i = 0; i < N; i++) {
        match[0] = i;
        VERTEX column = 0;
        std::fill(min.begin(), min.end(), MAX_WEIGHT);
        std::fill(visited.begin(), visited.end(), false);
        do {
            visited[column] = true;
            VERTEX row = match[column], min_column = 0;
            WEIGHT delta = MAX_WEIGHT;

            for (size_t j = 0; j < N; j++) {
                if (!visited[j]) {
                    VERTEX current = contiguity[row][j] - u[row] - v[j];

                    if (current < min[j])
                        min[j] = current, way[j] = column;

                    if (min[j] < delta)
                        delta = min[j], min_column = j;
                }
            }

            for (size_t j = 0; j < N; j++) {
                if (visited[j])
                    u[match[j]] += delta, v[j] -= delta;
                else
                    min[j] -= delta;
            }

            column = min_column;

        } while (match[column]);

        do {
            VERTEX min_column = way[column];
            match[column] = match[min_column];
            column = min_column;
        } while (column);
    }

    return -v[0];
}

void assignments(CONTIGUITY_MATRIX &contiguity) {
    VERTICES match(N);
    int answer = hungarian(contiguity, match);

    VERTICES result(N);
    for (size_t i = 0; i < N; i++) {
        result[match[i]] = i + 1;
    }

    std::cout << answer - 1 << std::endl;
    for (size_t i = 0; i < N; i++) {
        std::cout << i + 1 << ' ' << result[i] << std::endl;
    }
}

int main() {
    std::cin >> N;

    CONTIGUITY_MATRIX contiguity(N, WEIGHTS(N));

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            std::cin >> contiguity[i][j];
        }
    }

    assignments(contiguity);

    return 0;
}