/*

Сопоставление по образцу

Известно, что при работе с файлами можно указывать метасимволы * и ? для отбора нужной группы файлов, причём знак * соответствует любому множеству, даже пустому, в имени файла, а символ ? соответствует ровно одному символу в имени.

Первая строка программы содержит имя файла, состоящее только из заглавных букв латинского алфавита (A-Z), а вторая - образец, содержащий только заглавные буквы латинского алфавита и, возможно, символы * и ?. Строки не превышают по длине 700 символов.

Требуется вывести слова YES или NO в зависимости от того, сопоставляется ли имя файла указанному образцу.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOUND 1000

int matching_reverse(char **cache, char *string, char *template, int x, int y) {
    if (x < 0 || y < 0) return 0;
    if (cache[x][y] == 2) return 1;
    if (cache[x][y] != 0) return 0;
    cache[x][y] = -1;
    if (string[y] != template[x] && template[x] != '?' && template[x] != '*') return 0;
    cache[x][y] = 1;
    if (x == 0 && y == 0) return 1;
    if (template[x] == '*' && (matching_reverse(cache, string, template, x, y - 1) || matching_reverse(cache, string, template, x - 1, y))) return 1;
    return matching_reverse(cache, string, template, x - 1, y - 1);
}

int matching(char **cache, char *string, char *template, int x, int y, int N, int M) {
    if (x > N || y > M) return 0;
    if (cache[x][y] == 1) return 1;
    if (cache[x][y] != 0) return 0;
    cache[x][y] = -1;
    if (string[y] != template[x] && template[x] != '?' && template[x] != '*') return 0;
    cache[x][y] = 2;
    if (x == N - 1 && y == M - 1) return 1;
    if (template[x] == '*' && (matching(cache, string, template, x, y + 1, N, M) || matching(cache, string, template, x + 1, y, N, M))) return 1;
    return matching(cache, string, template, x + 1, y + 1, N, M);
}

int main(int argc, char *argv[]) {
    char string[BOUND], template[BOUND];
    scanf("%s%s", string, template);

    char **cache = calloc(BOUND, sizeof(char *));
    for (size_t i = 0; i < BOUND; i++) {
        cache[i] = calloc(BOUND, sizeof(char));
    }

    printf("%s\n", matching_reverse(cache, string, template, strlen(template) - 1, strlen(string) - 1) || matching(cache, string, template, 0, 0, strlen(template), strlen(string)) ? "YES" : "NO");

    // for (size_t i = 0; i < strlen(template); i++) {
    //     for (size_t j = 0; j < strlen(string); j++) {
    //         printf("%d ", cache[i][j]);
    //     }
    //     printf("\n");
    // }

    return 0;
}

//