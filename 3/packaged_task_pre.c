#include <malloc.h>
#include <assert.h>
#include "queue_impl.h"
#include "packaged_task.h"
/*typedef struct {
    void* (* func_ptr)(void*);
    void* args;
    //pthread_cond_t cv;
    atomic_int is_ready;
} packaged_task;
typedef struct Node_packaged_task_t { struct Node_packaged_task_t* next;packaged_task* val; } Node_packaged_task;
typedef struct { Node_packaged_task* head;Node_packaged_task* tail;size_t size; } Queue_packaged_task;*/
void Queue_packaged_task_init(Queue_packaged_task* q);
void Queue_packaged_task_push(Queue_packaged_task* q, packaged_task* val);
packaged_task* Queue_packaged_task_pop(Queue_packaged_task* q);
int Queue_packaged_task_empty(Queue_packaged_task* q);
void Queue_packaged_task_destroy(Queue_packaged_task* q);

void Queue_packaged_task_init(Queue_packaged_task* q) {
    q->head = q->tail = ((void*) 0);
    q->size = 0;
}

void Queue_packaged_task_push(Queue_packaged_task* q, packaged_task* val) {
    Node_packaged_task* temp = malloc(sizeof(Node_packaged_task));
    printf("pushing val on addr %ld\n", temp);
    temp->val = val;
    temp->next = ((void*) 0);
    if (q->size == 0) { q->head = q->tail = temp; }
    else {
        q->tail->next = temp;
        q->tail = temp;
    }
    ++q->size;
}

packaged_task* Queue_packaged_task_pop(Queue_packaged_task* q) {
    ((void) sizeof((q->size != 0) ? 1 : 0), __extension__ ({
        if (q->size != 0) {}
        else {
            __assert_fail("q->size != 0",
                          "packaged_task.c",
                          6,
                          __extension__
                          __PRETTY_FUNCTION__);
        }
    }));
    Node_packaged_task* temp = q->head;
    q->head = q->head->next;
    if (q->head == ((void*) 0)) { q->tail = ((void*) 0); }
    --q->size;
    packaged_task* ret = temp->val;
    free(temp);
    return ret;
}

void Queue_packaged_task_destroy(Queue_packaged_task* q) {
    Node_packaged_task* temp;
    while (q->head != ((void*) 0)) {
        temp = q->head;
        q->head = q->head->next;
        free(temp);
    }
}

int Queue_packaged_task_empty(Queue_packaged_task* q) { return (q->size == 0); }
