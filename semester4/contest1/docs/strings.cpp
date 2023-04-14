#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

template <typename T>
std::vector<std::size_t> BuildZFunctionTrivially(const T &str) {
    std::vector<std::size_t> z_function(str.length());
    for (size_t i = 0UL; i < str.length(); i++) {
        while (i + z_function[i] < str.length() && str[z_function[i]] == str[i + z_function[i]]) {
            z_function[i]++;
        }
    }
    return z_function;
}

template <typename T>
std::vector<std::size_t> BuildZFunction(const T &str) {
    std::vector<std::size_t> z_function(str.length());
    std::size_t left = 0;
    std::size_t right = 0;
    for (size_t i = 1UL; i < str.length(); i++) {
        if (i <= right) {
            z_function[i] = std::min(z_function[i - left], right - i + 1);
        }
        while (i + z_function[i] < str.length() && str[z_function[i]] == str[i + z_function[i]]) {
            z_function[i]++;
        }
        if (i + z_function[i] - 1 > right) {
            left = i;
            right = i + z_function[i] - 1;
        }
    }
    return z_function;
}

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
