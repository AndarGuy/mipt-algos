/*
*Вектора*

Реализуйте структуру данных struct vector и методы работы с ней.

Вектор способен содержать произвольное количество элементов любого типа. Главное
ограничение — все элементы должны иметь единый размер.
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct vector {
    size_t size;
    size_t capacity;
    size_t elem_size;
    void *elems;
};

/**
 * @brief Создать новый вектор, который будет содержать elems элементов размера
 * elem_size.
 *
 * @param elems Вместимость структуры
 * @param elem_size Резмер хранимого в структуре элемента
 * @return struct vector* Указатель на структуру
 */
struct vector *vector_new(size_t elems, size_t elem_size);

/**
 * @brief Возвратить количество элементов, которым владеет вектор v.
 *
 * @param v Указатель на структуру
 * @return size_t
 */
size_t vector_size(struct vector const *v);

/**
 * @brief Добавить ещё один элемент elem к вектору v.
 *
 * @complexity Амортизированная сложность этой операции должна быть O(1).
 *
 * @param v Указатель на структуру.
 * @param elem Указатель на данные для хранения.
 * @return int Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода.
 */
int vector_push(struct vector *v, const void *elem);

/**
 * @brief Убрать из вектора v последний элемент, если он существует, и положить
 * его значение по адресу elem.
 *
 * @complexity Амортизированная сложность этой операции должна быть O(1).
 *
 * @param v Указатель на структуру.
 * @param elem Указатель на данные для хранения.
 * @return int Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода.
 */
int vector_pop(struct vector *v, void *elem);

/**
 * @brief Присвоить elem значение элемента index вектора v.
 *
 * @complexity Амортизированная сложность этой операции должна быть O(1).
 *
 * @param v Указатель на структуру.
 * @param elem Указатель на данные для хранения.
 * @return int Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода.
 */
int vector_get(struct vector const *v, size_t index, void *elem);

/**
 * @brief Присвоить элементу index вектора v значение elem.
 *
 * @complexity Амортизированная сложность этой операции должна быть O(1).
 *
 * @param v Указатель на структуру.
 * @param elem Указатель на данные для хранения.
 * @return int Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода.
 */
int vector_set(struct vector *v, size_t index, void const *elem);

/**
 * @brief Изменить размер вектора v на new_size. Значение старых элементов
 * вектора не должны измениться. Остальные элементы остаются
 * неинициализированными.
 *
 * @param v Указатель на структуру.
 * @param new_size Новый размер структуры
 * @return int Функция должна вернуть 0 при успехе и 1 при неуспехе любого рода.
 */
int vector_resize(struct vector *v, size_t new_size);

/**
 * @brief Функция-предикат.
 *
 * @param v Указатель на структуру.
 * @return int Она должна вернуть логическую истину (1), если структура пуста и
 * логический ноль (0) в противном случае.
 */
int vector_empty(struct vector const *v);

/**
 * @brief Печать структуры.
 *
 * @param v Указатель на структуру.
 * @param pf Указатель на пользовательскую функцию, которая должна принимать
 * указатель на элемент данных, располагающийся в стеке, и вывести его в
 * соответствующем виде на стандартный вывод.
 */
void vector_print(struct vector const *v, void (*pf)(void const *v));

/**
 * @brief Удалить объект и вернуть NULL.
 *
 * @param v Указатель на структуру.
 * @return struct vector* NULL
 */
struct vector *vector_delete(struct vector *v);