/*

ADVANCED QUEUE

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _queue {
    int front, rear, size;
    size_t capacity, data_size;
    void *array;
} * Queue;

Queue queue_create(size_t data_size) {
    Queue queue = malloc(sizeof(struct _queue));
    queue->capacity = 1;
    queue->front = queue->size = 0;

    queue->rear = queue->capacity - 1;
    queue->data_size = data_size;
    queue->array = malloc(queue->capacity * queue->data_size);
    return queue;
}

int queue_full(Queue queue) { return (queue->size == queue->capacity); }

int queue_empty(Queue queue) { return (queue->size == 0); }

void queue_expand(Queue queue) {
    queue->array =
        realloc(queue->array, queue->capacity * queue->data_size * 2);
    if (queue->rear < queue->front) {
        memcpy(queue->array + queue->capacity * queue->data_size, queue->array,
               (queue->rear % queue->capacity + 1) * queue->data_size);
        queue->rear = queue->front + queue->size - 1;
    }
    queue->capacity = queue->capacity * 2;
}

void enqueue(Queue queue, void *item) {
    if (queue_full(queue)) queue_expand(queue);
    queue->rear = (queue->rear + 1) % queue->capacity;
    memcpy(queue->array + queue->rear * queue->data_size, item,
           queue->data_size);
    queue->size = queue->size + 1;
}

void *dequeue(Queue queue) {
    if (queue_empty(queue)) return NULL;
    void *item = queue->array + queue->front * queue->data_size;
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

void *queue_front(Queue queue) {
    if (queue_empty(queue)) return NULL;
    return queue->array + queue->front * queue->data_size;
}

void *queue_rear(Queue queue) {
    if (queue_empty(queue)) return NULL;
    return queue->array + queue->rear * queue->data_size;
}

Queue queue_destroy(Queue queue) {
    free(queue->array);
    free(queue);
    return NULL;
}

void queue_print(Queue queue, void (*print_data)(void const *data)) {
    printf("[");
    for (size_t i = 0; i < queue->size; i++) {
        print_data(queue->array +
                   ((queue->front + i) % queue->capacity) * queue->data_size);
        if (i + 1 < queue->size) printf(", ");
    }
    printf("]");
    printf(" ");
    printf(" {%d/%zu, %d:%d}\n", queue->size, queue->capacity, queue->front,
           queue->rear);
}

int main() {
    Queue queue = queue_create(sizeof(size_t));
    size_t temp;
    temp = 10;
    enqueue(queue, &temp);
    temp = 20;
    enqueue(queue, &temp);
    printf("%zu dequeued from queue\n", *((size_t *)dequeue(queue)));
    printf("%zu dequeued from queue\n", *((size_t *)dequeue(queue)));
    temp = 30;
    enqueue(queue, &temp);
    temp = 40;
    enqueue(queue, &temp);
    temp = 50;
    enqueue(queue, &temp);
    temp = 60;
    enqueue(queue, &temp);
    printf("%d %d %d %zu\n", queue->front, queue->rear, queue->size,
           queue->capacity);

    printf("Front item is %zu\n", *((size_t *)queue_front(queue)));
    printf("Rear item is %zu\n", *((size_t *)queue_rear(queue)));

    return 0;
}