/*

Осел и молодая чета огров едут в тридевятое царство. Хотя впереди еще половина пути, Осел уже изрядно всех достал
поэтому Фиона предлагает ему игру: она загадывает Ослу слово, а тот пытается придумать из него другие слова. Фиона
считает слово T неинтересным, если его можно составить из префиксов загаданного слова S. Шрек в бешенстве, что у Осла
никак не получается придумать интересное Фионе слово и продолжить поездку в тишине и просит вас помочь своему другу.

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

void BuildPartition(const std::string &example, const std::string &generated,
                    const std::vector<std::size_t> &prefix_function) {
    std::vector<std::size_t> dictionary;
    dictionary.push_back(0);
    for (size_t i = example.length() + generated.length(); i > example.length(); i--) {
        if (prefix_function[i] <= prefix_function[i - 1]) {
            i = i - prefix_function[i] + 1;
            dictionary.push_back(i - example.length() - 1);
        }
    }
    dictionary.push_back(generated.length());
    std::sort(dictionary.begin(), dictionary.end());
    for (size_t i = 0; i < dictionary.size() - 1; i++) {
        for (size_t j = dictionary[i]; j < dictionary[i + 1]; j++) {
            std::cout << generated[j];
        }
        std::cout << " ";
    }

    std::cout << std::endl;
}

void Solution(const std::string &example, const std::string &generated) {
    auto prefix_function = BuildPrefixFunction(example + "#" + generated);
    bool interesting = false;
    for (size_t i = 0; i < generated.length() && !interesting; i++) {
        std::size_t index = example.length() + i + 1;
        if (prefix_function[index] == 0) {
            interesting = true;
        }
    }

    if (interesting) {
        std::cout << "Yes" << std::endl;
    } else {
        std::cout << "No" << std::endl;
        BuildPartition(example, generated, prefix_function);
    }
}

int main() {
    std::string example;
    std::string generated;
    std::cin >> example >> generated;

    Solution(example, generated);

    return 0;
}