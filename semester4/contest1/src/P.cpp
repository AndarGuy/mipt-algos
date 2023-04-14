/*

Рассмотрим последовательность n целых чисел от 1 до m. Подпоследовательность подряд идущих чисел называется рефреном,
если произведение её длины на количество вхождений в последовательность максимально. По заданной последовательности
требуется найти её рефрен.

*/

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <stack>

template <typename T>
std::vector<size_t> SortCyclicShifts(T const& string) {
    size_t n_constant = string.size();
    size_t counter = 0;
    size_t n_classes = 0;
    std::vector<size_t> classes(n_constant);
    std::vector<size_t> permutations(n_constant);

    std::map<size_t, std::vector<size_t>> hashmap;
    for (size_t i = 0; i < n_constant; i++) {
        hashmap[string[i]].push_back(i);
    }

    for (auto& item : hashmap) {
        for (auto value : item.second) {
            classes[value] = n_classes, permutations[counter++] = value;
        }
        n_classes++;
    }

    for (size_t i = 1; n_classes < n_constant; i++) {
        std::vector<std::vector<size_t>> count_sort(n_classes);
        std::vector<size_t> temp_classes(n_constant);
        size_t d = (1 << i) / 2;
        size_t temp_n_classes = counter = 0;

        for (size_t j = 0; j < n_constant; j++) {
            size_t k = (permutations[j] - d + n_constant) % n_constant;
            count_sort[classes[k]].push_back(k);
        }

        for (size_t j = 0; j < n_classes; j++) {
            for (size_t k = 0; k < count_sort[j].size(); k++) {
                if (k == 0 ||
                    classes[(count_sort[j][k] + d) % n_constant] != classes[(count_sort[j][k - 1] + d) % n_constant]) {
                    temp_n_classes++;
                }
                temp_classes[count_sort[j][k]] = temp_n_classes - 1;
                permutations[counter++] = count_sort[j][k];
            }
        }

        classes = temp_classes;
        n_classes = temp_n_classes;
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

void Solution([[maybe_unused]] const size_t constant_n, [[maybe_unused]] const size_t constant_m,
              [[maybe_unused]] const std::vector<size_t>& array) {
    auto suffix_array = BuildSuffixArray(array);
    auto lcp_array = BuildLPC(array, suffix_array);
    lcp_array.push_back(0);
    std::stack<std::tuple<size_t, size_t, size_t>> stack;
    size_t max_size = constant_n;
    size_t max_repetition = 1;
    size_t start = 0;

    for (size_t i = 0; i < constant_n; i++) {
        size_t temp = 1;
        size_t largest_prefix = lcp_array[i];
        while (!stack.empty() && largest_prefix < std::get<0>(stack.top()) + 1) {
            auto [size, repetition, j] = stack.top();
            stack.pop();
            temp += repetition;
            if (temp * size > max_repetition * max_size) {
                max_repetition = temp;
                max_size = size;
                start = suffix_array[j];
            }
        }
        if (stack.empty() || largest_prefix > std::get<0>(stack.top())) {
            stack.emplace(largest_prefix, temp, i);
        }
    }

    std::cout << max_size * max_repetition << std::endl;
    std::cout << max_size << std::endl;
    for (size_t i = 0; i < max_size; i++) {
        std::cout << array[start + i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    size_t constant_n = 0;
    size_t constant_m = 0;
    std::cin >> constant_n >> constant_m;
    std::vector<size_t> array(constant_n);
    for (size_t i = 0; i < constant_n; i++) {
        std::cin >> array[i];
    }

    Solution(constant_n, constant_m, array);
    return 0;
}