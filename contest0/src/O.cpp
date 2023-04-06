/*

Дана строка. Для каждого ее префикса найдите количество ее различных подстрок.

*/

#include <string>
#include <vector>
#include <iostream>
#include <map>

struct Chain {
    size_t length = 0;
    int link = -1;
    //
    size_t path = 0;
    //
    std::map<char, int> next;

    Chain() = default;
};

class SuffixAutomaton {
private:
    static const size_t kN = 1000000;

public:
    SuffixAutomaton() : size_(1), last_(0), automaton_(kN) {
        automaton_[0].path = 1;
    }

    size_t Extend(char character) {
        int current = static_cast<int>(size_++);
        automaton_[current].length = automaton_[last_].length + 1;
        int parent = last_;
        while (parent != -1 && !automaton_[parent].next.count(character)) {
            automaton_[parent].next[character] = current;
            //
            automaton_[current].path += automaton_[parent].path;
            //
            parent = automaton_[parent].link;
        }
        if (parent == -1) {
            automaton_[current].link = 0;
        } else {
            int next = automaton_[parent].next[character];
            if (automaton_[parent].length + 1 == automaton_[next].length) {
                automaton_[current].link = next;
            } else {
                int clone = static_cast<int>(size_++);
                automaton_[clone].length = automaton_[parent].length + 1;
                automaton_[clone].next = automaton_[next].next;
                automaton_[clone].link = automaton_[next].link;
                while (parent != -1 && automaton_[parent].next[character] == next) {
                    automaton_[parent].next[character] = clone;
                    //
                    automaton_[next].path -= automaton_[parent].path;
                    automaton_[clone].path += automaton_[parent].path;
                    //
                    parent = automaton_[parent].link;
                }
                automaton_[next].link = automaton_[current].link = clone;
            }
        }
        last_ = current;
        //
        return automaton_[current].path;
        //
    }

    Chain& Get(size_t index) {
        return automaton_[index];
    }

    size_t Size() {
        return size_;
    }

private:
    size_t size_;
    int last_;
    std::vector<Chain> automaton_;
};

void Solution([[maybe_unused]] const std::string& word) {
    SuffixAutomaton automaton;
    const size_t n_constant = word.size();
    size_t substrings = 0;
    for (size_t i = 0; i < n_constant; i++) {
        substrings += automaton.Extend(word[i]);
        std::cout << substrings << std::endl;
    }
}

int main() {
    std::string word;
    std::cin >> word;
    Solution(word);
    return 0;
}