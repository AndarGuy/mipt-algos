#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

struct Branch {
    static const std::size_t kN = 26;
    int children[kN] = {-1};
    bool leaf = false;

    int parent = -1;
    char parent_character = 0;

    int suffix_link = -1;
    int transition[kN] = {-1};

    explicit Branch(int parent = -1, char parent_character = '\0')
        : parent(parent), parent_character(parent_character){};
};

template <typename T>
struct Trie {
private:
    std::vector<Branch> branches_;

public:
    Trie() {
        branches_.emplace_back();
    }

    Branch GetRoot() {
        return branches_[0];
    }

    void AddString(T const& string) {
        int current_vertex = 0;
        for (char character : string) {
            int character_index = character - 'a';
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
        int character_index = character - 'a';
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
