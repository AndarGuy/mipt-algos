/*

Кузнечик

Имеется лестница, в которой N ступенек. Кузнечик находится перед лестницей. Он прыгает только вверх и способен прыгнуть вверх не более, чем на M ступенек.

Определить, сколькими способами кузнечик может допрыгать да самой верхней ступеньки.

1⩽M⩽N⩽30

Для N=5 и M=2 имеется 8 маршрутов: (1,2,3,4,5),(1,2,3,5), (1,2,4,5), (1,3,4,5), (1,3,5), (2,3,4,5), (2,3,5), (2,4,5)

*/

#include <stdio.h>
#include <stdlib.h>

int main() {
    int N, M;
    scanf("%d%d", &N, &M);

    int *ladder = calloc(N, sizeof(int));
    for (int i = 0; i < M; i++) ladder[i] = 1;

    for (int i = 1; i < N; i++) {
        for (int j = 1; j <= M; j++) {
            if (i - j < 0) break;
            ladder[i] += ladder[i - j];
        }
    }
    
    printf("%d", ladder[N - 1]);

    return 0;
}

/* 

f(x) = f(x - 1) + f(x - 2) + ... + f(x - M)

*/