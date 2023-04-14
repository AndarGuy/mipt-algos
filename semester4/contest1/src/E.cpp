/*

— Шрек, ты чего?
— Не могу поверить, что я скоро стану отцом. Как это получилось?

Шрек и Фиона не могут неделями успокоить своих шебутных детей. Компьютерные игры еще не придумали, а бабушка с дедушкой
вечно заняты, поэтому нашим героям приходится раздавать своих детей знакомым на выходные, чтобы хотя бы немного
отдохнуть от суеты. Больше всего маленькие огры любят проводить время с Волшебным Зеркалом. Зеркало примечательно тем,
что люди, огры и другие обитатели 3-ки в нем не отражаются. Дети расставляют кубики в ряд и ползают вдоль него, таким
образом, что всегда видят перед собой N цветных кубиков, но не знают, какие из этих кубиков настоящие, а какие - всего
лишь отражение в зеркале. Be patient - дети огров, выросшие на болоте, не самые сообразительные в мире дети. Помогите
им! Выясните, сколько кубиков может быть в комнате с Волшебным Зеркалом. Дети видят отражение всех кубиков в зеркале и
часть кубиков, которая находится перед ними.

*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

/// @brief Z Function was modified to count max mirrored string on index i;
/// @param str String to search mirror Z Function on.
/// @return Modified Z Function.
template <typename T>
std::vector<std::size_t> BuildZFunctionModified(const T &str) {
    std::vector<std::size_t> z_function(str.size());
    std::size_t left = 0;
    std::size_t right = 0;
    for (size_t i = 0UL; i < str.size() / 2 + 1; i++) {
        if (i < right) {
            z_function[i] = std::min(z_function[(right - left) - (i - left) - 1], right - i);
        }
        while (i - z_function[i] + 1 > 0 && i + z_function[i] + 1 < str.size() &&
               str[i - z_function[i]] == str[i + z_function[i] + 1]) {
            z_function[i]++;
        }
        if (i + z_function[i] > right) {
            left = i;
            right = i + z_function[i];
        }
    }
    return z_function;
}

void Solution(const std::size_t constant_n, const std::vector<std::size_t> &blocks) {
    auto z_function = BuildZFunctionModified(blocks);
    for (size_t i = 0; i < constant_n; i++) {
        if (z_function[constant_n - i - 1] == constant_n - i) {
            std::cout << i << " ";
        }
    }
    std::cout << constant_n << std::endl;
}

int main() {
    std::size_t constant_n = 0;
    std::size_t constant_m = 0;
    std::cin >> constant_n >> constant_m;
    std::vector<std::size_t> blocks;
    blocks.resize(constant_n);
    for (size_t i = 0; i < constant_n; i++) {
        std::cin >> blocks[i];
    }

    Solution(constant_n, blocks);

    return 0;
}