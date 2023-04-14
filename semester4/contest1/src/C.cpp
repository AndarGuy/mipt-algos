/*

Шрек и Кот в сапогах наконец-то нашли общий язык и теперь мучают Осла новыми каламбурами. Схема очень проста и именно
поэтому просто невыносима - n слов склеиваются в одно, по следующему правилу: если два слова идут подряд, то удаляется
самый длинный префикс второго слова, который совпадает с суффиксом первого слова. Например, слова "каламбур" и "буря"
склеиваются в "каламбурю" (грозящую погрести под собой Осла). Каламбур склеивается слева направо (т.е. сначала
склеиваются первые два слова, потом результат с третьим, и так далее). Напишите программу, которая поможет
культивировать это развлечение.

*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

template <typename T>
std::vector<std::size_t> BuildPrefixFunction(const T &str) {
    std::vector<std::size_t> pi_function(str.length());
    for (size_t i = 1UL; i < str.length(); i++) {
        size_t k = pi_function[i - 1];
        while (k > 0 && str[i] != str[k]) {
            k = pi_function[k - 1];
        }
        if (str[i] == str[k]) {
            pi_function[i] = k + 1;
        } else {
            pi_function[i] = k;
        }
    }
    return pi_function;
}

void Solution(std::size_t constant_n, std::vector<std::string> &word_list) {

    std::string concatenated;

    for (size_t i = 0; i < constant_n; i++) {
        std::string &next_word = word_list[i];
        auto pi_function = BuildPrefixFunction(next_word);
        std::size_t repetition_size = 0;
        for (size_t j = std::max(0, static_cast<int>(concatenated.length()) - static_cast<int>(next_word.length()));
             j < concatenated.length(); j++) {
            while (repetition_size > 0 && next_word[repetition_size] != concatenated[j]) {
                repetition_size = pi_function[repetition_size - 1];
            }
            repetition_size += (concatenated[j] == next_word[repetition_size]);
        }
        concatenated.append(next_word.substr(repetition_size));
    }

    std::cout << concatenated << std::endl;
}

int main() {
    std::size_t constant_n = 0;
    std::cin >> constant_n;
    std::vector<std::string> word_list(constant_n);
    for (size_t i = 0; i < constant_n; i++) {
        std::cin >> word_list[i];
    }

    Solution(constant_n, word_list);

    return 0;
}