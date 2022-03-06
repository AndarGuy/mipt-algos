#include "simpleset.h"

struct set {
    char *array;
    size_t capacity;
};

struct set *set_new(size_t capacity) {
    struct set *s = (struct set *)malloc(sizeof(struct set));
    s->capacity = capacity;
    s->array = (char *)calloc(s->capacity / 8 + 1, sizeof(char));
    return s;
}

int set_insert(struct set *s, size_t elem) {
    if (elem >= s->capacity) return 1;
    s->array[elem / 8] |= (1 << (elem % 8));
    return 0;
}

int set_erase(struct set *s, size_t elem) {
    if (elem >= s->capacity) return 1;
    s->array[elem / 8] &= (unsigned char)~((char)(1 << (elem % 8)));
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

int set_empty(struct set const *s) { return set_findfirst(s, 0) != -1; }

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
        if (next != -1)
            printf("%zu, ", elem);
        else
            printf("%zu", elem);
        elem = next;
    }

    printf("]\n");
}

// int main() {
//     struct set *s = set_new(10);
//     assert(set_insert(s, 1) == 0);
//     assert(set_insert(s, 2) == 0);
//     assert(set_insert(s, 100) != 0);
//     assert(set_size(s) == 2);
//     set_print(s);
//     assert(set_find(s, 1) != 0);
//     assert(set_find(s, 100) == 0);
//     assert(set_insert(s, 5) == 0);
//     assert(set_erase(s, 2) == 0);
//     assert(set_findfirst(s, 2) == 5);
//     assert(set_findfirst(s, 10) == -1);
//     assert(set_delete(s) == NULL);

//     return 0;
// }