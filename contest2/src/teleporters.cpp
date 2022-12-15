/*

Телепортеры

В галактике Кин-дза-дза основной способ перемещаться между планетами — использовать однонаправленные телепортеры. Дядя Вова и Гедеван обнаружили табличку, на которую нанесены сведения, существует ли телепортер на планете i, отправляющий на планету j.

Так как расшифровки соответствия номеров планет их названиям они ещё не нашли, а табличку нужно срочно отдавать, они хотят составить другую карту—достижимости одной планеты с другой с помощью телепортеров. Помогите им её составить. Только поспешите, времени на это не очень много!

*/

#include <array>
#include <cinttypes>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

size_t N;

using VERTEX = short;
using CONNECTIONS = std::vector<std::vector<VERTEX>>;
using VERTICES = std::vector<VERTEX>;

void dfs(size_t x, CONNECTIONS &connections, VERTICES &visited) {
    for (auto y : connections[x]) {
        if (!visited[y]) {
            visited[y] = true;
            dfs(y, connections, visited);
        }
    }
}

void teleporters(CONNECTIONS &connections) {
    CONNECTIONS map(N, std::vector<short>(N, false));
    for (size_t i = 0; i < N; i++) {
        VERTICES visited(N, false);
        dfs(i, connections, visited);
        for (size_t j = 0; j < N; j++)
            map[i][j] = visited[j];
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            std::cout << map[i][j];
        }
        std::cout << std::endl;
    }
}

int main() {
    std::cin >> N;

    CONNECTIONS connections(N);

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            char connected;
            std::cin >> connected;
            if (connected == '1') connections[i].push_back(j);
        }
    }

    teleporters(connections);

    return 0;
}