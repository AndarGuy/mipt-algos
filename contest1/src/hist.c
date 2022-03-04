/*

Последовательность строк

Над прямой X расположено несколько столбиков из звёздочек. Все столбики
находятся рядом, количество звёздочек в них неотрицательно и все звёздочки, если
они есть, начинаются сразу над прямой.

Всё это описывается следующим образом. В первой строке — число столбиков 1 ⩽ N ⩽
1000. Остальные N чисел располагаются в следующей строке и обозначают высоты
столбиков. Высоты столбиков не превосходят 1000.

Например, входные данные из первого примера выглядят так:

*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stack {
    size_t top;
    size_t bottom;
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
 * @brief Печать стека.
 *
 * @param st Указатель на структуру.
 * @param pf Указатель на пользовательскую функцию, которая должна принимать
 * указатель на элемент данных, располагающийся в стеке, и вывести его в
 * соответствующем виде на стандартный вывод.
 */
void stack_print(struct stack const *st, void (*pf)(void const *st));

/**
 * @brief Удалить объект st и вернуть NULL.
 *
 * @param st Указатель на структуру.
 * @return struct stack* NULL
 */
struct stack *stack_delete(struct stack *st);

struct stack *stack_new(size_t elem_size) {
    assert(elem_size);
    const int INITIAL_SIZE = 1;
    struct stack *st = malloc(sizeof(struct stack));
    st->capacity = INITIAL_SIZE;
    st->top = 0;
    st->bottom = 0;
    st->elem_size = elem_size;
    st->elems = calloc(INITIAL_SIZE, elem_size);
    return st;
}

int stack_push(struct stack *st, const void *elem) {
    assert(st);
    assert(elem);
    if (st->top >= st->capacity) {
        void *pointer = realloc(st->elems, st->elem_size * (st->capacity *= 2));
        if (!pointer) return 1;
        st->elems = pointer;
    }
    memcpy(st->elems + st->top * st->elem_size, elem, st->elem_size);
    st->top++;
    return 0;
}

int stack_pop(struct stack *st, void *elem) {
    assert(st);
    assert(elem);
    if (stack_empty(st)) return 1;
    stack_top(st, elem);
    st->top--;
    return 0;
}

int stack_top(struct stack *st, void *elem) {
    assert(st);
    assert(elem);
    if (stack_empty(st)) return 1;
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

void stack_print(struct stack const *st, void (*pf)(void const *st)) {
    assert(st);
    assert(pf);
    printf("[");
    for (size_t i = 0; i < st->top; i++) {
        pf(st->elems + i * st->elem_size);
        if (i != st->top - 1) printf(", ");
    }
    printf("]\n");
}

struct column {
    int index;
    int height;
};

int main(int argc, char *argv[]) {
    int N;
    scanf("%d", &N);

    struct stack *s = stack_new(sizeof(struct column));

    struct column barrier = (struct column){0, -1};
    stack_push(s, &barrier);

    int max_square = 0, height = 0;
    for (size_t i = 1; i <= N + 1; i++) {
        if (i <= N)
            scanf("%d", &height);
        else
            height = 0;

        int index = i;
        struct column top, popped;
        stack_top(s, &top);
        while (height <= top.height) {
            stack_top(s, &top);
            stack_pop(s, &popped);
            index = top.index;
            int square = top.height * (i - top.index);
            max_square = square > max_square ? square : max_square;
            stack_top(s, &top);
        }

        struct column new = (struct column){index, height};
        stack_push(s, &new);
    }

    printf("%d", max_square);

    return 0;
}

/**
 * @complexity O(N)
 *
 * @memory 4 KB
 */