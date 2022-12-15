/*

Обмен валют

Рик решил на день почувствовать себя бизнесменом!

В городе есть несколько обменников валюты. В рамках данной задачи считаем, что каждый обменник специализируется только на двух валютах и может производить операции только с ними. Возможно, существуют обменники, специализирующиеся на одинаковых парах валют. В каждом обменнике — свой обменный курс: курс обмена A на B — это количество единиц валюты B, выдаваемое за 1 единицу валюты A. Также в каждом обменнике есть комиссия — сумма, которую вы должны заплатить, чтобы производить операцию. Комиссия взимается в той валюте, которую меняет клиент.

Например, если вы хотите поменять 100 долларов США на российские рубли в обменнике, где курс обмена равен 29.75, а комиссия равна 0.39, вы получите (100 - 0.39) * 29.75 = 2963.3975 рублей.

Вы точно знаете, что в городе используется всего N валют. Пронумеруем их числами 1, 2, ..., N. Тогда каждый обменник представляют 6 чисел: целые A и B — номера обмениваемых валют, а также вещественные RAB, CAB, RBA и CBA — обменные курсы и комиссии при переводе из A в B и из B в A соответственно.

Рик обладает некоторой суммой в валюте S. Он задаётся вопросом, можно ли, после нескольких операций обмена увеличить свой капитал. Конечно, он хочет, чтобы в конце его деньги вновь были в валюте S. Помогите ему ответить на его вопрос. Рик должен всегда должен иметь неотрицательную сумму денег.

*/

#include <array>
#include <cinttypes>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

/// @brief Первая строка содержит четыре числа: N — количество валют, M — количество обменников, S — валюта, которой располагает Рик, и V — количество единиц этой валюты.
size_t N, M, S;
double V;

using WEIGHT = double;
using VERTEX = int;
using CONNECTIONS = std::vector<std::vector<std::tuple<VERTEX, WEIGHT, WEIGHT>>>;
using VERTICES = std::vector<VERTEX>;
using WEIGHTS = std::vector<WEIGHT>;

WEIGHT bellman(CONNECTIONS &connections) {
    WEIGHTS distance(N, 0);
    distance[S] = V;

    while (true) {
        bool exit = true;
        for (size_t x = 0; x < N; x++) {
            for (auto [y, r, c] : connections[x]) {
                if (distance[x] > 0 && distance[x] > c && (distance[x] - c) * r > distance[y]) {
                    distance[y] = (distance[x] - c) * r;
                    if (distance[S] > V) return distance[S];
                    exit = false;
                }
            }
        }
        if (exit) break;
    }

    return distance[S];
}

void currency(CONNECTIONS &connections) {
    if (bellman(connections) > V)
        std::cout << "YES" << std::endl;
    else
        std::cout << "NO" << std::endl;
}

int main() {
    std::cin >> N >> M >> S >> V;
    S--;

    CONNECTIONS connections(N);

    for (size_t i = 0; i < M; i++) {
        VERTEX x, y;
        WEIGHT rab, cab, rba, cba;
        std::cin >> x >> y >> rab >> cab >> rba >> cba;
        x--;
        y--;
        connections[x].push_back({y, rab, cab});
        connections[y].push_back({x, rba, cba});
    }

    currency(connections);

    return 0;
}