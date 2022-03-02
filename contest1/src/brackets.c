/*

Скобочки

Правильной скобочной последовательность назовём такую последовательность
символов, которая сохраняет баланс открывающих и закрывающих скобок.

В нашей задаче парами скобок являются: { }, [ ], ( ), < >.

Нужно проверить много строк на предмет того, является ли данная строка
правильной скобочной последовательностью. Например, строка #include <stdio.h>
правильной скобочной последовательностью является, а строка x = y[10] + (a < b)
не является.

*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 1

struct stack {
    size_t top;
    size_t capacity;
    size_t elem_size;
    void *elems;
};

/**
 * @brief Создать новый стек, который будет хранить элементы размера elem_size и
 * вернуть указатель на него.
 *
 * @param elem_size Резмер хранимого в стеке элемента
 * @return struct stack* Указатель на структуру
 */
struct stack *stack_new(size_t elem_size);

/**
 * @brief Добавить в стек st элемент elem.
 *
 * @complexity Амортизированная сложность этой операции должна быть O(1).
 *
 * @param st Указатель на структуру.
 * @param elem Указатель на данные для хранения.
 * @return int Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода.
 */
int stack_push(struct stack *st, const void *elem);

/**
 * @brief Извлечь из стека st элемент elem и продвинуть стек вниз.
 *
 * @complexity Амортизированная сложность этой операции должна быть O(1).
 *
 * @param st Указатель на структуру.
 * @param elem Указатель на данные для хранения.
 * @return int Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода.
 */
int stack_pop(struct stack *st, void *elem);

/**
 * @brief Извлечь из стека st элемент elem и оставить стек в предыдущем
 * состоянии.
 *
 * @complexity Амортизированная сложность этой операции должна быть O(1).
 *
 * @param st Указатель на структуру.
 * @param elem Указатель на данные для хранения.
 * @return int Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода.
 */
int stack_top(struct stack *st, void *elem);

/**
 * @brief Функция-предикат.
 *
 * @param st Указатель на структуру.
 * @return int Она должна вернуть логическую истину (1), если стек пуст и
 * логический ноль (0) в противном случае.
 */
int stack_empty(struct stack const *st);

/**
 * @brief Удалить объект st и вернуть NULL.
 *
 * @param st Указатель на структуру.
 * @return struct stack* NULL
 */
struct stack *stack_delete(struct stack *st);

struct stack *stack_new(size_t elem_size) {
    assert(elem_size);
    struct stack *st = malloc(sizeof(struct stack));
    st->capacity = INITIAL_SIZE;
    st->top = 0;
    st->elem_size = elem_size;
    st->elems = calloc(INITIAL_SIZE, elem_size);
    return st;
}

int stack_push(struct stack *st, const void *elem) {
    assert(st);
    assert(elem);
    if (st->top >= st->capacity) {
        st->capacity *= 2;
        void *pointer = realloc(st->elems, st->elem_size * st->capacity);
        if (!pointer) {
            return 1;
        }
        st->elems = pointer;
    }
    memcpy(st->elems + st->top * st->elem_size, elem, st->elem_size);
    st->top++;
    return 0;
}

int stack_pop(struct stack *st, void *elem) {
    assert(st);
    assert(elem);
    if (stack_empty(st)) {
        return 1;
    }
    stack_top(st, elem);
    st->top--;
    return 0;
}

int stack_top(struct stack *st, void *elem) {
    assert(st);
    assert(elem);
    if (stack_empty(st)) {
        return 1;
    }
    memcpy(elem, st->elems + (st->top - 1) * st->elem_size, st->elem_size);
    return 0;
}

int stack_empty(struct stack const *st) {
    assert(st);
    return st->top == 0;
}

struct stack *stack_delete(struct stack *st) {
    assert(st);
    free(st->elems);
    free(st);
    return NULL;
}

char get_pair(char bracket) {
    switch (bracket) {
        case '}':
            return '{';
        case ')':
            return '(';
        case '>':
            return '<';
        case ']':
            return '[';
    }
    return '\0';
}

int main(int argc, char *argv[]) {
    struct stack *brackets = stack_new(sizeof(char));
    char bracket = 0;
    while ((bracket = getc(stdin)) != -1) {
        if (bracket == '{' || bracket == '(' || bracket == '<' ||
            bracket == '[') {
            stack_push(brackets, &bracket);
        } else {
            char pair = get_pair(bracket);
            if (pair == '\0') {
                continue;
            }
            if (brackets->top == 0) {
                printf("NO");
                return 0;
            }
            char actual = '\0';
            stack_pop(brackets, &actual);
            if (pair != actual) {
                printf("NO");
                return 0;
            }
        }
    }

    if (brackets->top == 0) {
        printf("YES");
    } else {
        printf("NO");
    }

    brackets = stack_delete(brackets);

    return 0;
}

/**
 * @complexity O(N)
 *
 * @memory N
 */