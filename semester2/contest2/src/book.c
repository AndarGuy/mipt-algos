/*

Издательство

Очень плодовитый автор Правдолюбов принёс в издательство свою новую эпопею, от
которой издательству отказаться не удалось (как ни тяжела ноша издательства, а
адвокат у Правдолюбова был очень известным, что тут поделаешь). В эпопее много
глав (1⩽N⩽1000000), в каждой из которых было до 10000 страниц. Издать такой
монументальный труд в виде одного тома оказалось никак невозможно, поэтому
эпопею для печати потребовалось разбить на тома. Как обычно, каждый том содержит
последовательные главы книги. Было решено издать сей продукт творчества в K
томах, причём размер каждого тома в страницах должен быть ограничен каким-то
числом M.

Ваша задача — найти такое минимальное число M, чтобы при разбиении книги на тома
размер тома не превосходил M и при этом потребовалось бы не более K томов.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    size_t K, N;
    scanf("%zu%zu", &K, &N);
    size_t *books = malloc(sizeof(size_t) * N);
    size_t sum = 0, max = 0;
    for (size_t i = 0; i < N; i++) {
        scanf("%zu", books + i);
        sum += books[i];
        max = max > books[i] ? max : books[i];
    }

    size_t left = max, right = sum;
    while (right - left > 0) {
        size_t mid = (left + right) / 2;
        size_t volume = 1, current = 0;
        for (size_t i = 0; i < N; i++) {
            current += books[i];
            if (current > mid) {
                volume++;
                if (volume > K) break;
                current = books[i];
            }
        }
        if (volume > K) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    printf("%zu\n", left);

    return 0;
}