/*

В некотором государстве в обращении находятся банкноты определенных номиналов. Центральный банк хочет, чтобы банкомат выдавал любую запрошенную сумму при помощи минимального числа банкнот, считая, что запас банкнот каждого номинала неограничен. Помогите Центральному банку решить эту задачу.

Первая строка входных данных содержит натуральное число N не превосходящее 100 — количество номиналов банкнот в обращении. Вторая строка входных данных содержит N различных натуральных чисел x1 , x2 , . . . , xN , не превосходящих 106 — номиналы банкнот. Третья строчка содержит натуральное число S, не превосходящее 106 — сумму, которую необходимо выдать.

В первую строку выходного файла выведите минимальное число слагаемых (или -1, если такого представления не существует). Во вторую строку выведите это представление в любом порядке.

*/

#include <stdio.h>
#include <stdlib.h>

#define MIN(A, B) A < B ? A : B

void buildSolution(int x, int *coins, int n, int *cache) {
    for (int rank = cache[x]; rank > 0; rank--) {
        for (int i = 0; i < n; i++) {
            int previous = x - coins[i];
            if (previous >= 0 && cache[previous] == rank - 1) {
                x = previous;
                printf("%d ", coins[i]);
                break;
            } 
        }
    }
}

int main() {
    int N;
    scanf("%d", &N);

    int *coins = calloc(N, sizeof(int));
    for (int i = 0; i < N; i++) {
        scanf("%d", coins + i);
    }

    int S;
    scanf("%d", &S);
    int *ladder = calloc(1000000 * 10, sizeof(int));
    for (int i = 0; i < S; i++) ladder[i + 1] = 99999999;
    for (int i = 0; i < N; i++) ladder[coins[i]] = 1;
    ladder[S] = 99999999;

    for (int i = 0; i < S; i++) {
        for (int j = 0; j < N; j++) {
            ladder[i + coins[j]] = MIN(ladder[i + coins[j]], ladder[i] + 1);
        }
    }

    // for (size_t i = 0; i < S + 1; i++) {
    //     printf("%d ", ladder[i]);
    // }
    // printf("\n");

    if (ladder[S] == 99999999) {
        printf("-1\n");
    } else {
        printf("%d\n", ladder[S]);
        buildSolution(S, coins, N, ladder);
    }

    free(ladder);
    free(coins);

    return 0;
}

/* 

f(x + X) = min(f(x) + 1, f(x + X))

*/