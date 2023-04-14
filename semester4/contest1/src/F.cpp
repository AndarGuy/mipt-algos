/*

Дана Z-функция некоторой строки. Найти лексикографически минимальную строку, массив Z-функции которой совпадет с данным.
Алфавит для построения ответа: a-z.

*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

std::string RestoreStringByZFunction(const std::vector<size_t> &z_function) {
    std::size_t n = z_function.size();
    std::string string = "a";
    std::vector<std::size_t> used_indices;
    for (std::size_t i = 1UL, z_end = 0UL, z_index = 0UL; i < n; i++) {
        if (z_function[i] == 0 && i > z_end) {
            char c = 'b';
            for (c = 'b'; c < 'z' + 1; c++) {
                bool can_be_used = true;
                for (std::size_t index : used_indices) {
                    if (string[index] == c) {
                        can_be_used = false;
                        break;
                    }
                }
                if (can_be_used) {
                    break;
                }
            }
            string.push_back(c);
            used_indices.clear();
            z_index = 0;
            continue;
        }
        if (i + z_function[i] - 1 > z_end) {
            used_indices.clear();
            z_end = i + z_function[i] - 1;
            z_index = 0;
        }
        if (i + z_function[i] - 1 == z_end) {
            used_indices.push_back(z_function[i]);
        }
        string.push_back(string[z_index]);
        z_index++;
    }
    return string;
}

void Solution([[maybe_unused]] const std::size_t constant_n, const std::vector<std::size_t> &z_function) {
    std::cout << RestoreStringByZFunction(z_function) << std::endl;
}

int main() {
    std::size_t constant_n = 0;
    std::cin >> constant_n;
    std::vector<std::size_t> z_function;
    z_function.resize(constant_n);
    for (size_t i = 0; i < constant_n; i++) {
        std::cin >> z_function[i];
    }

    if (constant_n > 0) {
        Solution(constant_n, z_function);
    }

    return 0;
}