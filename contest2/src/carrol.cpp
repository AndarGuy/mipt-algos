/*

Из мухи — слона

В известной игре «цепочка слов» необходимо получить из одного слова другое, меняя в нём по одной букве. Например, получить из мухи слона можно за 8 шагов: МУХА –МУЛА –КУЛА –КИЛА –КИЛТ –КИОТ –КЛОТ –КЛОН –СЛОН. Все использованные слова должны принадлежать словарю. Игра изобретена Льюисом Кэрроллом. Будем называть минимальное количество шагов, необходимое для получения из одного слова другого «расстоянием по Кэрроллу».

По заданному словарю и набору пар слов определите расстояние по Кэрроллу для каждой заданной пары слов.

*/

#include <array>
#include <cinttypes>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

/// @brief Первая строка входных данных содержит количество слов в словаре N.
size_t N, M;

using WEIGHT [[maybe_unused]] = int;
using VERTEX [[maybe_unused]] = int;
using VERTICES [[maybe_unused]] = std::vector<VERTEX>;
using WEIGHTS [[maybe_unused]] = std::vector<WEIGHT>;

WEIGHT carrol(int start, int end, std::vector<std::string> &words, std::vector<std::vector<int>> &dictionary) {
    std::vector<int> start_distance(N, INT32_MAX), end_distance(N, INT32_MAX), start_visited(N, false), end_visited(N, false);
    std::queue<int> start_queue, end_queue;
    start_queue.push(start);
    end_queue.push(end);
    start_distance[start] = end_distance[end] = 0;

    while (!start_queue.empty() && !end_queue.empty()) {
        int x_start = start_queue.front(), x_end = end_queue.front();
        if (start_distance[x_start] > end_distance[x_end]) {
            std::swap(x_start, x_end);
            std::swap(start_queue, end_queue);
            std::swap(start_distance, end_distance);
            std::swap(start_visited, end_visited);
        }

        start_queue.pop();
        start_visited[x_start] = true;
        if (end_visited[x_start])
            return end_visited[x_end] + end_visited[x_end];

        for (int y_start : dictionary[x_start]) {
            if (!start_visited[y_start] && start_distance[y_start] > start_distance[x_start] + 1) {
                start_distance[y_start] = start_distance[x_start] + 1;
                start_queue.push(y_start);
            }
            if (end_visited[y_start]) return start_distance[y_start] + end_distance[y_start];
        }
    }

    return -1;
}

int switchable(std::string &a, std::string &b) {
    int counter = 0;
    for (size_t i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) counter++;
        if (counter > 1) return false;
    }
    return counter == 1;
}

int main() {
    std::cin >> N;

    std::vector<std::string> words(N);
    std::vector<std::vector<int>> dictionary(N);

    for (size_t i = 0; i < N; i++)
        std::cin >> words[i];

    std::sort(words.begin(), words.end());

    for (size_t i = 0; i < N; i++) {
        for (size_t j = i + 1; j < N; j++) {
            if (!switchable(words[i], words[j])) continue;
            dictionary[i].push_back(j);
            dictionary[j].push_back(i);
        }
    }

    std::cin >> M;

    std::string string_a, string_b;
    for (size_t i = 0; i < M; i++) {
        std::cin >> string_a >> string_b;
        auto start = std::lower_bound(words.begin(), words.end(), string_a) - words.begin();
        auto end = std::lower_bound(words.begin(), words.end(), string_b) - words.begin();
        int answer = carrol(start, end, words, dictionary);
        std::cout << answer << std::endl;
    }

    return 0;
}