/*

*Простые множества*

Простые множества могут содержать в себе элементы перечислимых целых значений заданного диапазона. Пример: [0..59] или [-128..127]. Их удобно и часто эффективно реализовывать в виде набора битов с использованием побитовых операций.

Вам предлагается реализовать такую структуру данных и методы работы с ней.

Для простоты значения элементов множества будут начинаться с нуля и количество элементов в них будет фиксированным.

Вы должны реализовать структуру struct set и все интерфейсные функции.

*/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/**
 * @brief Создать новое пустое множество, значения элементов которого могут лежать в границах от 0 до capacity-1 включительно. Вернуть указатель на него.
 * 
 * @param capacity Максимальная вместимость структуры
 * @return struct set* Указатель на структуру
 */
struct set *set_new(size_t capacity); 

/**
 * @brief Добавить во множество s элемент elem. Если элемент существовал, множество не изменится.
 * 
 * @param s Указатель на структуру
 * @param elem Элемент, который необходимо добавить в структуру
 * @return int Вернуть 0, если операция корректна, т.е. elem < capacity, иначе вернуть 1.
 */
int set_insert(struct set *s, size_t elem); 

/**
 * @brief Удалить элемент из множества. Если элемент не существовал, множество не изменится.
 * 
 * @param s Указатель на структуру
 * @param elem Элемент, который необходимо удалить из структуры
 * @return int Вернуть 0, если операция корректна, т.е. elem < capacity, иначе вернуть 1. 
 */
int set_erase(struct set *s, size_t elem); 

/**
 * @brief Поиск элемента в структуре
 * 
 * @param s Указатель на структуру
 * @param elem Элемент, который необходимо найти в структуре
 * @return int Вернуть 1, если элемент присутствует в множестве и 0 в противном случае. 
 */
int set_find(struct set const *s, size_t elem); 

/**
 * @brief Удалить объект множество и вернуть NULL 
 * 
 * @param s Указатель на структуру
 * @return struct set* NULL
 */
struct set *set_delete(struct set *s); 

/**
 * @brief Предикат: пустая ли структура
 * 
 * @param s Указатель на структуру
 * @return int Вернуть единицу, если в множестве есть хотя бы один элемент и ноль в противном случае. 
 */
int set_empty(struct set const *s); 

/**
 * @brief Получить наименьший элемент, не меньший заданного числа
 * 
 * @param s Указатель на структуру
 * @param start Начальный элемент
 * @return ssize_t Вернуть наименьший из элементов множества, не меньших start. Если таких элементов нет (например, множество пустое), то вернуть -1. 
 */
ssize_t set_findfirst(struct set const *s, size_t start); 

/**
 * @brief Размер структуры
 * 
 * @param s Указатель на структуру
 * @return size_t Вернуть количество элементов множества. 
 */
size_t set_size(struct set const *s); 

/**
 * @brief Вывести на стандартный вывод содержимое множества в формате по образцу: []\n или [1, 2, 3]\n.
 * 
 * @param s Указатель на структуру
 */
void set_print(struct set const *s); 

struct set {
    char* array;
    size_t capacity;
};

struct set *set_new(size_t capacity) {
    struct set *s = (struct set*) malloc(sizeof(struct set));
    s->capacity = capacity;
    s->array = (char*) calloc(s->capacity / 8 + 1, sizeof(char));
    return s;
}

int set_insert(struct set *s, size_t elem) {
    if (elem >= s->capacity) return 1;
    s->array[elem / 8] |= (1 << (elem % 8));
    return 0;
}

int set_erase(struct set *s, size_t elem) {
    if (elem >= s->capacity) return 1;
    s->array[elem / 8] &= (unsigned char) ~((char) (1 << (elem % 8)));
    return 0;
}

int set_find(struct set const *s, size_t elem) {
    return s->array[elem / 8] & (1 << (elem % 8));
}

struct set *set_delete(struct set *s) {
    free(s->array);
    free(s);
    return NULL;
}

int set_empty(struct set const *s) {
    return set_findfirst(s, 0) != -1;
}

ssize_t set_findfirst(struct set const *s, size_t start) {
    for (int i = start; i < s->capacity; i++) {
        if (set_find(s, i)) return i;
    }
    return -1;
}

size_t set_size(struct set const *s) {
    size_t size = 0;
    for (int i = 0; i < s->capacity; i++) {
        if (set_find(s, i)) size++;
    }

    return size;
}

void set_print(struct set const *s) {
    printf("[");

    size_t elem = set_findfirst(s, 0), next = 0;

    while (next != -1 && elem != -1) {
        size_t next = set_findfirst(s, elem + 1);
        if (next != -1) printf("%zu, ", elem);
        else printf("%zu", elem);
        elem = next;
    }

    printf("]\n");
}

int main() {
    struct set *s = set_new(10);
    assert(set_insert(s, 1) == 0);
    assert(set_insert(s, 2) == 0);
    assert(set_insert(s, 100) != 0);
    assert(set_size(s) == 2);
    set_print(s);            
    assert(set_find(s, 1) != 0);
    assert(set_find(s, 100) == 0);
    assert(set_insert(s, 5) == 0);
    assert(set_erase(s, 2) == 0);
    assert(set_findfirst(s, 2) == 5);
    assert(set_findfirst(s, 10) == -1);
    assert(set_delete(s) == NULL);

    return 0;
}