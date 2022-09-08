/*
*Односвязные списки*

Односвязные списки — одна из базовых структур данных, применяемых в системном программировании. Их аккуратная и надёжная реализация – основа многих других алгоритмов.

Вам предлагается реализовать такую структуру данных и методы работы с ней.

Мы будем считать, что каждый список определяется своей головой и может содержать произвольное число элементов. Пустой список в качестве головы имеет NULL.

Вы должны реализовать структуру struct list как вам заблагорассудится. Однако, вы обязаны реализовать все интерфейсные функции.

После операций вставок в список и удаления из списка голова списка может измениться, поэтому все функции, способные изменить список, возвращают новую голову.
*/

/**
 * @brief Создать новый узел со значением elem и вернуть указатель на него.
 * 
 * @param elem Элемент
 * @return struct set* Указатель на структуру
 */
struct list *list_new(int elem); 

/**
 * @brief Вставить в конец списка с головой head узел со значением elem. Если голова списка пуста — создать новый список.
 * 
 * @param head Указатель на структуру
 * @param elem Элемент, который необходимо добавить в структуру
 * @return struct list * Вернуть голову нового списка.
 */
struct list *list_insert(struct list *head, int elem); 

/**
 * @brief Поиск элемента в структуре
 * 
 * @param s Указатель на структуру
 * @param elem Элемент, который необходимо найти в структуре
 * @return struct list * вернуть указатель на него. Если таких значений в списке нет, вернуть NULL.
 */
struct list *list_find(struct list *head, int elem); 

/**
 * @brief Найти узел, начиная с головы списка, в котором значение будет равно elem и удалить его из списка.
 * 
 * @param head 
 * @param elem 
 * @return struct list* Вернуть указатель на голову вновь получившегося списка.
 */
struct list *list_erase(struct list *head, int elem); 

// Вставить узел what после узла where в списке, готова которого находится в head. Вернуть указатель на голову нового списка. Вставка в пустой список приводит к созданию непустого. Вставка NULL или после NULL есть пустая операция.
struct list *list_insert_after(struct list *head, struct list *where, struct list *what); 

// Вставить узел what перед узлом where в списке, готова которого находится в head. Вернуть указатель на голову нового списка. Вставка в пустой список приводит к созданию непустого. Вставка NULL или после NULL есть пустая операция.
struct list *list_insert_before(struct list *head, struct list *where, struct list *what); 

// Удалить весь список, начинающийся с head. Вернуть NULL.
struct list *list_delete(struct list *head); 

// Вернуть указатель на следующий элемент списка. Если curr пустой — вернуть NULL.
struct list *list_next(struct list *curr); 

// Вывести на стандартный вывод содержимое списка в формате по образцу: []\n или [1, 2, 3]\n.
void list_print(struct list const *head); 

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct list {
    struct list *next;
    int value;
};

struct list *list_new(int elem) {
    struct list *l = malloc(sizeof(struct list));
    l->next = NULL;
    l->value = elem;
    return l;
}

struct list *list_insert(struct list *head, int elem) {
    if (head == NULL) return list_new(elem);

    struct list *current = head;

    while (current->next != NULL) current = current->next;

    current->next = list_new(elem);

    return head;
}

struct list *list_find(struct list *head, int elem) {
    if (head == NULL || head->value == elem) return head;
    struct list *current = head;
    
    while (current != NULL) {
        if (current->value == elem) return current;
        current = current->next;
    }

    return NULL;
}

struct list *list_erase(struct list *head, int elem) {
    if (head == NULL) return head;
    if (head->value == elem) {
        struct list *delete = head;
        head = head->next;
        free(delete);
        return head;
    }

    struct list *current = head;
    struct list *next = NULL;
    while (current != NULL) {
        next = current->next;
        if (next == NULL) break;
        if (next->value == elem) {
            struct list *delete = current->next;
            current->next = next->next;
            free(delete);
            break;
        }
        current = next;
    }

    return head;
}

struct list *list_insert_after(struct list *head, struct list *where, struct list *what) {
    if (head == NULL) return what;
    if (where == NULL || what == NULL) return head;
    struct list *current = head;
	while (current != NULL && current != where)
		current = current->next;
	if (current == NULL)
		return head;
    what->next = current->next;
	current->next = what;
	return head;
}

struct list *list_insert_before(struct list *head, struct list *where, struct list *what) {
    if (head == NULL) return what;
    if (where == NULL || what == NULL) return head;
    if (head == where) {
		what->next = head;
		return what;
    }
    struct list *current = head;
    while (current != NULL && current->next != where) current = current->next;
    if (current == NULL) return head;
    what->next = current->next;
    current->next = what;
    return head;
}

struct list *list_delete(struct list *head) {
    struct list *current = head;
    struct list *next = NULL;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    return NULL;
}

struct list *list_next(struct list *curr) {
    return curr != NULL ? curr->next : NULL;
}

void list_print(struct list const *head) {
    struct list *current = (struct list*) *(&head);
    struct list *next = NULL;

    printf("[");
    while (current != NULL) {
        next = current->next;
        if (next == NULL) printf("%d", current->value);
        else printf("%d, ", current->value);
        current = next;
    }
    printf("]\n");
}

int main() {
    struct list *head = NULL;
    head = list_insert(head, 1);
    head = list_insert(head, 2);
    head = list_insert(head, 3);
    head = list_insert(head, 4);
    head = list_insert(head, 5);
    head = list_insert(head, 6);
    list_print(head);
    struct list *a = list_find(head, 3);
    struct list *b = list_find(head, 6);
    printf("%d\n", a->value);
    head = list_insert_after(head, a, list_new(10));
    list_print(head);
    head = list_insert_before(head, a, list_new(10));
    list_print(head);
    head = list_insert_before(head, head, list_new(10));
    list_print(head);
    head = list_insert_after(head, b, list_new(10));
    list_print(head);

    printf("%p\n", list_find(head, 128));

    head = list_erase(head, 10);
    head = list_erase(head, 10);
    head = list_erase(head, 10);
    head = list_erase(head, 10);

    printf("%p\n", list_find(head, 6));

    head = list_erase(head, 1);
    head = list_erase(head, 6);
    head = list_erase(head, 2);
    head = list_erase(head, 5);
    head = list_erase(head, 3);
    head = list_erase(head, 4);

    list_print(head);

    head = list_delete(head);
    list_print(head);

    return 0;
}