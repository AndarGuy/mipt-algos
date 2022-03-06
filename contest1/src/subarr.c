/*

Точные суммы

Умеете ли вы быстро складывать числа? На вход программы подаётся массив V
размером 10⩽N⩽100000 целых чисел, каждое из которых лежит в диапазоне от 1 до
109 и 1⩽M⩽100000 образцов. Для каждого образца требуется определить, существует
ли такой подмассив массива V, сумма которого равна точно значению образца.

*/

/*

2 3 5 6 4 7 4 2 1 4
2 5 10 16 20 27 31 33 34 38
13

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    long long V, M;
    scanf("%lld%lld", &V, &M);

    long long *num = calloc(V, sizeof(long long)),
              *sum = calloc(V + 1, sizeof(long long)),
              *req = calloc(M, sizeof(long long));

    for (size_t i = 0; i < V; i++) {
        scanf("%lld", num + i);
    }

    for (size_t i = 0; i < M; i++) {
        scanf("%lld", req + i);
    }

    // precount

    for (size_t i = 1; i < V + 1; i++) {
        sum[i] = sum[i - 1] + num[i - 1];
    }

    for (size_t i = 0; i < M; i++) {
        long long target = req[i];
        long long left = 0, right = 0;
        long long found = 0;
        while (right < V + 1 && left < V + 1) {
            if (sum[right] - sum[left] > target)
                left++;
            else if (sum[right] - sum[left] < target)
                right++;
            else {
                printf("%lld %lld\n", left + 1, right + 1);
                found = 1;
                break;
            }
        }
        if (!found) printf("Not found\n");
    }

    // 1 2 5 8 10 16 25
    // 3 5 13 15

    return 0;
}