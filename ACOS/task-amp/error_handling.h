#ifndef TASK_ERROR_STUFF
#define TASK_ERROR_STUFF
#include "common_types.h"

extern const char * const errmsg[];
enum ERRCODE
{
    E_SIGPROCMASK,
    E_TIMER_CREATE,
    E_SIGACTION,
    E_TIMER_SETTIME,
    E_SOCKET,
    E_WRITE,
	E_READ,
    E_LISTEN,
    E_BIND,
    E_ACCEPT,
	E_BIGDATA
};

#define CN1( VAL, CODE )\
if ((VAL) == -1) \
err(CODE, "%s", errmsg[CODE] );

#define CNON0( VAL, CODE )\
if ((VAL) != 0) \
err(CODE, "%s", errmsg[CODE] );

#endif /* TASK_ERROR_STUFF */
