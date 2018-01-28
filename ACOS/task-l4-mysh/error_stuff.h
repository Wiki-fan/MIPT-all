#ifndef TASK_L3__ERROR_STUFF
#define TASK_L3__ERROR_STUFF

extern char* errmsg[];
enum ERRCODE
{
    E_OPEN,
    E_CLOSE,
    E_WAITPID,
    E_EXECVP,
    E_FORK,
    E_DUP2,
    E_FILE_OPEN,
    E_UNREACHABLE,
    E_PIPE,
    E_RUN,
    E_SIGACTION,
    E_UNSUPPORTED_SIGNAL,
    T_MY_REDIRECT
};

#define RAISE(CODE)\
	err(CODE, errmsg[CODE]);

#define RAISEX(CODE)\
	errx(CODE, errmsg[CODE]);

#define CN1( VAL, CODE )\
if ((VAL) == -1) \
err(CODE, errmsg[CODE] );

#define CNON0( VAL, CODE )\
if ((VAL) != 0) \
err(CODE, errmsg[CODE] );

#define CNULL( VAL, CODE )\
if ((VAL) == NULL) \
err(CODE, errmsg[CODE] );

/* Non-fatal negative 1.
 * Cleanups argv.
 * Implicitly captures struct proc* pr
 */
#define NFN1(VAL, CODE)\
	if ((VAL) == -1) {\
    int i;\
	/* Freeing args */\
    for( i = 0; i < pr->argc; ++i ) {\
        free( pr->argv[i] );\
    }\
    free( pr->argv );\
	    perror( errmsg[CODE] );\
	    return -1;\
	}

#define ERROR_HANDLER t_error
#define HANDLE_ERROR goto ERROR_HANDLER

/**For user-caused errors
 * Cause function to return T_ERROR.
 */
#define DROP( MSG ) {\
    \
    fprintf(stderr, "Error: %s\n", MSG);\
    HANDLE_ERROR; \
}

#endif /* L3__ERROR_STUFF */