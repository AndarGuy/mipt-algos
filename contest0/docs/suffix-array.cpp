#include <string>
#include <vector>

template <typename T>
std::vector<size_t> SortCyclicShifts(T const& string) {
    size_t constant_n = string.size();
    const size_t alphabet = 256;

    std::vector<size_t> permutations(constant_n);
    std::vector<size_t> classes(constant_n);
    std::vector<size_t> counters(std::max(alphabet, constant_n), 0);

    for (size_t i = 0; i < constant_n; i++) {
        counters[string[i]]++;
    }

    for (size_t i = 1; i < alphabet; i++) {
        counters[i] = counters[i] + counters[i - 1];
    }

    for (size_t i = 0; i < constant_n; i++) {
        permutations[--counters[string[i]]] = i;
    }

    classes[permutations[0]] = 0;
    size_t c = 1;
    for (size_t i = 1; i < constant_n; i++) {
        c = c + (string[permutations[i]] != string[permutations[i - 1]]);
        classes[permutations[i]] = c - 1;
    }

    std::vector<std::size_t> temp_permutations(constant_n);
    std::vector<std::size_t> temp_classes(constant_n);
    for (size_t h = 0; (1 << h) < constant_n; ++h) {
        for (size_t i = 0; i < constant_n; i++) {
            temp_permutations[i] = permutations[i] - (1 << h);
            if (temp_permutations[i] < 0) {
                temp_permutations[i] += constant_n;
            }
        }

        fill(counters.begin(), counters.begin() + static_cast<int>(c), 0);

        for (size_t i = 0; i < constant_n; i++) {
            counters[classes[temp_permutations[i]]]++;
        }
        for (size_t i = 1; i < c; i++) {
            counters[i] += counters[i - 1];
        }
        for (size_t i = constant_n - 1; i >= 0; i--) {
            permutations[--counters[classes[temp_permutations[i]]]] = temp_permutations[i];
        }
        temp_classes[permutations[0]] = 0;
        c = 1;
        for (size_t i = 1; i < constant_n; i++) {
            std::pair<size_t, size_t> current = {classes[permutations[i]],
                                                 classes[(permutations[i] + (1 << h)) % constant_n]};
            std::pair<size_t, size_t> previous = {classes[permutations[i - 1]],
                                                  classes[(permutations[i - 1] + (1 << h)) % constant_n]};
            c = c + (current != previous);
            temp_classes[permutations[i]] = c - 1;
        }
        classes.swap(temp_classes);
    }
    return permutations;
}

template <typename T>
std::vector<std::size_t> BuildSuffixArray(T& string) {
    T copied = string;
    copied.push_back('\0');
    std::vector<std::size_t> sorted_shifts = SortCyclicShifts(string);
    sorted_shifts.erase(sorted_shifts.begin());
    return sorted_shifts;
}
