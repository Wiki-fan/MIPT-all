#ifndef TASK_L3__ERROR_STUFF
#define TASK_L3__ERROR_STUFF
#include "common_types.h"

extern const char * const errmsg[];
enum ERRCODE
{
    E_LOSTCONNECTION,
    E_SIGPROCMASK,
    E_TIMER_CREATE,
    E_SIGACTION,
    E_TIMER_SETTIME,
    E_TCGETATTR,
    E_TCSETATTR,
    E_SOCKET,
    E_CONNECT,
    E_FOPEN,
    E_READ,
    E_WRITE,
    E_LISTEN,
    E_BIND,
    E_ACCEPT,
    E_SELECT,
    E_POLL,
    E_FREOPEN,
    E_WRONG_PARAMETERS
};

#define CN1( VAL, CODE )\
if ((VAL) == -1) \
err(CODE, "%s", errmsg[CODE] );

#define CNON0( VAL, CODE )\
if ((VAL) != 0) \
err(CODE, "%s", errmsg[CODE] );

#define CNULL( VAL, CODE )\
if ((VAL) == NULL) \
err(CODE, "%s", errmsg[CODE] );

#define NFN1(VAL, CODE)\
if ((VAL) == -1)\
	perror( errmsg[CODE] );

#define RAISE(CODE)\
	err(CODE, "%s", errmsg[CODE]);

#endif /* L3__ERROR_STUFF */
