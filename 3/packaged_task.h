#pragma once
#include <unitypes.h>
#include <pthread.h>
#include "queue_impl.h"

typedef struct {
    void* (* func_ptr)(void*);
    void* args;
    pthread_cond_t cv;
    int is_ready;
} packaged_task;

define_queue(packaged_task)
