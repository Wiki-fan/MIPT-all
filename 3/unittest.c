
#include <malloc.h>
#include <assert.h>
#include "packaged_task.h"
#include "BlockingQueue.h"

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
    printf("%d %d %d", d, e ,f);
    Queue_int_destroy(&q);
}

void test_blocking_queue() {
    blocking_queue q;
    blocking_queue_init(&q, 5);
    pthread_cond_t c;
    packaged_task t = {NULL, NULL, c, 13};
    blocking_queue_put(&q, &t);
    packaged_task* ret;
    blocking_queue_get(&q, &ret);
    blocking_queue_get(&q, &ret);
    blocking_queue_shutdown(&q);
}

int main() {
    test_blocking_queue();
    return 0;
}