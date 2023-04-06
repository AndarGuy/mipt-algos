/*

Палиндром — это строка, которая одинаково читается как слева направо, так и справа налево.
Дан набор из N строк. Найдите среди них такие пары, которые при конкатенации дают палиндром.
Более формально, найдите все пары (i, j) такие, что i ≠ j и строка si + sj является палиндромом.
Выведите все упорядоченные пары индексов (нумерация с единицы).

*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <cstring>

class Hashed {
public:
    static const uint64_t kPrime = 239017ULL;
    // 1, P, P^2, P^3, ...
    static uint64_t powers[10 + 1];

private:
    // 0, s[0], s[0] * P + s[1], s[0] * P^2 + s[1] * P + s[2], ...
    uint64_t hashes_[10 + 1];
    uint64_t reversed_hashes_[10 + 1];

public:
    Hashed() = default;
    explicit Hashed(char string[10 + 1], size_t length) {
        reversed_hashes_[0] = string[0], hashes_[0] = 0;
        for (std::size_t i = 0; i < length; i++) {
            hashes_[i + 1] = hashes_[i] * kPrime + string[i];
            reversed_hashes_[i + 1] = reversed_hashes_[i] + string[i + 1] * powers[i + 1];
        }
    }

    static uint64_t Power(std::size_t of) {
        return powers[of];
    }

    uint64_t Hash(std::size_t left, std::size_t right) const {
        return hashes_[right + 1] - hashes_[left] * powers[right - left + 1];
    };

    uint64_t ReversedHash(std::size_t left, std::size_t right) const {
        if (left == 0) {
            return reversed_hashes_[right];
        }
        return reversed_hashes_[right] - reversed_hashes_[left - 1];
    }
};

bool IsPolindrome(size_t i, size_t j, const Hashed &hash) {
    size_t polindrome_size = j - i;
    if (polindrome_size == 0 || polindrome_size == 1) {
        return true;
    }
    size_t mid = polindrome_size / 2;
    return hash.Hash(i, i + mid - 1 + polindrome_size % 2) * hash.Power(i + mid) == hash.ReversedHash(i + mid, j - 1);
}

void Solution([[maybe_unused]] const std::size_t n_constant, [[maybe_unused]] const std::vector<size_t> &lengths,
              [[maybe_unused]] const std::vector<Hashed> &hashes) {

    std::unordered_map<uint64_t, std::size_t> hashmap;

    for (size_t i = 0; i < n_constant; i++) {
        hashmap[hashes[i].ReversedHash(0, lengths[i] - 1)] = i;
    }

    std::vector<std::pair<size_t, size_t>> answer;

    for (size_t i = 0; i < n_constant; i++) {
        const size_t string_size = lengths[i];
        for (size_t j = 0; j < string_size + 1; j++) {
            if (IsPolindrome(0, j, hashes[i])) {
                uint64_t hash = hashes[i].Hash(j, string_size - 1);
                if (auto search = hashmap.find(hash); search != hashmap.end()) {
                    size_t k = search->second;
                    if (k != i) {
                        answer.emplace_back(k + 1, i + 1);
                    }
                }
            }
            if (IsPolindrome(string_size - j, string_size, hashes[i])) {
                uint64_t hash = hashes[i].Hash(0, string_size - j - 1);
                if (auto search = hashmap.find(hash); search != hashmap.end()) {
                    size_t k = search->second;
                    if (k != i) {
                        answer.emplace_back(i + 1, k + 1);
                    }
                }
            }
        }
    }

    std::set<std::pair<size_t, size_t>> filtered_answer(answer.begin(), answer.end());

    std::cout << filtered_answer.size() << std::endl;

    for (auto [i, j] : filtered_answer) {
        printf("%zu %zu\n", i, j);
    }
}

uint64_t Hashed::powers[10 + 1] = {0};

int main() {
    std::size_t n_constant = 0;
    std::cin >> n_constant;

    Hashed::powers[0] = 1;
    for (size_t i = 0; i < 10; i++) {
        Hashed::powers[i + 1] = Hashed::powers[i] * Hashed::kPrime;
    }

    std::vector<Hashed> hashes;
    std::vector<size_t> lengths(n_constant);
    char string[10 + 1];
    for (size_t i = 0; i < n_constant; i++) {
        scanf("%s", string);
        lengths[i] = strlen(string);
        hashes.emplace_back(string, lengths[i]);
    }

    Solution(n_constant, lengths, hashes);

    return 0;
}