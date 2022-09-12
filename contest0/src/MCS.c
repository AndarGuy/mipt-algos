/*

Максимальная общая подстрока

Во входном файле находятся две строки, длиной до 30000 символов, состоящих из цифр и больших и малых букв латинского алфавита, каждая в отдельной строке файла.

Необходимо найти общую подстроку наибольшей длины. Если таких подстрок несколько, то следует вывести ту из них, которая лексикографически меньше.

Обратите внимание, что в приведённом примере имеется две подстроки длины 4 - rash и abra. Несмотря на то, что первая встречается раньше, ответом будет вторая, так как она лексикографически меньше.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30000

void mcs(char *stringA, char *stringB, size_t lengthA, size_t lengthB) {
    char *max = stringA;
    int maxLength = 0;

    for (size_t i = 0; i < lengthA; i++) {
        int streak = 0;
        for (size_t j = i, k = 0; j < lengthA && k < lengthB; j++, k++) {
            if (stringA[j] == stringB[k]) {
                streak++;
                char *current = stringA + j - streak + 1;
                if (streak > maxLength) {
                    maxLength = streak;
                    max = current;
                } else if (streak == maxLength)
                    if (memcmp(max, current, streak) > 0) max = current;
            } else
                streak = 0;
        }
    }

    for (size_t i = 1; i < lengthB; i++) {
        int streak = 0;
        for (size_t j = 0, k = i; j < lengthA && k < lengthB; j++, k++) {
            if (stringA[j] == stringB[k]) {
                streak++;
                char *current = stringA + j - streak + 1;
                if (streak > maxLength) {
                    maxLength = streak;
                    max = current;
                } else if (streak == maxLength) {
                    if (memcmp(max, current, streak) > 0) max = current;
                }
            } else
                streak = 0;
        }
    }

    max[maxLength] = 0;
    printf("%s\n", max);
}

int main() {
    char *stringA = calloc(MAX + 1, sizeof(char)), *stringB = calloc(MAX + 1, sizeof(char));
    scanf("%s%s", stringA, stringB);
    size_t lengthA = strlen(stringA), lengthB = strlen(stringB);
    mcs(stringA, stringB, lengthA, lengthB);
    return 0;
}

/*

F[x][y] = F[x - 1][y - 1] + 1 если s1[x] == s2[y], иначе 0;

*/