#pragma once

#include <pthread.h>
#include <string.h>
#include "packaged_task.h"
#include "utils.h"
#include "queue_impl.h"
#include "cyclic_buffer.h"

typedef struct {
    int capacity;
    int is_open;
    pthread_mutex_t mutex;
    pthread_cond_t cv_put, cv_get;
    CyclicBuffer q;
} blocking_queue;

void blocking_queue_init(blocking_queue* queue, int capacity);

int blocking_queue_put(blocking_queue* queue, void* item);

int blocking_queue_get(blocking_queue* queue, void* item);

// Как TryPop
int blocking_queue_try_get(blocking_queue* queue, void* v);

void blocking_queue_shutdown(blocking_queue* queue);

void blocking_queue_destroy(blocking_queue* queue);

size_t blocking_queue_size(blocking_queue* queue);
