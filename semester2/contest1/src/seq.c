/*

Последовательность строк

Имеется последовательность строк:

1. a

2. baa

3. cbaabaa

4. dcbaabaacbaabaa

5. ...

Каждая последующая строка формируется из повторения предыдущей строки и
дописывания в начала очередной буквы по алфавиту.

*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char get_seq(int at, int depth, int length) {
    // printf("%d %d %d\n", at, depth, length);
    if (at <= 1)
        return 'a' + depth - 1;
    else if (at > length / 2 + 1) {
        return get_seq(at - length / 2 - 1, depth - 1, length / 2);
    } else {
        return get_seq(at - 1, depth - 1, length / 2);
    }
}

int main(int argc, char *argv[]) {
    int N, K, L;
    scanf("%d%d%d", &N, &K, &L);

    int length = (1 << N) - 1;

    for (size_t j = K; j <= L; j++) {
        printf("%c", get_seq(j, N, length));
    }

    return 0;
}
