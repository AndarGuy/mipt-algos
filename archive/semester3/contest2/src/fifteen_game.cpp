/*

Пятнашки

Написать алгоритм для решения игры в "пятнашки". Решением задачи является приведение к виду:

1   2   3   4
5   6   7   8
9   10  11  12
13  14  15  0

Необходимо найти любое из кратчайших решений.

*/

#include <array>
#include <cinttypes>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

#define N 4
#define M 4

class PLAYGROUND {
    struct ROW {
        char a1 : 4, a2 : 4, a3 : 4, a4 : 4;

        struct MODIFIABLE_CELL {
            ROW* row;
            size_t no;

            char operator=(const char value) {
                switch (no) {
                    case 0:
                        row->a1 = value;
                        break;
                    case 1:
                        row->a2 = value;
                        break;
                    case 2:
                        row->a3 = value;
                        break;
                    case 3:
                        row->a4 = value;
                        break;
                }

                return value;
            }
        };

        MODIFIABLE_CELL operator[](size_t index) {
            return MODIFIABLE_CELL{this, index};
        }

        const char operator()(size_t index) {
            switch (index) {
                case 0:
                    return a1;
                case 1:
                    return a2;
                case 2:
                    return a3;
                case 3:
                    return a4;
                default:
                    return -1;
            }
        }
    };

   public:
    PLAYGROUND() = default;

    ROW operator[](size_t index) {
        return storage.rows[index];
    }

    bool operator==(const PLAYGROUND& other) {
        return storage.data == other.storage.data;
    }

    PLAYGROUND(std::array<std::array<char, 4>, 4> array) {
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < M; j++) {
                this->operator[](i)[j] = array[i][j];
            }
        }
    }

   private:
    union {
        ROW rows[4];
        uint64_t data = 0;
    } storage;
};

std::pair<int, int> get_gap(std::array<std::array<char, M>, N>& state) {
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < M; j++)
            if (!state[i][j]) return {i, j};

    return {0, 0};
}

std::vector<std::pair<char, std::array<std::array<char, M>, N>>> get_turns(std::array<std::array<char, M>, N>& state) {
    std::vector<std::pair<char, std::array<std::array<char, M>, N>>> result;
    auto gap = get_gap(state);
    int x = gap.first, y = gap.second;
    if (x > 0) {
        std::array<std::array<char, M>, N> turn = state;
        std::swap(turn[x - 1][y], turn[x][y]);
        result.push_back({'D', turn});
    }
    if (y > 0) {
        std::array<std::array<char, M>, N> turn = state;
        std::swap(turn[x][y - 1], turn[x][y]);
        result.push_back({'R', turn});
    }
    if (x + 1 < N) {
        std::array<std::array<char, M>, N> turn = state;
        std::swap(turn[x + 1][y], turn[x][y]);
        result.push_back({'U', turn});
    }
    if (y + 1 < M) {
        std::array<std::array<char, M>, N> turn = state;
        std::swap(turn[x][y + 1], turn[x][y]);
        result.push_back({'L', turn});
    }
    return result;
}

void display(std::array<std::array<char, M>, N>& state) {
    std::cout << "---" << std::endl;
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            std::cout << ((int)state[i][j]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "---" << std::endl;
}

// def a_star(start_chain, goal_node):
//     node_hash = {}
//     chains_queue = []
//     heappush(chains_queue, start_chain)
//     while chains_queue:
//         cur_chain = heappop(chains_queue)
//         cur_node = cur_chain.last_node()
//         if cur_node == goal_node:
//             return cur_chain
//         node_hash[cur_node] = cur_chain.g()
//         for chain in cur_chain.get_neighbours():
//             if chain.last_node() in node_hash:
//                 if chain.g() >= node_hash[chain.last_node()]:
//                     continue
//                 node_hash[chain.last_node()] = chain.g()
//             heappush(chains_queue, chain)

std::pair<int, std::string> bfs(std::array<std::array<char, M>, N> state, std::array<std::array<char, M>, N>& required, std::map<std::array<std::array<char, M>, N>, int>& visited) {
    std::queue<std::pair<std::string, std::array<std::array<char, M>, N>>> queue;
    queue.push({"", state});

    while (!queue.empty()) {
        auto [history, state] = queue.front();
        queue.pop();
        // display(state);
        if (required == state) return {visited[state], history};
        for (auto [type, turn] : get_turns(state)) {
            if (!visited[turn]) {
                visited[turn] = visited[state] + 1;
                queue.push({history + type, turn});
            }
        }
    }

    return {-1, ""};
}

void fifteen_game(std::array<std::array<char, M>, N> state, std::array<std::array<char, M>, N> required) {
    std::map<std::array<std::array<char, M>, N>, int> visited;
    auto [n, history] = bfs(state, required, visited);
    std::cout << n << std::endl;
    if (n > -1) std::cout << history << std::endl;
}

int main() {
    PLAYGROUND required({{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 0}}}), state;

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            int temp;
            std::cin >> temp;
            state[i][j] = (char)temp;
        }
    }

    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < M; j++) {
            std::cout << ((int)state[i](j)) << ' ';
        }
        std::cout << std::endl;
    }

    // const auto [R, C] = get_gap(state);
    // if ((N + R) % 2)
    //     std::cout << -1;
    // else
    //     fifteen_game(state, required);

    return 0;
}