/*

Комитет По Исследованию Бинарных Вирусов обнаружил, что некоторые последовательности единиц и нулей являются кодами
вирусов. Комитет изолировал набор кодов вирусов. Последовательность из единиц и нулей называется безопасной, если
никакой ее сегмент (т.е. последовательность из соседних элементов) не является кодом вируса. Сейчас цель комитета
состоит в том, чтобы установить, существует ли бесконечная безопасная последовательность из единиц и нулей.

*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <queue>

struct Branch {
    static const std::size_t kN = 2;
    int children[kN];
    bool leaf = false;

    int parent = -1;
    char parent_character = 0;

    int suffix_link = -1;
    int transition[kN];

    explicit Branch(int parent = -1, char parent_character = '\0')
        : parent(parent), parent_character(parent_character) {
        std::fill(std::begin(transition), std::end(transition), -1);
        std::fill(std::begin(children), std::end(children), -1);
    };

    bool IsLeaf() const {
        return leaf;
    }
};

template <typename T>
struct Trie {
public:
    std::vector<Branch> branches_;

public:
    Trie() {
        branches_.emplace_back();
    }

    const Branch& GetRoot() const {
        return branches_[0];
    }

    const Branch& GetBranch(size_t index) const {
        return branches_[index];
    }

    size_t Size() const {
        return branches_.size();
    }

    void AddString(T const& string) {
        // std::cout << "ADDING STRING \"" << string << "\"" << std::endl;
        int current_vertex = 0;
        for (char character : string) {
            int character_index = character - '0';
            if (branches_[current_vertex].children[character_index] == -1) {
                branches_[current_vertex].children[character_index] = branches_.size();
                branches_.emplace_back(current_vertex, character);
            }
            current_vertex = branches_[current_vertex].children[character_index];
        }
        branches_[current_vertex].leaf = true;
    }

    int GetSuffixLink(int vertex) {
        if (branches_[vertex].suffix_link == -1) {
            if (vertex == 0 || branches_[vertex].parent == 0) {
                branches_[vertex].suffix_link = 0;
            } else {
                branches_[vertex].suffix_link =
                    Transit(GetSuffixLink(branches_[vertex].parent), branches_[vertex].parent_character);
            }
        }
        return branches_[vertex].suffix_link;
    }

    int Transit(int vertex, char character) {
        int character_index = character - '0';
        if (branches_[vertex].transition[character_index] == -1) {
            if (branches_[vertex].children[character_index] != -1) {
                branches_[vertex].transition[character_index] = branches_[vertex].children[character_index];
            } else {
                branches_[vertex].transition[character_index] =
                    vertex == 0 ? 0 : Transit(GetSuffixLink(vertex), character);
            }
        }
        return branches_[vertex].transition[character_index];
    }
};

template <typename T>
static Trie<T> MakeTrie(std::vector<T>& strings) {
    Trie<T> trie;
    for (T string : strings) {
        trie.AddString(string);
    }
    return trie;
}

template <typename T>
void RemoveEdges(Trie<T>& trie, std::vector<std::pair<int, int>>& copied) {
    std::queue<int> queue;
    queue.push(0);
    while (!queue.empty()) {
        int vertex = queue.front();
        queue.pop();
        auto [left_child, right_child] = copied[vertex];
        for (auto child : {left_child, right_child}) {
            if (child == -1) {
                continue;
            }
            trie.branches_[child].leaf =
                trie.branches_[child].leaf || trie.branches_[trie.branches_[child].suffix_link].leaf;
            queue.push(child);
        }
    }
}

void Solution([[maybe_unused]] size_t n_constant, [[maybe_unused]] std::vector<std::string>& strings) {
    Trie trie = MakeTrie(strings);

    std::vector<std::pair<int, int>> copied;
    for (size_t i = 0; i < trie.Size(); i++) {
        copied.emplace_back(trie.branches_[i].children[0], trie.branches_[i].children[1]);
    }

    for (size_t i = 0; i < trie.Size(); i++) {
        for (size_t j = 0; j < Branch::kN; j++) {
            if (trie.branches_[i].children[j] == -1) {
                trie.branches_[i].children[j] = trie.Transit(static_cast<int>(i), static_cast<char>('0' + j));
            }
        }
    }

    RemoveEdges(trie, copied);

    enum Color { WHITE, GREY, BLACK };
    std::vector<int> visited(trie.Size(), WHITE);

    std::function<bool(int)> dfs = [&](int index) {
        const Branch& branch = trie.GetBranch(index);
        if (branch.IsLeaf()) {
            // std::cout << "FOUND LEAF ON BRANCH " << index << ", SO DEAD." << std::endl;
            return false;
        }

        // std::cout << "GOING " << index << " WITH COUNTER " << counter << std::endl;
        if (visited[index]) {
            // std::cout << "LEAF " << index << " ALREADY VISITED, SO DEAD." << std::endl;
            return false;
        }

        visited[index] = GREY;

        for (size_t i = 0; i < Branch::kN; i++) {
            int child = branch.children[i];
            if (visited[child] == GREY || dfs(child)) {
                return true;
            }
        }

        visited[index] = BLACK;

        return false;
    };

    std::cout << (dfs(0) ? "TAK" : "NIE") << std::endl;
}

int main() {
    std::size_t n_constant = 0;
    std::cin >> n_constant;
    std::vector<std::string> strings(n_constant);
    for (size_t i = 0; i < n_constant; i++) {
        std::cin >> strings[i];
    }

    Solution(n_constant, strings);

    return 0;
}