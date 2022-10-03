/*

Замощение

Прямоугольник размером N × M требуется полностью замостить фигурами домино (1 × 2). и тримино (1 × 3).

Определите, сколькими способами можно это сделать.

*/

#include <iostream>
#include <map>

int bitmask(uint64_t mask, int N, int M, uint64_t bricks[64][4], std::map<uint64_t, int> masks) {
    std::map<uint64_t, int>::iterator search = masks.find(mask);
    if (search != masks.end()) return search->second;

    int leftmost = __builtin_clzll(mask);
    int i = leftmost / M;
    int j = leftmost % M;
    int result = 0;
    if (j + 2 < M) {
        uint64_t brick = bricks[leftmost][0];
        if ((brick & mask) == brick) result += bitmask(mask - brick, N, M, bricks, masks);
    }
    if (j + 3 < M) {
        uint64_t brick = bricks[leftmost][1];
        if ((brick & mask) == brick) result += bitmask(mask - brick, N, M, bricks, masks);
    }
    if (i + 2 < N) {
        uint64_t brick = bricks[leftmost][2];
        if ((brick & mask) == brick) result += bitmask(mask - brick, N, M, bricks, masks);
    }
    if (i + 3 < N) {
        uint64_t brick = bricks[leftmost][3];
        if ((brick & mask) == brick) result += bitmask(mask - brick, N, M, bricks, masks);
    }

    return masks[mask] = result;
}

int main(int argc, char *argv[]) {
    std::map<uint64_t, int> masks;

    int N, M;
    std::cin >> N >> M;

    uint64_t bricks[64][4];

    bricks[0][0] = 0b11;
    bricks[0][1] = 0b111;
    bricks[0][2] = 0b1 + (0b1 >> M);
    bricks[0][3] = bricks[0][2] + (0b1 >> (M + M));

    for (size_t i = 1; i < N * M; i++) {
        for (size_t k = 0; k < 4; k++) {
            bricks[i][k] = bricks[i - 1][k] >> 1;
        }
    }

    std::cout << bitmask(UINT64_MAX, N, M, bricks, masks);

    return 0;
}

//