/*

Телефонная книга

Вам поручили написать программу, которая является частью backend большого
проекта — реализации управления пользователями некоего сайта. Что там делают
frontend-щики нас не особо интересует, но от них поступает некий поток команд,
которые вы должны адекватно обрабатывать и возвращать результаты. Вы должны
вести некоторую внутреннюю базу данных, доступ к которой возможен только через
строго определённые команды.

К числу таких команд относятся:

• ADD name value. В базу добавляется пользователь под именем name с значением
телефонного номера value. Имена пользователей и значения телефонных номеров —
произвольные строки символов, длиной не более 30. Если пользователя с таким
именем нет, то всё происходит успешно и программа ничего не выводит. Если
пользователь с таким именем уже есть, программа должны вывести на отдельной
строке сообщение ERROR.
• DELETE name. Из базы удаляется пользователь под именем
name. Если такого пользователя нет, то вывести ERROR.
• EDITPHONE name value.
Номер телефона пользователя именем name изменяется на новое значение. Если
такого пользователя нет, то вывести ERROR.
• PRINT name. Вывести или строку именем
пользователя и его номером телефона через пробел name value или ERROR, если
пользователя с указанным именем не существует.


*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGHT 4096 + 1
#define MAX_SIZE 300000 + 1

ssize_t hash_function(void *a) {
    ssize_t hash = 0;
    ssize_t power = 1;
    for (char *i = a; *i > 0; i++, power *= 31) {
        hash += *i * power;
    }
    return hash;
}

enum EntryStatus { EMPTY, OK, ERASED };

struct Entry {
    char key[MAX_LENGHT];
    char value[MAX_LENGHT];
    long next;
    enum EntryStatus status;
};

FILE *hashmap_open(int file, char access[4]) {
    char name[20] = {0};
    sprintf(name, "hashmap%d.txt", file);
    return fopen(name, access);
}

void hashmap_add(long *entries, char *key, char *value) {
    ssize_t hash = hash_function(key) % MAX_SIZE;
    long entry = entries[hash];

    FILE *hashmap = hashmap_open(hash % 10, "rb+");
    fseek(hashmap, 0, SEEK_END);

    struct Entry record;
    for (size_t i = 0; i < MAX_LENGHT; i++) record.key[i] = record.value[i] = 0;
    strcpy(record.key, key);
    strcpy(record.value, value);
    record.status = OK;
    record.next = -1;

    // if entry was not found
    if (entry == -1) {
        entries[hash] = ftell(hashmap);
    } else {
        struct Entry temp;
        FILE *current = hashmap_open(hash % 10, "rb+");
        for (; entry > -1; entry = temp.next) {
            fseek(current, entry, SEEK_SET);
            fread(&temp, sizeof(struct Entry), 1, current);
            fseek(current, entry, SEEK_SET);

            if (temp.status != ERASED) {
                int result = strcmp(key, temp.key);
                // if key was found
                if (result == 0) {
                    printf("ERROR\n");
                    break;
                }
            }

            if (temp.next == -1) {
                temp.next = ftell(hashmap);
                fwrite(&temp, sizeof(struct Entry), 1, current);
                fwrite(&record, sizeof(struct Entry), 1, hashmap);
                break;
            }
        }
        if (entry == -1) printf("ERROR\n");
        fclose(current);
    }
    fwrite(&record, sizeof(struct Entry), 1, hashmap);
    fclose(hashmap);
}

void hashmap_print(long *entries, char *key) {
    ssize_t hash = hash_function(key) % MAX_SIZE;
    long entry = entries[hash];

    // if entry was not found
    if (entry == -1) {
        printf("ERROR\n");
        return;
    }

    struct Entry temp;
    FILE *current = hashmap_open(hash % 10, "rb");
    for (; entry > -1; entry = temp.next) {
        fseek(current, entry, SEEK_SET);
        fread(&temp, sizeof(struct Entry), 1, current);

        if (temp.status != ERASED) {
            int result = strcmp(key, temp.key);

            // if key was found
            if (result == 0) {
                printf("%s %s\n", temp.key, temp.value);
                break;
            }
        }
    }
    if (entry == -1) printf("ERROR\n");
    fclose(current);
}

void hashmap_delete(long *entries, char *key) {
    ssize_t hash = hash_function(key) % MAX_SIZE;
    long entry = entries[hash];

    // if entry was not found
    if (entry == -1) {
        printf("ERROR\n");
        return;
    }

    struct Entry temp;
    FILE *current = hashmap_open(hash % 10, "rb+");
    for (; entry > -1; entry = temp.next) {
        fseek(current, entry, SEEK_SET);
        fread(&temp, sizeof(struct Entry), 1, current);
        fseek(current, entry, SEEK_SET);

        if (temp.status != ERASED) {
            int result = strcmp(key, temp.key);

            // if key was found
            if (result == 0) {
                temp.status = ERASED;
                fwrite(&temp, sizeof(struct Entry), 1, current);
                break;
            }
        }
    }
    if (entry == -1) printf("ERROR\n");
    fclose(current);
}

void hashmap_update(long *entries, char *key, char *value) {
    ssize_t hash = hash_function(key) % MAX_SIZE;
    long entry = entries[hash];

    // if entry was not found
    if (entry == -1) {
        printf("ERROR\n");
        return;
    }

    struct Entry temp;
    FILE *current = hashmap_open(hash % 10, "rb+");
    for (; entry > -1; entry = temp.next) {
        fseek(current, entry, SEEK_SET);
        fread(&temp, sizeof(struct Entry), 1, current);
        fseek(current, entry, SEEK_SET);

        if (temp.status != ERASED) {
            int result = strcmp(key, temp.key);

            // if key was found
            if (result == 0) {
                strcpy(temp.value, value);
                fwrite(&temp, sizeof(struct Entry), 1, current);
                break;
            }
        }
    }
    if (entry == -1) printf("ERROR\n");
    fclose(current);
}

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);

    for (int i = 0; i < 10; i++) {
        FILE *temp = hashmap_open(i, "wb");
        fclose(temp);
    }

    long *entries = calloc(MAX_SIZE, sizeof(long));
    for (size_t i = 0; i < MAX_SIZE; i++) {
        entries[i] = -1;
    }

    char key[MAX_LENGHT], value[MAX_LENGHT];

    char command[MAX_LENGHT];
    for (size_t i = 0; i < N; i++) {
        scanf("%s", command);
        if (strcmp(command, "ADD") == 0) {
            scanf("%s", key);
            scanf("%s", value);
            hashmap_add(entries, key, value);
        } else if (strcmp(command, "PRINT") == 0) {
            scanf("%s", key);
            hashmap_print(entries, key);
        } else if (strcmp(command, "DELETE") == 0) {
            scanf("%s", key);
            hashmap_delete(entries, key);
        } else if (strcmp(command, "UPDATE") == 0) {
            scanf("%s", key);
            scanf("%s", value);
            hashmap_update(entries, key, value);
        }
    }

    free(entries);

    return 0;
}