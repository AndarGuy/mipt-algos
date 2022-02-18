/*

*Очереди*

Очереди — одна из базовых структур данных, применяемых в системном программировании. 
Она реализует дисциплину FIFO (First input - First Output).

Вам предлагается реализовать структуру данных struct queue и методы работы с ней.

Очередь, которую мы должны реализовать, должна уметь хранить произвольное количество элементов любого типа. 
Главное ограничение — все элементы должны иметь единый размер.

Вы должны реализовать структуру struct queue как вам заблагорассудится. Вы обязаны реализовать все интерфейсные функции.

*/


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct stack {
    size_t top;
    size_t bottom;
    size_t capacity;
    size_t elem_size;
    void *elems;
};

struct queue {
    struct stack *s;
};

struct stack *stack_new(size_t elem_size); 

int stack_push(struct stack *st, const void *elem);

int stack_pop(struct stack *st, void *elem);

int stack_bottom(struct stack *st, void *elem); 

int stack_empty(struct stack const *st); 

void stack_print(struct stack const *st, void (*pf)(void const *st)); 

struct stack *stack_delete(struct stack *st);

/**
 * @brief Создать новый стек, который будет хранить элементы размера elem_size и вернуть указатель на него.
 * 
 * @param elem_size Резмер хранимого в очереде элемента
 * @return struct queue* Указатель на структуру
 */
struct queue *queue_new(size_t elem_size); 

/**
 * @brief Добавить в очередь q элемент elem.
 * 
 * @complexity Амортизированная сложность этой операции должна быть O(1).
 * 
 * @param q Указатель на структуру.
 * @param elem Указатель на данные для хранения.
 * @return int Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода.
 */
int queue_push(struct queue *q, const void *elem);

/**
 * @brief Извлечь из очереди q элемент elem и продвинуть стек вниз.
 * 
 * @complexity Амортизированная сложность этой операции должна быть O(1).
 * 
 * @param q Указатель на структуру.
 * @param elem Указатель на данные для хранения.
 * @return int Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода.
 */
int queue_pop(struct queue *q, void *elem);

/**
 * @brief Функция-предикат.
 * 
 * @param q Указатель на структуру.
 * @return int Она должна вернуть логическую истину (1), если стек пуст и логический ноль (0) в противном случае.
 */
int queue_empty(struct queue const *q); 

/**
 * @brief Печать очереди.
 * 
 * @param q Указатель на структуру.
 * @param pf Указатель на пользовательскую функцию, которая должна принимать указатель на элемент данных, располагающийся в очереде, и вывести его в соответствующем виде на стандартный вывод.
 */
void queue_print(struct queue const *q, void (*pf)(void const *q)); 

/**
 * @brief Удалить объект q и вернуть NULL.
 * 
 * @param q Указатель на структуру.
 * @return qruct queue* NULL
 */
struct queue *queue_delete(struct queue *q);


struct stack *stack_new(size_t elem_size) {
    const int INITIAL_SIZE = 262144;
    struct stack *st = malloc(sizeof(struct stack));
    st->capacity = INITIAL_SIZE;
    st->top = 0;
    st->bottom = 0;
    st->elem_size = elem_size;
    st->elems = calloc(INITIAL_SIZE, elem_size);
    return st;
}

int stack_push(struct stack *st, const void *elem) {
    if (st->top >= st->capacity) {
        if (st->bottom >= st->capacity / 2) {
            memmove(st->elems, st->elems + st->bottom * st->elem_size, (st->top - st->bottom) * st->elem_size);
            st->top -= st->bottom;
            st->bottom = 0;
        } else st->elems = realloc(st->elems, st->elem_size * (st->capacity *= 4));
    }
    memcpy(st->elems + st->top * st->elem_size, elem, st->elem_size);
    st->top++;
    return 0;
}

int stack_pop(struct stack *st, void *elem) {
    if (stack_empty(st)) return 1;
    stack_bottom(st, elem);
    st->bottom++;
    return 0;
}

int stack_bottom(struct stack *st, void *elem) {
    if (stack_empty(st)) return 1;
    memcpy(elem, st->elems + st->bottom * st->elem_size, st->elem_size);
    return 0;
}

int stack_empty(struct stack const *st) {
    return st->top - st->bottom == 0;
}

struct stack *stack_delete(struct stack *st) {
    free(st->elems);
    free(st);
    return NULL;
}

void stack_print(struct stack const *st, void (*pf)(void const *st)) {
    printf("[");
    for (size_t i = st->bottom; i < st->top; i++) {
        pf(st->elems + (st->top - i - 1 + st->bottom) * st->elem_size);
        if (i != st->top - 1) printf(", ");
    }
    printf("]\n");
}


struct queue *queue_new(size_t elem_size) {
    struct queue *q = malloc(sizeof(struct queue));
    q->s = stack_new(elem_size);
    return q;
}

int queue_push(struct queue *q, const void *elem) {
    return stack_push(q->s, elem);
}

int queue_pop(struct queue *q, void *elem) {
    return stack_pop(q->s, elem);
}

int queue_empty(struct queue const *q) {
    return stack_empty(q->s);
}

struct queue *queue_delete(struct queue *q) {
    stack_delete(q->s);
    free (q);
    return NULL;
}

void queue_print(struct queue const *q, void (*pf)(void const *v)) {
    stack_print(q->s, pf);
}

static void print_double(void const *st) {
   printf("%lf", *(double *)st);
}

int main() {
    struct queue *q = queue_new(sizeof (double));
    for (int i = 0; i < 10; i++) {
        double tmp = i * i;
        queue_push(q, &tmp);
    }
    queue_print(q, print_double);
    while (!queue_empty(q)) {
        double tmp;
        queue_pop(q, &tmp);
        printf("%lf\n", tmp);
    }                                                                                                                                                                                        
 
    q = queue_delete(q);
}
// Будут выведены все числа от 0 до 9 включительно.
            