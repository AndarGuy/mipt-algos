/*

В файле "input.txt" содержатся строки символов, длина каждой строки не превышает
10000 байт. Файл нужно отсортировать в лексикографическом порядке и вывести
результат в файл "output.txt". Вот беда, файл занимает много мегабайт, а в вашем
распоряжение оказывается вычислительная система с памятью всего в 256 килобайт.
Но файл должен быть отсортирован!

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
} Stack;

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

int stack_empty(struct stack const *st) {
    assert(st);
    return st->top == 0;
}

int stack_top(struct stack *st, void *elem) {
    assert(st);
    assert(elem);
    if (stack_empty(st)) return 1;
    memcpy(elem, st->elems + (st->top - 1) * st->elem_size, st->elem_size);
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

struct stack *stack_delete(struct stack *st) {
    assert(st);
    free(st->elems);
    free(st);
    return NULL;
}

// --------------------------------------------------------- //

#define LINE_SIZE 10001

char buffer_a[LINE_SIZE], buffer_b[LINE_SIZE], buffer_fn[64];

typedef struct Chunk {
    size_t depth;
    size_t number;
} Chunk;

int comparator(const void *s1, const void *s2) {
    return strcmp((const char *)s1, (const char *)s2);
}

char *get_storage_filename(Chunk chunk) {
    sprintf(buffer_fn, "chunk(%zu, %zu).txt", chunk.depth, chunk.number);
    return buffer_fn;
}

FILE *get_storage(Chunk chunk, char *mode) {
    FILE *file = fopen(get_storage_filename(chunk), mode);
    return file;
}

Chunk merge(Chunk chunk1, Chunk chunk2) {
    Chunk chunk3;

    if (chunk1.depth < chunk2.depth) {
        chunk3.depth = chunk2.depth;
        chunk3.number = chunk2.number;
    } else if (chunk1.depth > chunk2.depth) {
        chunk3.depth = chunk1.depth;
        chunk3.number = chunk1.number;
    } else {
        chunk3.depth = chunk2.depth + 1;
        if (chunk1.number < chunk2.number) {
            chunk3.number = chunk1.number;
        } else {
            chunk3.number = chunk2.number;
        }
    }

    FILE *storage1 = get_storage(chunk1, "r"),
         *storage2 = get_storage(chunk2, "r"),
         *storage3 = fopen("temp.txt", "w");

    buffer_a[0] = buffer_b[0] = 0;
    while (!feof(storage1) || !feof(storage2) || buffer_a[0] || buffer_b[0]) {
        if (buffer_a[0] == 0 && !feof(storage1))
            fgets(buffer_a, LINE_SIZE, storage1);

        if (buffer_b[0] == 0 && !feof(storage2))
            fgets(buffer_b, LINE_SIZE, storage2);

        if (buffer_a[0] == 0) {
            fputs(buffer_b, storage3);
            buffer_b[0] = 0;
        } else if (buffer_b[0] == 0) {
            fputs(buffer_a, storage3);
            buffer_a[0] = 0;
        } else if (strcmp(buffer_a, buffer_b) > 0) {
            fputs(buffer_b, storage3);
            buffer_b[0] = 0;
        } else {
            fputs(buffer_a, storage3);
            buffer_a[0] = 0;
        }
    }

    fclose(storage1);
    fclose(storage2);
    fclose(storage3);
    remove(get_storage_filename(chunk1));
    remove(get_storage_filename(chunk2));
    rename("temp.txt", get_storage_filename(chunk3));

    return chunk3;
}

Chunk merge_buffer(char *buffer, size_t buffer_size, Chunk chunk) {
    Chunk result = (Chunk){chunk.depth + 1, chunk.number};

    FILE *storage1 = get_storage(chunk, "r"),
         *storage2 = get_storage(result, "w");

    size_t line = 0;
    buffer_a[0] = 0;
    while (!feof(storage1) || buffer_a[0] || line < buffer_size) {
        if (buffer_a[0] == 0 && !feof(storage1))
            fgets(buffer_a, LINE_SIZE, storage1);

        if (buffer_a[0] == 0) {
            fputs(buffer + line * LINE_SIZE, storage2);
            line++;
        } else if (line >= buffer_size) {
            fputs(buffer_a, storage2);
            buffer_a[0] = 0;
        } else if (strcmp(buffer_a, buffer + line * LINE_SIZE) > 0) {
            fputs(buffer + line * LINE_SIZE, storage2);
            line++;
        } else {
            fputs(buffer_a, storage2);
            buffer_a[0] = 0;
        }
    }

    fclose(storage1);
    fclose(storage2);
    remove(get_storage_filename(chunk));

    return result;
}

void extern_sort() {
    FILE *input = fopen("input.txt", "r"), *output = fopen("output.txt", "w");

    const size_t memory_limit = 255 * 1024;
    // const char character_from = '!', character_to = '~';
    const size_t chunk_capacity = memory_limit / LINE_SIZE;

    Stack *stack = stack_new(sizeof(Chunk));
    char *buffer = calloc(memory_limit, sizeof(char));
    for (size_t chunk = 0; !feof(input); chunk++) {
        size_t line;
        for (line = 0; line < chunk_capacity; line++) {
            if (!fgets(buffer + line * LINE_SIZE, LINE_SIZE, input)) break;
        }
        size_t lines = line;
        qsort(buffer, lines, LINE_SIZE * sizeof(char), comparator);
        Chunk new_chunk = (Chunk){0, chunk}, top_chunk;

        stack_top(stack, &top_chunk);

        if (!stack_empty(stack) && new_chunk.depth == top_chunk.depth) {
            stack_pop(stack, &top_chunk);
            new_chunk = merge_buffer(buffer, lines, top_chunk);
            stack_top(stack, &top_chunk);
        } else {
            FILE *temp = get_storage(new_chunk, "w");
            for (line = 0; line < lines; line++) {
                fputs(buffer + line * LINE_SIZE, temp);
            }
            fclose(temp);
        }

        while (!stack_empty(stack) && new_chunk.depth == top_chunk.depth) {
            stack_pop(stack, &top_chunk);
            new_chunk = merge(top_chunk, new_chunk);
            stack_top(stack, &top_chunk);
        }
        stack_push(stack, &new_chunk);
    }

    Chunk new_chunk, top_chunk;
    stack_pop(stack, &new_chunk);
    while (!stack_empty(stack)) {
        stack_pop(stack, &top_chunk);
        new_chunk = merge(top_chunk, new_chunk);
    }

    rename(get_storage_filename(new_chunk), "output.txt");

    free(buffer);
    fclose(input);
    fclose(output);
    stack_delete(stack);
}

int main() {
    extern_sort();

    return 0;
}