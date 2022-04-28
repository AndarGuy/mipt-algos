/*

Инверсии

Известно, что сложность сортировки может определяться числом инверсий — пар
элементов последовательности, расположенных не в надлежащем порядке.

Например, в последовательности {4,5,1,2}, которую нужно упорядочить по
неубыванию, имеются следующие пары, образующие инверсии: (4,1), (4,2), (5,1),
(5,2).

Требуется установить, а сколько же инверсий в заданной последовательности длины
N, которую нужно упорядочить по неубыванию. Имеется не более 10000000 целых
чисел, каждое из которых лежит в диапазоне от -1000000 до 1000000.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t get_inversions(int arr[], int left, int right) {
    if (right - left < 2) {
        return 0;
    }

    int mid = left + (right - left) / 2;
    size_t inversions =
        get_inversions(arr, left, mid) + get_inversions(arr, mid, right);

    int *merged = calloc(right - left, sizeof(int));
    int cursorA = left, cursorB = mid;
    for (int i = 0; i < (right - left); i++) {
        if (cursorA >= mid) {
            merged[i] = arr[cursorB];
            cursorB++;
        } else if (cursorB >= right) {
            merged[i] = arr[cursorA];
            cursorA++;
        } else if (arr[cursorA] <= arr[cursorB]) {
            merged[i] = arr[cursorA];
            cursorA++;
        } else {
            merged[i] = arr[cursorB];
            cursorB++;
            inversions += mid - cursorA;
        }
    }

    memcpy(arr + left, merged, (right - left) * sizeof(int));
    free(merged);

    return inversions;
}

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);
    int *numbers = calloc(10000000 + 1, sizeof(int));
    for (size_t i = 0; i < N; i++) {
        scanf("%d", numbers + i);
    }
    printf("%zu\n", get_inversions(numbers, 0, N));

    return 0;
}