/*

Точные суммы

Умеете ли вы быстро искать нужное? На вход программы подаётся массив V размером
10⩽N⩽100000 целых чисел, каждое из которых лежит в диапазоне от -109 до 109 и
1⩽M⩽100000 образцов. Для каждого образца требуется определить, существует ли
такая пара чисел, принадлежащих массиву, которая даёт в сумме ровно значение
образца.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comparator(const void *a, const void *b) {
    return *((const int *)a) - *((const int *)b);
}

int main(int argc, char *argv[]) {
    int V, M;
    scanf("%d%d", &V, &M);

    int *num = calloc(V * 2, sizeof(int)), *req = calloc(M, sizeof(int));

    for (size_t i = 0; i < V; i++) {
        scanf("%d", num + i * 2);
        num[i * 2 + 1] = i;
    }

    for (size_t i = 0; i < M; i++) {
        scanf("%d", req + i);
    }

    qsort(num, V, sizeof(int) * 2, comparator);

    for (size_t i = 0; i < M; i++) {
        int target = req[i];
        int left = 0, right = V - 1;
        int found = 0;
        while (right - left > 0) {
            int right_value = num[right * 2], left_value = num[left * 2];
            if (right_value + left_value > target)
                right--;
            else if (right_value + left_value < target)
                left++;
            else {
                printf("%d %d\n", num[left * 2], num[right * 2]);
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