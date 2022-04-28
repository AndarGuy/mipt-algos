/* hashtable
Реализуйте структуру данных хеш-таблица. Она должна по заданному ключу находить
значение, которое этому ключу соответствует.

Ключ и значение таблицы — строка из строчных букв латинского алфавита не более
20 символов.

В каждой строке стандартного ввода находится одна из пяти возможных команд:

create # — создать таблицу под номером #. Номер может быть от 0 до 9
включительно. insert # key value — В таблице # ключу key ставится в соответствие
значение value. Для уже существующего ключа значение должно поменяться на новое.
erase # key — убрать запись key из хеш-таблицы #.
find # key — вывести на стандартный вывод значение по ключу или слово MISSING,
если ключ не существует или нет такой хеш-таблицы.
destroy # — уничтожить таблицу #. Обращения к уничтоженной хеш-таблице должны
игнорироваться, за исключением команды find.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct List {
    char *key, *value;
    struct List *next;
} list;

typedef struct Hashtable {
    size_t capacity;
    list **pointers;
} hashtable;

size_t hash(const char *key) {
    size_t res = 0;
    for (size_t i = 0; i < strlen(key); ++i) {
        res += key[i];
    }
    return res;
}

list *list_new(const char *key, const char *value) {
    list *head = calloc(1, sizeof(list));
    strcpy(head->key = calloc(strlen(key), sizeof(char)), key);
    strcpy(head->value = calloc(strlen(value), sizeof(char)), value);
    head->next = NULL;
    return head;
}

list *list_insert(list *head, const char *key, const char *value) {
    if (head == NULL) {
        return list_new(key, value);
    } else {
        list *p = head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = list_new(key, value);
        return head;
    }
}

list *list_find(list *head, const char *key) {
    if (head == NULL) {
        return head;
    } else {
        list *p = head;
        while (p != NULL) {
            if (!strcmp(p->key, key)) {
                return p;
            }
            p = p->next;
        }
    }
    return NULL;
}

list *list_erase(list *head, const char *key) {
    if (head == NULL) {
        return NULL;
    } else if (!strcmp(head->key, key)) {
        list *p = head;
        head = head->next;
        free(p->key);
        free(p->value);
        free(p);
        return head;
    } else {
        list *p = head;
        list *tmp = NULL;
        while (p != NULL && (strcmp(p->key, key) != 0)) {
            tmp = p;
            p = p->next;
        }
        if (p == NULL) {
            return head;
        } else {
            tmp->next = p->next;
            free(p->key);
            free(p->value);
            free(p);
            return head;
        }
    }
}

struct list *list_delete(list *head) {
    while (head != NULL) {
        list *tmp = head->next;
        free(head->key);
        free(head->value);
        free(head);
        head = tmp;
    }
    return NULL;
}

hashtable *hashtable_create(size_t capacity) {
    hashtable *ht = calloc(1, sizeof(hashtable));
    ht->capacity = capacity;
    ht->pointers = calloc(capacity, sizeof(list *));
    for (size_t i = 0; i < capacity; ++i) {
        ht->pointers[i] = NULL;
    }
    return ht;
}

void hashtable_insert(hashtable *ht, const char *key, const char *value) {
    size_t index = hash(key) % ht->capacity;
    if (ht->pointers[index] == NULL) {
        ht->pointers[index] = list_new(key, value);
    } else {
        list *tmp = list_find(ht->pointers[index], key);
        if (tmp != NULL) {
            strcpy(tmp->value, value);
        } else {
            list_insert(ht->pointers[index], key, value);
        }
    }
}

void hashtable_erase(hashtable *ht, const char *key) {
    size_t index = hash(key) % ht->capacity;
    if (ht->pointers[index]) {
        ht->pointers[index] = list_erase(ht->pointers[index], key);
    }
}

char *hashtable_find(hashtable *ht, const char *key) {
    size_t index = hash(key) % ht->capacity;
    if (ht->pointers[index] != NULL) {
        list *tmp = list_find(ht->pointers[index], key);
        if (tmp != NULL) {
            return tmp->value;
        }
    }
    return NULL;
}

hashtable *hashtable_delete(hashtable *ht) {
    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->pointers[i] != NULL) {
            list_delete(ht->pointers[i]);
        }
    }
    free(ht->pointers);
    free(ht);
    return NULL;
}

int main() {
    hashtable **ht = calloc(10, sizeof(hashtable *));
    int flag[10] = {0}, num;
    char command[10] = {0}, key[25] = {0}, value[25] = {0};
    while (scanf("%7s", command) > 0) {
        scanf("%d", &num);
        if (!strcmp(command, "create")) {
            if (ht[num] == NULL) ht[num] = hashtable_create(100000);
        } else if (flag[num]) {
            if (!strcmp(command, "find")) {
                printf("MISSING\n");
            }
            continue;
        } else if (!strcmp(command, "insert")) {
            scanf("%20s %20s", key, value);
            hashtable_insert(ht[num], key, value);
        } else if (!strcmp(command, "erase")) {
            scanf("%20s", key);
            hashtable_erase(ht[num], key);
        } else if (!strcmp(command, "find")) {
            scanf("%20s", key);
            if (hashtable_find(ht[num], key) != NULL) {
                printf("%s\n", hashtable_find(ht[num], key));
            } else {
                printf("MISSING\n");
            }
        } else if (!strcmp(command, "destroy")) {
            if (ht[num] != NULL) ht[num] = hashtable_delete(ht[num]);
        }
    }
    for (int i = 0; i < 10; ++i) {
        if (ht[i] != NULL) {
            ht[i] = hashtable_delete(ht[i]);
        }
    }
    free(ht);
    return 0;
}