/*

Дана строка длины N и отсортированный массив суффиксов этой строки (т.е. суффиксный массив), вам нужно вычислить LCP.
При сортировке строка a считается меньше строки aa. LCP — наибольший общий префикс двух последовательных суффиксов в
суффиксном массиве.

*/

#include <string>
#include <vector>
#include <iostream>
#include <map>

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

void Solution([[maybe_unused]] const size_t n_constant, [[maybe_unused]] const std::string& word,
              [[maybe_unused]] const std::vector<size_t>& suffix_array) {

    for (auto index : BuildLPC(word, suffix_array)) {
        std::cout << index << " ";
    }

    std::cout << std::endl;
}

int main() {
    size_t n_constant = 0;
    std::cin >> n_constant;
    std::string word;
    word.resize(n_constant);
    for (size_t i = 0; i < n_constant; i++) {
        std::cin >> word[i];
    }
    std::vector<size_t> suffix_array(n_constant);
    for (size_t i = 0; i < n_constant; i++) {
        std::cin >> suffix_array[i];
        suffix_array[i]--;
    }

    Solution(n_constant, word, suffix_array);
    return 0;
}