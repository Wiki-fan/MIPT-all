#define define_vector(TYPE)\
typedef struct {\
	TYPE* arr;\
	int size;\
	int max_size;\
} Vector_##TYPE;\
\
void Vector_##TYPE##_init(Vector_##TYPE* v, int init_size);\
void Vector_##TYPE##_push(Vector_##TYPE* v, TYPE elem);\
void Vector_##TYPE##_set(Vector_##TYPE* v, TYPE elem, int pos);\
void Vector_##TYPE##_alloc(Vector_##TYPE* v, int pos);\
void Vector_##TYPE##_destroy(Vector_##TYPE* v );


#define implement_vector(TYPE)\
void Vector_##TYPE##_init(Vector_##TYPE* v, int init_size)\
{\
	v->arr = malloc_s(init_size*sizeof(TYPE));\
	v->max_size = init_size;\
	v->size = 0;\
}\
\
void Vector_##TYPE##_push(Vector_##TYPE* v, TYPE elem)\
{\
	v->arr[v->size++] = elem;\
	if (v->size >= v->max_size) {\
		v->max_size *= 2;\
		v->arr = realloc_s(v->arr, v->max_size*sizeof(TYPE));\
	}\
}\
\
void Vector_##TYPE##_set(Vector_##TYPE* v, TYPE elem, int pos)\
{\
	if (pos >= v->max_size) {\
		v->max_size = pos+1;\
		v->arr = realloc_s(v->arr, v->max_size*sizeof(TYPE));\
	}\
	if (pos+1 > v->size) {\
		v->size = pos+1;\
	}\
	v->arr[pos] = elem;\
}\
\
void Vector_##TYPE##_alloc(Vector_##TYPE* v, int pos)\
{\
	if (pos >= v->max_size) {\
		v->max_size = pos+1;\
		v->arr = realloc_s(v->arr, v->max_size*sizeof(TYPE));\
	}\
	if (pos+1 > v->size) {\
		v->size = pos+1;\
	}\
}\
\
void Vector_##TYPE##_destroy(Vector_##TYPE* v )\
{\
	free(v->arr);\
}
