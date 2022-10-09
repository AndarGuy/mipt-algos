/*

Замощение

Прямоугольник размером N × M требуется полностью замостить фигурами домино (1 × 2). и тримино (1 × 3).

Определите, сколькими способами можно это сделать.

*/

#include <iostream>
#include <unordered_map>

uint64_t bitmask(uint64_t mask, int N, int M, uint64_t bricks[64][4], std::unordered_map<uint64_t, uint64_t> &masks) {
    if (masks.find(mask) != masks.end()) {
        // std::cout << "НАШЕЛ! " << mask << std::endl;
        return masks[mask];
    }

    // std::cout << std::bitset<16>(mask) << std::endl;

    int leftmost = 63 - __builtin_clzll(mask);
    int i = leftmost / M;
    int j = leftmost % M;
    // std::cout << leftmost << ' ' << i << ' ' << j << std::endl;
    // for (size_t i = 0; i < 4; i++) {
    //     std::cout << std::bitset<16>(bricks[leftmost][i]) << std::endl;
    // }

    uint64_t result = 0;
    if (j > 0) {
        uint64_t brick = bricks[leftmost][0];
        if (brick && (brick & mask) == brick) result += bitmask(mask - brick, N, M, bricks, masks);
    }
    if (j - 1 > 0) {
        uint64_t brick = bricks[leftmost][1];
        if (brick && (brick & mask) == brick) result += bitmask(mask - brick, N, M, bricks, masks);
    }
    if (i > 0) {
        uint64_t brick = bricks[leftmost][2];
        if (brick && (brick & mask) == brick) result += bitmask(mask - brick, N, M, bricks, masks);
    }
    if (i - 1 > 0) {
        uint64_t brick = bricks[leftmost][3];
        if (brick && (brick & mask) == brick) result += bitmask(mask - brick, N, M, bricks, masks);
    }

    masks[mask] = result;

    return result;
}

int main(int argc, char *argv[]) {
    std::unordered_map<uint64_t, uint64_t> masks;
    masks[0] = 1;

    int N, M;
    std::cin >> N >> M;
    if (N < M) std::swap(N, M);

    uint64_t bricks[64][4] = {0};

    bricks[1][0] = 3ull;
    bricks[2][1] = 7ull;
    bricks[M][2] = 1ull + (1ull << M);
    bricks[M + M][3] = bricks[M][2] + (1ull << (M + M));

    for (size_t i = 2; i < 64; i++) bricks[i][0] = bricks[i - 1][0] << 1;
    for (size_t i = 3; i < 64; i++) bricks[i][1] = bricks[i - 1][1] << 1;
    for (size_t i = M + 1; i < 64; i++) bricks[i][2] = bricks[i - 1][2] << 1;
    for (size_t i = M + M + 1; i < 64; i++) bricks[i][3] = bricks[i - 1][3] << 1;

    // for (size_t j = 0; j < 4; j++) {
    //     std::cout << std::bitset<50>(bricks[50][j]) << std::endl;
    // }

    // for (size_t i = 1; i < N; i++) {
    //     for (size_t j = 1; j < M; j++) {
    //         uint64_t mask = (1ull << i * j) - 1;
    //         bitmask(mask, i, j, bricks, masks);
    //     }
    // }

    uint64_t mask = (1ull << N * M) - 1;
    std::cout << bitmask(mask, N, M, bricks, masks);

    return 0;
}

//