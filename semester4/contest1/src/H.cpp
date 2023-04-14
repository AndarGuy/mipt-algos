/*

На курсе алгоритмов в ФПМИ есть модуль, посвященный строкам. Преподаватель решил не ограничиться стандартными задачами и
предложил своим студентам следующую задачу. Есть текст T, в котором ищут паттерн (тоже строку) P. Казалось бы,
классическая задача, но семинарист решил ее усложнить, а именно, надо искать не только паттерн, но и все его циклические
сдвиги. Нужно найти количество вхождений (возможно пересекающихся) всех циклических сдвигов паттерна в тексте. Отметим,
что если паттерн «abс», то его циклическими сдвигами называют следующие строки: «abс», «bca», «cab».

*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

class HashedString {
    static const uint64_t kPrime = 239017;

private:
    std::string string_;
    std::vector<uint64_t> hashes_;
    std::vector<uint64_t> powers_;

public:
    explicit HashedString(std::string string) : string_(string) {
        std::size_t n_constant = string.size();
        hashes_.resize(n_constant + 1);
        powers_.resize(n_constant + 1);
        hashes_[0] = 0, powers_[0] = 1;
        for (std::size_t i = 0; i < n_constant; i++) {
            hashes_[i + 1] = hashes_[i] * kPrime + string_[i];
            powers_[i + 1] = powers_[i] * kPrime;
        }
    }

    uint64_t Hash(std::size_t left, std::size_t right) {
        return hashes_[right + 1] - hashes_[left] * powers_[right - left + 1];
    };

    std::string String() {
        return string_;
    }
};

void Solution([[maybe_unused]] std::string &text, [[maybe_unused]] std::string &pattern) {
    HashedString hashed_text(text);
    std::size_t pattern_length = pattern.size();
    std::string doubled_pattern = pattern + pattern;
    HashedString hashed_pattern(doubled_pattern);

    std::vector<uint64_t> shifts;
    for (size_t i = 0; i < pattern_length; i++) {
        size_t j = i + pattern_length - 1;
        shifts.push_back(hashed_pattern.Hash(i, j));
    }
    std::sort(shifts.begin(), shifts.end());
    std::size_t result = 0;
    for (size_t i = 0; i < text.length() - pattern_length + 1; i++) {
        uint64_t hash = hashed_text.Hash(i, i + pattern_length - 1);
        result = result + std::binary_search(shifts.begin(), shifts.end(), hash);
    }
    std::cout << result << std::endl;
}

int main() {
    std::string pattern;
    std::cin >> pattern;
    std::string text;
    std::cin >> text;

    if (pattern.size() > text.size()) {
        std::cout << 0 << std::endl;
    } else {
        Solution(text, pattern);
    }

    return 0;
}