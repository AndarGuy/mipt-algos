/*

Битва

Дон Кихот, наконец, осуществил мечту своей жизни, и обнаружил полчища драконов,
расположенных в линию. (ну мы же не скажем ему, что это — солнечные панели?).

За время странствий ему удалось усовершенствовать свой меч так, чтобы его удар
захватывал широкое пространство. Та цель, на которую попадает основной удар,
теряет p хитпойнтов, все остальные — по q хитпойнтов. Тот дракон, у которого
количество хитпойнтов равно нулю или меньше нуля, погибает.

Так сколько же ударов должен нанести Дон Кихот, чтобы уничтожить все солнечные
панели?

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int updiv(int a, int b) { return a / b + (a % b > 0); }

int main(int argc, char *argv[]) {
    int N, Q, P;
    scanf("%d%d%d", &N, &Q, &P);

    int *hitpoints = malloc(sizeof(int) * N);
    int max = 0;
    for (size_t i = 0; i < N; i++) {
        scanf("%d", hitpoints + i);
        max = max > hitpoints[i] ? max : hitpoints[i];
    }

    size_t left = 1, right = updiv(max, P);
    while (right - left > 0) {
        size_t mid = (left + right) / 2;
        int extra = 0;
        for (size_t i = 0; i < N; i++) {
            int after_splash = hitpoints[i] - P * mid;
            if (after_splash > 0) {
                if (Q != P)
                    extra += updiv(after_splash, Q - P);
                else
                    extra = mid + 1;
                if (extra > mid) break;
            }
        }

        if (extra > mid) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }

    printf("%zu\n", right);

    free(hitpoints);

    return 0;
}

/**
 * @complexity O(N * LogN)
 *
 * @memory N
 */