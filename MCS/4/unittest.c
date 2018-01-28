
#include <malloc.h>
#include <assert.h>
#include "packaged_task.h"
#include "BlockingQueue.h"
#include "ThreadPool.h"

define_queue(int)
implement_queue(int)

void test_queue() {
    Queue_int q;
    Queue_int_init(&q);
    int a=1, b=2, c=3, d, e, f;
    Queue_int_push(&q, &a);
    Queue_int_push(&q, &b);
    Queue_int_push(&q, &c);
    d = *Queue_int_pop(&q);
    e = *Queue_int_pop(&q);
    f = *Queue_int_pop(&q);
    f = *Queue_int_pop(&q);
    printf("%d %d %d", d, e ,f);
    Queue_int_destroy(&q);
}

void test_blocking_queue() {
    blocking_queue_packaged_task q;
    blocking_queue_init(&q, 5);
    pthread_cond_t c;
    packaged_task t = {NULL, NULL, .is_ready=13};
    blocking_queue_put(&q, &t);
    packaged_task* ret;
    blocking_queue_get(&q, &ret);
    blocking_queue_shutdown(&q);
}

pthread_mutex_t mutex;

int a = 0;

void* do_deeds(void* arg) {
    for (int i = 0; i<1000; ++i) {
        pthread_mutex_lock(&mutex);
        ++a;
        pthread_mutex_unlock(&mutex);
    }
}

void test_thread_pool() {
    struct thread_pool tp;
    ThreadPool_init(&tp, 4);
    packaged_task task = {do_deeds, NULL};
    for (int i = 0; i<10; ++i) {
        ThreadPool_Submit(&tp, &task);
    }
    Thread_Pool_Shutdown(&tp);
    printf("%d", a);
}

int main() {
    //test_queue();
    test_thread_pool();
    return 0;
}