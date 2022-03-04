/*

Периодическая дробь

Выведите десятичное представление рациональной правильной дроби. Если в
представлении присутствует период, то нужно вывести первое его вхождение в
круглых скобках.

*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./modules/simpleset/simpleset.h"
#include "./modules/vector/vector.h"

unsigned get_remainder(unsigned N, unsigned M) {
    struct set *remainders = set_new(150000000);
    for (size_t i = 0; 1; i++) {
        N *= 10;
        N = N % M;
        if (N == 0) return 0;
        if (!set_find(remainders, N))
            set_insert(remainders, N);
        else
            return N;
    }

    set_delete(remainders);
}

int main(int argc, char *argv[]) {
    unsigned N, M;
    scanf("%u %u", &N, &M);

    struct vector *quotients = vector_new(0, sizeof(unsigned));

    unsigned target = get_remainder(N, M);
    unsigned start = UINT32_MAX, period = 0;

    for (size_t i = 0; 1; i++) {
        N *= 10;
        int quotient = N / M;
        vector_push(quotients, &quotient);
        N = N % M;
        if (N == 0) break;

        if (N == target) {
            if (start == UINT32_MAX)
                start = i;
            else {
                period = i - start;
                unsigned first, last;
                vector_get(quotients, start, &first);
                vector_get(quotients, i, &last);
                if (first == last) vector_pop(quotients, &first);
                break;
            }
        }
    }

    printf("0.");

    for (size_t i = 0; i < vector_size(quotients) - period; i++) {
        unsigned temp;
        vector_get(quotients, i, &temp);
        printf("%d", temp);
    }

    if (period > 0) {
        printf("(");
        for (size_t i = vector_size(quotients) - period;
             i < vector_size(quotients); i++) {
            unsigned temp;
            vector_get(quotients, i, &temp);
            printf("%d", temp);
        }
        printf(")\n");
    }

    quotients = vector_delete(quotients);

    return 0;
}