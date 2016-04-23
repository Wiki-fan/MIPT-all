#ifndef TASK_L4__PARSER_STUFF
#define TASK_L4__PARSER_STUFF

typedef enum {
	T_BAR, /*|*/
			T_GT, /*>*/
			T_GTGT, /*>>*/
			T_LT, /*<*/
			T_AMP, /*&*/
			T_WORD,
	T_END
} TOKEN;

void push( char* buf, int* buf_size, int* cur_pos, char c );
TOKEN gettoken( char** buf );


#endif /* TASK_L4__PARSER_STUFF */