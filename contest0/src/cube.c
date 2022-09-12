/*

Бросание кубика

Теория вероятности начиналась с изучения великими математиками игр, связанных с бросанием кубика. Мы тоже будем бросать кубик с равновероятно выпадающими числами от 1 до 6 и будем вычислять вероятности накопления определённых сумм.

Требуется узнать, какова вероятность события, что при 1⩽N⩽500 бросаниях кубика сумма всех выпавших очков равна в точности M. Для прохождения теста достаточно, чтобы выведенный результат отличался от точного не более, чем на 10-8

*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

long double cube(int N, int M, long double *cache[500 + 1]) {
    long double result;
    if (M <= 0) return 0;
    if (M > N * 6) return 0;
    if (M < N) return 0;
    if (N <= 0) return 0;
    if (cache[N][M] != 0)
        return cache[N][M];

    result = cube(N - 1, M - 1, cache) + cube(N - 1, M - 2, cache) + cube(N - 1, M - 3, cache) + cube(N - 1, M - 4, cache) + cube(N - 1, M - 5, cache) + cube(N - 1, M - 6, cache);
    cache[N][M] = result;
    return result;
}

int main() {
    int N, M;
    scanf("%d%d", &N, &M);
    long double *cache[500 + 1];
    for (size_t i = 0; i < 500 + 1; i++) {
        cache[i] = calloc(3000 + 1, sizeof(long double));
    }

    for (size_t i = 0; i < 6; i++) {
        cache[1][i + 1] = 1;
    }

    long double result = cube(N, M, cache);

    for (size_t i = 0; i < N; i++) {
        result /= 6;
    }

    printf("%.10Lf\n", result);

    return 0;
}

/*

f(x) = f(x - 1) + f(x - 2) + f(x - 3) + f(x - 4) + f(x - 5) + f(x - 6)

*/