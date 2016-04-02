#include <string.h>
#include <err.h>
#include "safe_string.h"
#include "../common/utils.h"

int gets_safe_ext(FILE* f, char** str, const char* term)
{
    int bufSize = 1000; /* Initial buffer size. */
    int c;
    char* iter;
    int i = 0;
    char* buf;
    buf = (char*)malloc_s(bufSize*sizeof(char));
    iter = buf;

    while ( (c = getc(f)) != EOF)
    {
        ++i;
        /* Realloc if needed. */
        if (i == bufSize-1)
        {
            bufSize *= 2;
            buf = realloc_s(buf, bufSize*sizeof(char));

            iter = buf + i-1;
        }
        *iter++ = c;
        /* Characters that should terminate input. */
        if (c == '\0' || strchr(term, c))
        {
            break;
        }
    }
    /* Append '\0' if terminating character was term. */
    if (c != '\0')
    {
        *iter = '\0';
    }
    *str = buf;
    return i;
}

int gets_safe(FILE* f, char** str)
{
    return gets_safe_ext(f, str, "\n");
}