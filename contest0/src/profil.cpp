/*

Замощение

Прямоугольник размером N × M требуется полностью замостить фигурами домино (1 × 2). и тримино (1 × 3).

Определите, сколькими способами можно это сделать.

*/

#include <iostream>

using namespace std;

size_t profil(int N, int M) {
    size_t slices = 1 << N;

    bool **cache = new bool *[slices];
    for (size_t i = 0; i < slices; i++) cache[i] = new bool[slices];

    for (size_t i = 0; i < slices; i++) {
        for (size_t j = 0; j < slices; j++) {
            cache[i][j] = 1;
            for (size_t k = 1, mask = 0b11; k < N; k++, mask <<= 1) {
                size_t i_ = i & mask, j_ = j & mask;
                if ((i & j & mask) == mask || (i & mask | j & mask) == 0) cache[i][j] = 0;
            }
        }
    }

    // for (size_t i = 0; i < slices; i++) {
    //     for (size_t j = 0; j < slices; j++) {
    //         cout << cache[i][j] << ' ';
    //     }
    //     cout << endl;
    // }

    size_t **answer = new size_t *[M + 1];
    for (size_t i = 0; i < M + 1; i++) answer[i] = new size_t[slices];

    for (size_t i = 0; i < slices; i++) {
        answer[0][i] = 1;
    }

    for (size_t i = 1; i < M; i++) {
        for (size_t j = 0; j < slices; j++) {
            for (size_t k = 0; k < slices; k++) {
                answer[i][j] += answer[i - 1][k] * cache[k][j];
            }
        }
    }

    // for (size_t i = 0; i < M; i++) {
    //     for (size_t j = 0; j < slices; j++) {
    //         cout << answer[i][j] << ' ';
    //     }
    //     cout << endl;
    // }

    size_t result = 0;
    for (size_t i = 0; i < slices; i++) {
        result += answer[M - 1][i];
    }

    return result;
}

int main(int argc, char *argv[]) {
    int N, M;
    cin >> N >> M;
    if (M < N) swap(N, M);

    cout << profil(N, M);

    return 0;
}

//