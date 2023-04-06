/*

Дана строка, требуется построить суффиксный массив для этой строки. Суффиксный массив —– лексикографически
отсортированный массив всех суффиксов строки. Каждый суффикс задается целым числом — позицией начала. Строка s
лексикографически меньше строки t, если есть такое i, что s_i < t_i и s_j = t_j для всех j < i, или если такого i не
существует и строка s короче строки t. Здесь s_i — код i-го символа строки s. Формат входных данных Файл состоит из
единственной строки. Эта строка — английский литературный текст. Длина текста не превосходит 10^5 . Коды всех символов в
тексте от 32 до 127. Формат выходных данных Выведите чисел — суффиксный массив данной строки.

*/

#include <string>
#include <vector>
#include <iostream>
#include <map>

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
    std::string copied = string;
    copied.push_back('\0');
    std::vector<size_t> sorted_shifts = SortCyclicShifts(copied);
    return sorted_shifts;
}

void Solution([[maybe_unused]] std::string& text) {

    for (size_t index : BuildSuffixArray(text)) {
        std::cout << index + 1 << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::string text;
    std::getline(std::cin, text);
    Solution(text);
    return 0;
}