#ifndef TASK_L4__PARSER_STUFF
#define TASK_L4__PARSER_STUFF

#define PROMPT ">"

typedef enum {
	T_BAR, /*|*/
			T_GT, /*>*/
			T_GTGT, /*>>*/
			T_LT, /*<*/
			T_AMP, /*&*/
			T_WORD, /* word ow command */
			T_NL, /* new line */
			T_END, /* end of input */
			T_ERROR /* some error in query, but work can be continued */
} TOKEN;

/*void push( char* buf, int* buf_size, int* cur_pos, char c );
TOKEN gettoken( char** buf );*/
TOKEN process( int* retpid, int makepipe, int* pipefd );

#endif /* TASK_L4__PARSER_STUFF */