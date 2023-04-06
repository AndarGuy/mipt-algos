/*

"Шрек, помнишь, как ты говорил, что великаны многослойные? Должен тебе признаться в том,что ослы устроены проще. Наши
страхи написаны у нас на лбу."

*/

#include <iostream>
#include <string>
#include <vector>

void A(std::string string_a, std::string string_b) {
    const ssize_t hash_constant = 31ULL;

    if (string_a.length() < string_b.length()) {
        return;
    }

    std::vector<ssize_t> powers(string_b.length() + 1);
    powers[0] = 1;
    for (size_t i = 1; i < string_b.length() + 1; i++) {
        powers[i] = powers[i - 1] * hash_constant;
    }

    std::vector<ssize_t> hash_storage(string_a.length() + 1);
    hash_storage[string_a.length()] = 0;
    for (size_t i = 0; i < string_a.length(); i++) {
        hash_storage[string_a.length() - i - 1] =
            hash_storage[string_a.length() - i] * hash_constant + string_a[string_a.length() - i - 1];
    }

    // std::cout << hash_storage[0] << "\n";

    ssize_t target_hash = 0;
    for (size_t i = 0; i < string_b.length(); i++) {
        target_hash = target_hash + string_b[i] * powers[i];
    }

    // std::cout << target_hash << "\n";

    for (size_t i = 0; i < string_a.length() - string_b.length() + 1; i++) {
        ssize_t hash = hash_storage[i] - hash_storage[i + string_b.length()] * powers[string_b.length()];
        // std::cout << hash << " " << target_hash << " " << hash_storage[i] << " " << hash_storage[i +
        // string_b.length() + 1] << std::endl;
        if (hash == target_hash) {
            std::cout << i << std::endl;
        }
    }
}

int main() {
    std::string string_a;
    std::string string_b;
    std::cin >> string_a >> string_b;

    A(string_a, string_b);

    return 0;
}