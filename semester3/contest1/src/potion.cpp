/*

Зелье

Злой маг Крокобобр варит зелье. У него есть большая колба, которую можно ставить на огонь и две колбы поменьше, которые огня не выдержат.
В большой колбе до самого верха налиты слёзы дракона, которые нужно подогреть на огне и затем смешать с другими компонентами. Маленькие колбы пусты.
Ёмкости всех колбы магу Крокобобру известны — N лигул в большой колбе, в маленьких — по M и K лигул.
Жидкости можно переливать из любой колбы в любую, если выполняется одно из условий: после переливания либо одна из колб становится пустой,
либо одна из колб становится полной, частичные переливания недопустимы.
Слёзы дракона достались Крокобобру очень дорого, поэтому их нельзя выливать в другие сосуды.

По рецепту в большой колбе нужно подогреть ровно L лигул слёз дракона. Помогите Крокобобру определить, сколько переливаний он должен сделать для этого.

*/

#include <cinttypes>
#include <iostream>
#include <map>
#include <vector>

#define MAXN 2000
#define MAXM 2000
#define MAXK 2000
#define MAXL 2000

std::vector<std::tuple<short, short, short>> get_turns(short x, short y, short z, short N, short M, short K) {
    std::vector<std::tuple<short, short, short>> result;
    if (x > 0) {
        if (y + x <= M) result.push_back({0, y + x, z});
        if (z + x <= K) result.push_back({0, y, z + x});
    }
    if (x < N) {
        if (y >= N - x) result.push_back({N, y - (N - x), z});
        if (z >= N - x) result.push_back({N, y, z - (N - x)});
    }
    if (y > 0) {
        if (y + x <= N) result.push_back({x + y, 0, z});
        if (y + z <= K) result.push_back({x, 0, z + y});
    }
    if (y < M) {
        if (x >= M - y) result.push_back({x - (M - y), M, z});
        if (z >= M - y) result.push_back({x, M, z - (M - y)});
    }
    if (z < K) {
        if (x >= K - z) result.push_back({x - (K - z), y, K});
        if (y >= K - z) result.push_back({x, y - (K - z), K});
    }
    if (z > 0) {
        if (z + x <= N) result.push_back({x + z, y, 0});
        if (z + y <= M) result.push_back({x, y + z, 0});
    }
    return result;
}

void dfs(short x, short y, short z, int step, short N, short M, short K, short L, std::map<std::tuple<short, short, short>, int>& visited, int* min_distance) {
    // std::cout << x << ' ' << y << ' ' << z << ' ' << visited[{x, y, z}] << std::endl;
    visited[{x, y, z}] = step;
    if (x == L) {
        *min_distance = std::min(*min_distance, step);
        return;
    }
    for (auto turn : get_turns(x, y, z, N, M, K)) {
        if (!visited[turn] || visited[turn] > step + 1) dfs(std::get<0>(turn), std::get<1>(turn), std::get<2>(turn), step + 1, N, M, K, L, visited, min_distance);
    }
}

void potions(short N, short M, short K, short L) {
    std::map<std::tuple<short, short, short>, int> visited;
    int min_distance = INT32_MAX;
    dfs(N, 0, 0, 1, N, M, K, L, visited, &min_distance);
    if (min_distance != INT32_MAX)
        std::cout << min_distance - 1 << std::endl;
    else
        std::cout << "OOPS" << std::endl;
}

int main() {
    short N, M, K, L;
    std::cin >> N >> M >> K >> L;

    potions(N, M, K, L);

    return 0;
}