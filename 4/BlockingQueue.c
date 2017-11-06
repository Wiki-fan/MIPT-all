#include "BlockingQueue.h"
#include "queue_impl.h"
#include "cyclic_buffer.h"

void blocking_queue_init(blocking_queue* queue, int capacity) {
    queue->is_open = 1;
    queue->capacity = capacity;
    cb_init(&queue->q, 10000, sizeof(packaged_task));
    PRERR(pthread_mutex_init(&queue->mutex, NULL));
    PRERR(pthread_cond_init(&queue->cv_get, NULL));
    PRERR(pthread_cond_init(&queue->cv_put, NULL));
}

int blocking_queue_put(blocking_queue* queue, void* item) {
    PRERR(pthread_mutex_lock(&queue->mutex));

    while (queue->q.size == queue->capacity && queue->is_open) {
        PRERR(pthread_cond_wait(&queue->cv_put, &queue->mutex));
    }
    if (!queue->is_open) {
        PRERR(pthread_mutex_unlock(&queue->mutex));
        return 0;
    }

    //Queue_push(&queue->q, item);
    cb_push(&queue->q, item);
    PRERR(pthread_cond_signal(&queue->cv_get));

    PRERR(pthread_mutex_unlock(&queue->mutex));
    return 1;
}

int blocking_queue_get(blocking_queue* queue, void* item) {

    PRERR(pthread_mutex_lock(&queue->mutex));

    while (queue->q.size == 0 && queue->is_open) {
        PRERR(pthread_cond_wait(&queue->cv_get, &queue->mutex));
    }

    if (!queue->is_open) {
        if (queue->q.size == 0) {
            PRERR(pthread_mutex_unlock(&queue->mutex));
            return 0;
        }
    }

    //*item = Queue_pop(&queue->q);
    cb_pop(&queue->q, item);

    PRERR(pthread_cond_signal(&queue->cv_put));

    PRERR(pthread_mutex_unlock(&queue->mutex));
    return 1;
}

int blocking_queue_try_get(blocking_queue* queue, void* v) {
    pthread_mutex_lock(&queue->mutex);

    if (queue->q.size == 0) {
        PRERR(pthread_mutex_unlock(&queue->mutex));
        return 0;
    }

    //*v = Queue_pop(&queue->q);
    cb_pop(&queue->q, v);

    PRERR(pthread_cond_signal(&queue->cv_put));

    PRERR(pthread_mutex_unlock(&queue->mutex));
    return 1;
}

void blocking_queue_shutdown(blocking_queue* queue) {
    PRERR(pthread_mutex_lock(&queue->mutex));

    queue->is_open = 0;
    PRERR(pthread_cond_broadcast(&queue->cv_put));
    PRERR(pthread_cond_broadcast(&queue->cv_get));

    PRERR(pthread_mutex_unlock(&queue->mutex));
    PRERR(pthread_cond_broadcast(&queue->cv_put));
    PRERR(pthread_cond_broadcast(&queue->cv_get));
}

void blocking_queue_destroy(blocking_queue* queue) {
    cb_destroy(&queue->q);
    PRERR(pthread_mutex_destroy(&queue->mutex));
    PRERR(pthread_cond_destroy(&queue->cv_get));
    PRERR(pthread_cond_destroy(&queue->cv_put));
}

size_t blocking_queue_size(blocking_queue* queue) {
    PRERR(pthread_mutex_lock(&queue->mutex));
    size_t ret = queue->q.size;
    PRERR(pthread_mutex_unlock(&queue->mutex));
    return ret;
}