/*

Дана префикс-функция некоторой строки. Найти лексикографически минимальную строку, массив префикс-функции которой
совпадет с данным. Алфавит для построения ответа: a-z.

*/

#include <iostream>
#include <string>
#include <vector>

std::string RestoreStringByPrefixFunction(std::size_t constant_n, std::vector<std::size_t> &pi_function) {
    if (constant_n == 0) {
        return std::string{};
    }
    std::string string = "a";
    for (size_t i = 1; i < constant_n; i++) {
        if (pi_function[i] != 0) {
            string.push_back(string[pi_function[i] - 1]);
        } else {
            std::vector<bool> used(26, false);
            std::size_t next = pi_function[i - 1];
            while (next) {
                used[string[next] - 'a'] = true;
                next = pi_function[next - 1];
            }
            char c = 'b';
            for (; used[c - 'a']; c++) {
                // Nothing
            }
            string.push_back(c);
        }
    }

    return string;
}

int main() {
    std::size_t constant_n = 0;
    std::cin >> constant_n;
    std::vector<std::size_t> pi_function(constant_n);
    for (size_t i = 0; i < constant_n; i++) {
        std::cin >> pi_function[i];
    }

    std::cout << RestoreStringByPrefixFunction(constant_n, pi_function) << std::endl;

    return 0;
}