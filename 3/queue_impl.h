#define define_queue(TYPE)\
typedef struct Node_##TYPE##_t {\
struct Node_##TYPE##_t* next;\
TYPE* val;\
} Node_##TYPE;\
\
typedef struct {\
Node_##TYPE* begin;\
Node_##TYPE* end;\
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
    q->begin = q->end = NULL;\
    q->size = 0;\
}\
\
void Queue_##TYPE##_push( Queue_##TYPE* q, TYPE* val)\
{\
	Node_##TYPE* temp = malloc( sizeof( Node_##TYPE ));\
    temp->val = val;\
	if( q->end == NULL) {\
		q->begin = q->end = temp;\
	} else {\
		q->end->next = temp;\
	}\
    ++q->size;\
}\
\
TYPE* Queue_##TYPE##_pop( Queue_##TYPE* q )\
{\
	Node_##TYPE* temp = q->begin;\
	q->begin = q->begin->next;\
	if( q->begin == NULL) {\
		q->end = NULL;\
	}\
    --q->size;\
	return temp->val;\
}\
\
void Queue_##TYPE##_destroy( Queue_##TYPE* q )\
{\
	Node_##TYPE* temp;\
	while( q->begin != NULL) {\
		temp = q->begin;\
		q->begin = q->begin->next;\
		free( temp );\
	}\
}\
\
int Queue_##TYPE##_empty( Queue_##TYPE* q )\
{\
	return ( q->begin == NULL);\
}
