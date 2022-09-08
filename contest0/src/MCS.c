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

int mcs(char *stringA, char *stringB, unsigned long lengthA, unsigned long lengthB) {
    int *cache = calloc(MAX, sizeof(int));
    for (size_t i = 0; i < lengthA; i++)
    {
        for (size_t j = 0; j < lengthB; j++)
        {
            int k = cache[j];
            while (k > 0 && stringA[i] != stringA[k]) k = cache[k];
            if (stringA[i] == stringB[k]) k++;
            cache[i] = k;
            printf("%d\n", k);
        }
        
    }
    return 0;
    
}

int main() {

    char *stringA = calloc(MAX, sizeof(char)), *stringB = calloc(MAX, sizeof(char));
    scanf("%s%s", stringA, stringB);
    unsigned long lengthA = strlen(stringA), lengthB = strlen(stringB);
    
    mcs(stringA, stringB, lengthA, lengthB);

    return 0;
}

/*



*/