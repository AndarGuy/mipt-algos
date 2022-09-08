/*

Конкатенация и поворот

Неформально определим операции конкатенации · и поворота R. Конкатенация
склеивает два слова в одно: для слов u = ab и v = abb их конкатенация есть u · v
= uv = ababb. Операция поворота переставляет буквы слова в обратном порядке: uR
= ba, vR = bba, (uv)R = bbaba. Заметим, что для любых u и v, (uv)R = (vR)(uR).

На вход задачи подаётся дерево, каждый узел которого является операцией
конкатенации. К некоторым узлам применена операция поворота — слово, которое
окажется вычисленным в узле необходимо повернуть, прежде чем продолжать
вычисление. Необходимо вычислить получившееся в корне слово.

*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct stack {
    size_t top;
    size_t bottom;
    size_t capacity;
    size_t elem_size;
    void *elems;
} * Stack;

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

// ---------------------------------------------------------- //

int main(int argc, char *argv[]) {
    struct string {
        char *ptr;
        unsigned int length;
        char reversed;
    };

    struct string open = (struct string){NULL, 0, 0};
    struct string close = (struct string){NULL, 1, 0};

    Stack stack = stack_new(sizeof(struct string));
    Stack storage = stack_new(sizeof(struct string));

    char *buffer = malloc(10000 * sizeof(char));
    size_t buffer_size = 0;
    char current;
    while ((current = getchar()) != 0 && current != '\n') {
        if (current == '(') {
            stack_push(stack, &open);
            buffer_size = 0;
        } else if (current == ')') {
            if (buffer_size) {
                char *temp = malloc(sizeof(char) * buffer_size + 1);
                memcpy(temp, buffer, buffer_size);
                temp[buffer_size] = 0;
                struct string s = (struct string){temp, buffer_size, 0};
                stack_push(stack, &s);
            }
            buffer_size = 0;
            stack_push(stack, &close);
        } else if (current == 'R') {
            int counter = 0;
            struct string s;
            stack_pop(stack, &s);
            if (s.ptr == NULL && s.length == close.length) {
                counter++;
                stack_push(storage, &open);
            } else {
                stack_push(stack, &close);
            }
            while (counter > 0) {
                stack_pop(stack, &s);
                s.reversed = !s.reversed;
                if (s.ptr == NULL && s.length == close.length) {
                    counter++;
                    stack_push(storage, &open);
                } else if (s.ptr == NULL && s.length == open.length) {
                    counter--;
                    stack_push(storage, &close);
                } else
                    stack_push(storage, &s);
            }
            memcpy(stack->elems + stack->elem_size * stack->top, storage->elems,
                   storage->top * storage->elem_size);
            stack->top += storage->top;
            storage->top = 0;
        } else {
            buffer[buffer_size] = current;
            buffer_size++;
        }
    }

    struct string temp;
    while (!stack_empty(stack)) {
        stack_pop(stack, &temp);
        stack_push(storage, &temp);
    }

    while (!stack_empty(storage)) {
        stack_pop(storage, &temp);
        if (temp.ptr != NULL) {
            if (temp.reversed) {
                for (size_t i = 0; i < temp.length; i++) {
                    printf("%c", temp.ptr[temp.length - i - 1]);
                }
            } else {
                printf("%s", temp.ptr);
            }
            free(temp.ptr);
        }
    }

    stack_delete(storage);
    stack_delete(stack);
    free(buffer);

    return 0;
}