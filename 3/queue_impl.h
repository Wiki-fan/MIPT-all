#define define_queue(TYPE)\
typedef struct Node_##TYPE##_t {\
struct Node_##TYPE##_t* next;\
TYPE* val;\
} Node_##TYPE;\
\
typedef struct {\
Node_##TYPE* head;\
Node_##TYPE* tail;\
size_t size;\
} Queue_##TYPE;\
\
void Queue_##TYPE##_init( Queue_##TYPE* q );\
void Queue_##TYPE##_push( Queue_##TYPE* q, TYPE* val);\
TYPE* Queue_##TYPE##_pop( Queue_##TYPE* q );\
int Queue_##TYPE##_empty( Queue_##TYPE* q );\
void Queue_##TYPE##_destroy( Queue_##TYPE* q );

#define implement_queue(TYPE)\
void Queue_##TYPE##_init( Queue_##TYPE* q)\
{\
    q->head = q->tail = NULL;\
    q->size = 0;\
}\
\
void Queue_##TYPE##_push( Queue_##TYPE* q, TYPE* val)\
{\
	Node_##TYPE* temp = malloc( sizeof( Node_##TYPE ));\
    temp->val = val;\
    temp->next = NULL;\
	if(q->size == 0) {\
		q->head = q->tail = temp;\
	} else {\
		q->tail->next = temp;\
        q->tail = temp;\
	}\
    ++q->size;\
}\
\
TYPE* Queue_##TYPE##_pop( Queue_##TYPE* q )\
{\
	assert(q->size != 0);\
	Node_##TYPE* temp = q->head;\
	q->head = q->head->next;\
	if( q->head == NULL) {\
		q->tail = NULL;\
	}\
    --q->size;\
	TYPE* ret = temp->val;\
	free(temp);\
	return ret;\
}\
\
void Queue_##TYPE##_destroy( Queue_##TYPE* q )\
{\
	Node_##TYPE* temp;\
	while( q->head != NULL) {\
		temp = q->head;\
		q->head = q->head->next;\
		free( temp );\
	}\
}\
\
int Queue_##TYPE##_empty( Queue_##TYPE* q )\
{\
	return ( q->size == 0);\
}
