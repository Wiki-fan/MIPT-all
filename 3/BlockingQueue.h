#pragma once

#include <pthread.h>

#include "packaged_task.h"

#define Value packaged_task

typedef struct {
    int capacity;
    int is_open;
    pthread_mutex_t mutex;
    pthread_cond_t cv_put, cv_get;
    Queue_packaged_task queue;
} blocking_queue;

void blocking_queue_init(blocking_queue* queue, int capacity) {
    queue->is_open = 1;
    queue->capacity = capacity;
    Queue_packaged_task_init(&queue->queue);
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cv_get, NULL);
    pthread_cond_init(&queue->cv_put, NULL);
}

int blocking_queue_put(blocking_queue* queue, Value* item) {
    pthread_mutex_lock(&queue->mutex);

    while (queue->queue.size == queue->capacity && queue->is_open) {
        pthread_cond_wait(&queue->cv_put, &queue->mutex);
    }
    if (!queue->is_open) {
        return 0;
    }

    Queue_packaged_task_push(&queue->queue, item);
    pthread_cond_signal(&queue->cv_get);

    pthread_mutex_unlock(&queue->mutex);
    return 1;
}

int blocking_queue_get(blocking_queue* queue, Value** item) {
    pthread_mutex_lock(&queue->mutex);

    while (queue->queue.size == 0 && queue->is_open) {
        pthread_cond_wait(&queue->cv_get, &queue->mutex);
    }

    if (!queue->is_open) {
        if (queue->queue.size == 0) {
            return 0;
        }
    }

    *item = Queue_packaged_task_pop(&queue->queue);

    pthread_cond_signal(&queue->cv_put);

    pthread_mutex_unlock(&queue->mutex);
    return 1;
}

// Как TryPop
int blocking_queue_try_get(blocking_queue* queue, Value** v) {
    pthread_mutex_lock(&queue->mutex);

    if (queue->queue.size == 0) {
        return 0;
    }

    *v = Queue_packaged_task_pop(&queue->queue);

    pthread_cond_signal(&queue->cv_put);

    pthread_mutex_unlock(&queue->mutex);
    return 1;
}

void blocking_queue_shutdown(blocking_queue* queue) {
    pthread_mutex_lock(&queue->mutex);

    // Чтобы не вызывать лишний раз broadcast, проверяем, не была ли очередь уже закрыта.
    if (queue->is_open) {
        queue->is_open = 0;
        pthread_cond_broadcast(&queue->cv_put);
        pthread_cond_broadcast(&queue->cv_get);

        Queue_packaged_task_destroy(&queue->queue);
        pthread_mutex_destroy(&queue->mutex);
        pthread_cond_destroy(&queue->cv_get);
        pthread_cond_destroy(&queue->cv_put);
    }
    pthread_mutex_unlock(&queue->mutex);
}
