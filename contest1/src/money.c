/*

Покупка

У покупателя имеется набор N монет A1⩽A2⩽···⩽AN, каждой монеты ровно по одной
штуке.

Найти наименьшую стоимость предмета, который нельзя купить без сдачи, используя
только эти монеты.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_int(const void *a) { printf("%d", *((const int *)a)); }

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);

    int *coins = calloc(N, sizeof(int));
    for (size_t i = 0; i < N; i++) {
        scanf("%d", coins + i);
    }

    int interupted = 0;
    int sum = 0;
    for (size_t i = 0; i < N; i++) {
        if (sum >= coins[i] - 1) {
            sum += coins[i];
        } else {
            printf("%d", sum + 1);
            interupted = 1;
            break;
        }
    }

    if (!interupted) printf("%d", sum + 1);

    return 0;
}