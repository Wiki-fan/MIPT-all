#include <malloc.h>
#include "ThreadPool.h"
#include "BlockingQueue.h"

void* RunWorker(void* arg) {
    thread_pool* this_pool = (thread_pool*) arg;
    while (1) {
        packaged_task* task;

        if (blocking_queue_get(&this_pool->queue, (void**)&task)) {
            task->func_ptr(task->args);
            //atomic_store(&task->is_ready, 1); // TODO: ATOMIC!
            //pthread_cond_broadcast(&task->cv);
            //free(task);
        } else {
            break;
        }
    }
}

void ThreadPool_init(thread_pool* pool, size_t num_workers) {
    pool->num_workers = num_workers;
    blocking_queue_init(&pool->queue, 100000);

    PRERR(pthread_mutex_init(&pool->mutex, NULL));
    PRERR(pthread_mutex_init(&pool->mutex_side_task, NULL));

    pool->workers = malloc(num_workers * sizeof(pthread_t));
    for (size_t i = 0; i < num_workers; ++i) {
        PRERR(pthread_create(&pool->workers[i], NULL, RunWorker, pool));
    }

}

void ThreadPool_Submit(thread_pool* pool, packaged_task* task) {
    PRERR(pthread_mutex_lock(&pool->mutex));
    blocking_queue_put(&pool->queue, task);
    PRERR(pthread_mutex_unlock(&pool->mutex));
}

void Thread_Pool_Shutdown(thread_pool* pool) {
    PRERR(pthread_mutex_lock(&pool->mutex));
    blocking_queue_shutdown(&pool->queue);
    for (size_t i = 0; i < pool->num_workers; ++i) {
        int status;
        PRERR(pthread_join(pool->workers[i], (void**)&status));
    }
    blocking_queue_destroy(&pool->queue);
    free(pool->workers);
    PRERR(pthread_mutex_unlock(&pool->mutex));
    PRERR(pthread_mutex_destroy(&pool->mutex));
    PRERR(pthread_mutex_destroy(&pool->mutex_side_task));
}

void ThreadPool_help(thread_pool* pool) {
    packaged_task* side_task;
    if (blocking_queue_try_get(&pool->queue, (void**)&side_task)) {
        side_task->func_ptr(side_task->args);
        //atomic_store(&side_task->is_ready, 1);
    }
}
// Надо тестировать.
void ThreadPool_Wait(thread_pool* pool, packaged_task* task) {
    // Если очередь не пуста, делаем новые задания. Выходим, если было сделано то, которое мы ждём.
    // Если в очереди не осталось заданий, значит, нужное уже выполняется и скоро будет сделано.
    // Можем выйти из цикла и встать на Wait'е.
    //pthread_mutex_lock(&pool->mutex_side_task);
    packaged_task* side_task;

    while (!atomic_load(&task->is_ready)) {
        // TODO: Если заснём тут, плохо
        //while (pthread_cond_wait(&task->cv, &pool->mutex_side_task)) {
        if (!blocking_queue_try_get(&pool->queue, (void**)&side_task)) {
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

void ThreadPool_Wait_until_free(thread_pool* tp) {
    while (blocking_queue_size(&tp->queue)) {
        //ThreadPool_help(tp);
    }
}
