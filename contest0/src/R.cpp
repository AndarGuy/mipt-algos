/*

k-м циклическим сдвигом строки S называется строка, полученная перестановкой k первых символов строки S в конец строки.
Рассмотрим все различные циклические сдвиги строки S и отсортируем их по возрастанию. Требуется вычислить i-ю строчку
этого массива. Например, для строки abacabac существует четыре различных циклических сдвига: нулевой (abacabac), первый
(bacabaca), второй (acabacab) и третий (cabacaba). После сортировки по возрастанию получится такой массив: abacabac,
acabacab, bacabaca, cabacaba.

*/

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <stack>
#include <algorithm>

template <typename T>
std::vector<size_t> SortCyclicShifts(T const& string) {
    int n_constant = string.size();
    const int alphabet = 128;
    std::vector<size_t> permutations(n_constant);
    std::vector<int> classes(n_constant);
    std::vector<int> count_sort(std::max(alphabet, n_constant), 0);
    for (int i = 0; i < n_constant; i++) {
        count_sort[string[i]]++;
    }
    for (int i = 1; i < alphabet; i++) {
        count_sort[i] += count_sort[i - 1];
    }
    for (int i = 0; i < n_constant; i++) {
        permutations[--count_sort[string[i]]] = i;
    }
    classes[permutations[0]] = 0;
    int c = 1;
    for (int i = 1; i < n_constant; i++) {
        if (string[permutations[i]] != string[permutations[i - 1]]) {
            c++;
        }
        classes[permutations[i]] = c - 1;
    }
    std::vector<int> pn(n_constant);
    std::vector<int> cn(n_constant);
    for (int h = 0; (1 << h) < n_constant; ++h) {
        for (int i = 0; i < n_constant; i++) {
            pn[i] = static_cast<int>(permutations[i]) - (1 << h);
            if (pn[i] < 0) {
                pn[i] += n_constant;
            }
        }
        std::fill(count_sort.begin(), count_sort.begin() + static_cast<int>(c), 0);
        for (int i = 0; i < n_constant; i++) {
            count_sort[classes[pn[i]]]++;
        }
        for (int i = 1; i < c; i++) {
            count_sort[i] += count_sort[i - 1];
        }
        for (int i = n_constant; i > 0; i--) {
            permutations[--count_sort[classes[pn[i - 1]]]] = pn[i - 1];
        }
        cn[permutations[0]] = 0;
        c = 1;
        for (int i = 1; i < n_constant; i++) {
            std::pair<int, int> cur = {classes[permutations[i]], classes[(permutations[i] + (1 << h)) % n_constant]};
            std::pair<int, int> prev = {classes[permutations[i - 1]],
                                        classes[(permutations[i - 1] + (1 << h)) % n_constant]};
            if (cur != prev) {
                ++c;
            }
            cn[permutations[i]] = c - 1;
        }
        classes.swap(cn);
    }
    return {permutations.begin() + 1, permutations.end()};
}

template <typename T>
std::vector<size_t> BuildSuffixArray(const T& string) {
    T copied = string;
    copied.push_back('\0');
    std::vector<size_t> sorted_shifts = SortCyclicShifts(copied);
    return sorted_shifts;
}

template <typename T>
std::vector<size_t> BuildLPC(T const& string, std::vector<size_t> const& suffix_array) {
    size_t n = string.size();
    std::vector<size_t> inversed(n);
    for (size_t i = 0; i < n; i++) {
        inversed[suffix_array[i]] = i;
    }

    size_t k = 0;
    std::vector<size_t> lcp(n - 1);
    for (size_t i = 0; i < n; i++) {
        if (inversed[i] != n - 1) {
            size_t j = suffix_array[inversed[i] + 1];
            while (i + k < n && j + k < n && string[i + k] == string[j + k]) {
                k++;
            }
            lcp[inversed[i]] = k;
            k = k - (k != 0);
        } else {
            k = 0;
        }
    }
    return lcp;
}

void Solution([[maybe_unused]] const size_t constant_k, const std::string& string) {
    std::string new_string = string + string;
    std::vector<size_t> suffix_array = BuildSuffixArray(new_string);
    // for (auto i : suffix_array) {
    //     std::cout << i << " ";
    // }
    // std::cout << std::endl;
    std::vector<size_t> lcp_array = BuildLPC(new_string, suffix_array);
    // for (auto i : lcp_array) {
    //     std::cout << i << " ";
    // }
    // std::cout << std::endl;

    size_t starting_point = 0;
    size_t counter = 0;
    size_t common_prefix = 0;
    for (size_t i = 0; i < new_string.size() && counter != constant_k; i++) {
        if (i != 0) {
            common_prefix = std::min(common_prefix, lcp_array[i - 1]);
        }

        if (suffix_array[i] < string.size()) {
            if (i == 0 || common_prefix < string.size()) {
                counter++;
                common_prefix = UINT64_MAX;
            }
        }

        starting_point = suffix_array[i] + 1;
    }

    if (counter != constant_k) {
        std::cout << "IMPOSSIBLE" << std::endl;
    } else {
        for (size_t i = 0; i < string.size(); i++) {
            std::cout << new_string[starting_point - 1 + i];
        }
        std::cout << std::endl;
    }
}

int main() {
    std::string string;
    std::cin >> string;
    size_t constant_k = 0;
    std::cin >> constant_k;
    Solution(constant_k, string);
}