#pragma once

#include <limits.h>
#include "BlockingQueue.h"
#include "packaged_task.h"

struct thread_pool {
    size_t num_workers; //atomic
    struct blocking_queue queue;
    pthread_t* workers; // std::vector <std::thread>
    pthread_mutex_t mutex;
};

void* RunWorker(void* arg) {
    struct thread_pool* this_pool = (struct thread_pool*) arg;
    while (1) {
        packaged_task* task;

        if (blocking_queue_get(&this_pool->queue, &task)) {
            task->func_ptr(task->args);
        } else {
            break;
        }
    }
}

void ThreadPool_init(struct thread_pool* pool, size_t num_workers) {
    pool->num_workers = num_workers;
    blocking_queue_init(&pool->queue, INT_MAX);
    pool->workers = malloc(num_workers * sizeof(pthread_t));

    for (size_t i = 0; i < num_workers; ++i) {
        pthread_create(&pool->workers[i], NULL, RunWorker, &pool);
    }
}

void ThreadPool_Submit(struct thread_pool* pool, packaged_task* task) {
    blocking_queue_put(&pool->queue, &task);
}

void Thread_Pool_Shutdown(struct thread_pool* pool) {
    pthread_mutex_lock(&pool->mutex);
    blocking_queue_shutdown(&pool->queue);
    for (size_t i = 0; i < pool->num_workers; ++i) {
        void* retval;
        pthread_join(pool->workers[i], &retval);
    }
    pthread_mutex_unlock(&pool->mutex);
}

void ThreadPool_Wait(struct thread_pool* pool, packaged_task* task) {
    // Если очередь не пуста, делаем новые задания. Выходим, если было сделано то, которое мы ждём.
    // Если в очереди не осталось заданий, значит, нужное уже выполняется и скоро будет сделано.
    // Можем выйти из цикла и встать на Wait'е.
    pthread_mutex_lock(&pool->mutex);
    packaged_task* side_task;

    while(!task->is_ready) {
        // Если заснём тут, плохо
        while (pthread_cond_wait(&task->cv, &pool->mutex)) {
            if (!blocking_queue_try_get(&pool->queue, &side_task)) {
                break;
            } else {
                side_task->func_ptr(side_task->args);
            }
        }
    }
    //async_result.wait();
    pthread_mutex_unlock(&pool->mutex);
}

