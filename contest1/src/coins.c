/*

Пары монет

Митрофан, путешествуя по Африке, зашёл в комисионный магазин и, к своей радости,
обнаружил там редкую флейту, которую продавали ну очень недорого. Подойдя к
кассе, он установил, что забыл дома как пластиковую банковскую карту, так и
кошелёк с купюрами. К счастью для него, у него в кармане нашлась монетница, в
которой хранились монеты различных номиналов, причём ровно по две монеты каждого
номинала. Задача состоит в том, чтобы купить флейту, истратив наименьшее
возможное количество монет.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT32_MAX 2147483647

int subsets(int *coins, int *valve, int elements, int target, int depth,
            int length) {
    if (depth == length) {
        int sum = 0;
        for (size_t i = 0; i < elements; i++) {
            // printf("%d ", valve[i]);
            sum += valve[i];
        }
        // printf("\n");
        if (sum == target)
            return elements;
        else if (sum > target) {
            return INT32_MAX - 1;
        } else {
            return INT32_MAX;
        }
    }
    int result_a = subsets(coins, valve, elements, target, depth + 2, length);

    valve[elements] = coins[depth];

    int result_b =
        subsets(coins, valve, elements + 1, target, depth + 2, length);

    valve[elements + 1] = coins[depth + 1];

    int result_c =
        subsets(coins, valve, elements + 2, target, depth + 2, length);

    return result_a < result_b ? (result_a < result_c ? result_a : result_c)
                               : (result_b < result_c ? result_b : result_c);
}

int main(int argc, char *argv[]) {
    int V, N;
    scanf("%d%d", &V, &N);
    int *coins = calloc(15, sizeof(int)), *valve = calloc(15, sizeof(int));
    for (int i = 0; i < N * 2; i += 2) {
        scanf("%d", coins + i);
        coins[i + 1] = coins[i];
    }

    int result = subsets(coins, valve, 0, V, 0, N * 2);
    if (result == INT32_MAX - 1)
        printf("Change");
    else if (result == INT32_MAX)
        printf("Impossible");
    else
        printf("%d\n", result);

    return 0;
}