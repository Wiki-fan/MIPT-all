#include <malloc.h>
#include <assert.h>
#include "queue_impl.h"

void Queue_init( Queue* q)
{
    q->head = q->tail = NULL;
    q->size = 0;
}

void Queue_push( Queue* q, void* val)
{
	Node* temp = malloc( sizeof( Node ));
    temp->val = val;
    temp->next = NULL;
	if(q->size == 0) {
		q->head = q->tail = temp;
	} else {
		q->tail->next = temp;
        q->tail = temp;
	}
    ++q->size;
}

void* Queue_pop( Queue* q )
{
	assert(q->size != 0);
	Node* temp = q->head;
	q->head = q->head->next;
	if( q->head == NULL) {
		q->tail = NULL;
	}
    --q->size;
	void* ret = temp->val;
	free(temp);
	return ret;
}

void Queue_destroy( Queue* q )
{
	Node* temp;
	while( q->head != NULL) {
		temp = q->head;
		q->head = q->head->next;
		free( temp );
	}
}

int Queue_empty( Queue* q )
{
	return ( q->size == 0);
}
