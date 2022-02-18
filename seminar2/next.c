/*
Задано слово, состоящее из строчных букв латинского алфавита. 

Рассмотрим набор всех перестановок букв этого слова (например, abc имеет всего 6 перестановок — abc, acb, bac, bca, cab, cba, а слово abb — три перестановки, 
abb, bab, bba). Требуется выбрать из этого множества слово, следующее за заданным по алфавиту.
*/

#include <stdio.h>
#include <string.h>

int main() {
    char word[1001];
    fgets(word, 1000, stdin);
    int word_len = strlen(word);
    word[word_len - 1] = 0;
    word_len--;
    int i, j;
    for (i = 1; i < word_len; i++) {
        int done = 0;
        for (j = 0; j < i; j++) {
            int a = word_len - i - 1;
            int b = word_len - j - 1;
            if (word[a] < word[b]) {
                char temp = word[a];
                word[a] = word[b];
                word[b] = temp;
                done = 1;
                break;
            }
        }
        if (done) break;
    }

    for (int k = 0; k <= word_len - i - 1; k++) {
        printf("%c", word[k]);
    }

    for (int k = 0; k < i; k++) {
        printf("%c", word[word_len - k - 1]);
    }
    return 0;
}