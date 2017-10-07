#pragma once

#include <limits.h>
#include <stdatomic.h>
#include "BlockingQueue.h"
#include "packaged_task.h"

struct thread_pool {
    size_t num_workers; //atomic
    blocking_queue queue;
    pthread_t* workers; // std::vector <std::thread>
    pthread_mutex_t mutex, mutex_side_task;
};

void* RunWorker(void* arg) {
    struct thread_pool* this_pool = (struct thread_pool*) arg;
    while (1) {
        packaged_task* task;

        if (blocking_queue_get(&this_pool->queue, &task)) {
            task->func_ptr(task->args);
            atomic_store(&task->is_ready, 1); // TODO: ATOMIC!
            //pthread_cond_broadcast(&task->cv);
        } else {
            break;
        }
    }
}

void ThreadPool_init(struct thread_pool* pool, size_t num_workers) {
    pool->num_workers = num_workers;
    blocking_queue_init(&pool->queue, 100000);

    assert(!pthread_mutex_init(&pool->mutex, NULL));
    assert(!pthread_mutex_init(&pool->mutex_side_task, NULL));

    pool->workers = malloc(num_workers * sizeof(pthread_t));
    for (size_t i = 0; i < num_workers; ++i) {
        assert(!pthread_create(&pool->workers[i], NULL, RunWorker, &pool));
    }

}

void ThreadPool_Submit(struct thread_pool* pool, packaged_task* task) {
    blocking_queue_put(&pool->queue, task);
}

void Thread_Pool_Shutdown(struct thread_pool* pool) {
    assert(!pthread_mutex_lock(&pool->mutex));
    blocking_queue_shutdown(&pool->queue);
    for (size_t i = 0; i < pool->num_workers; ++i) {
        void* retval;
        assert(!pthread_join(pool->workers[i], &retval));
    }
    blocking_queue_destroy(&pool->queue);
    free(pool->workers);
    assert(!pthread_mutex_unlock(&pool->mutex));
    assert(!pthread_mutex_destroy(&pool->mutex));
    assert(!pthread_mutex_destroy(&pool->mutex_side_task));
}

void ThreadPool_Wait(struct thread_pool* pool, packaged_task* task) {
    // Если очередь не пуста, делаем новые задания. Выходим, если было сделано то, которое мы ждём.
    // Если в очереди не осталось заданий, значит, нужное уже выполняется и скоро будет сделано.
    // Можем выйти из цикла и встать на Wait'е.
    //pthread_mutex_lock(&pool->mutex_side_task);
    packaged_task* side_task;

    while (!atomic_load(&task->is_ready)) {
        // TODO: Если заснём тут, плохо
        //while (pthread_cond_wait(&task->cv, &pool->mutex_side_task)) {
        if (!blocking_queue_try_get(&pool->queue, &side_task)) {
            break;
        } else {
            side_task->func_ptr(side_task->args);
            atomic_store(&side_task->is_ready, 1);
            //pthread_cond_broadcast(&side_task->cv);
        }
        //}
    }
    //async_result.wait();
    //pthread_mutex_unlock(&pool->mutex_side_task);
}

