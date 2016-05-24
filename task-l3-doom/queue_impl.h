#define define_queue(TYPE)\
typedef struct {\
Node_##TYPE* begin;\
Node_##TYPE* end;\
} Queue_##TYPE;


#define implement_queue(TYPE)\
void Queue_##TYPE_init( Queue_##TYPE* q )\
{\
q->begin = q->end = NULL;\
}\
\
void Queue_##TYPE_push( Queue_##TYPE* q, ##TYPE val)\
{\
	Node_##TYPE* temp = malloc_s( sizeof( Node ));\
	temp->sock_info = sock_info;\
	temp->act = act;\
	temp->next = NULL;\
	if( q->end == NULL) {\
		q->begin = q->end = temp;\
	} else {\
		q->end->next = temp;\
	}\
}\
\
Node* Queue_##TYPE_pop( Queue_##TYPE* q )\
{\
	Node_##TYPE* temp = q->begin;\
	q->begin = q->begin->next;\
	if( q->begin == NULL) {\
		q->end = NULL;\
	}\
	return temp;\
}\
\
void Queue_##TYPE_destroy( Queue_##TYPE* q )\
{\
	Node_##TYPE* temp;\
	while( q->begin != NULL) {\
		temp = q->begin;\
		q->begin = q->begin->next;\
		free( temp );\
	}\
}\
\
int Queue_##TYPE_empty( Queue_##TYPE* q )\
{\
	return ( q->begin == NULL);\
}
