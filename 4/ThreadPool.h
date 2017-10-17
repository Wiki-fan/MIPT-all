#pragma once

#include <limits.h>
#include <stdatomic.h>
#include "packaged_task.h"
#include "BlockingQueue.h"

typedef struct {
    size_t num_workers; //atomic
    blocking_queue queue;
    pthread_t* workers; // std::vector <std::thread>
    pthread_mutex_t mutex, mutex_side_task;
} thread_pool;

void* RunWorker(void* arg);

void ThreadPool_init(thread_pool* pool, size_t num_workers);

void ThreadPool_Submit(thread_pool* pool, packaged_task* task);

void Thread_Pool_Shutdown(thread_pool* pool);

void ThreadPool_Wait(thread_pool* pool, packaged_task* task);
