#include <string.h>
#include "safe_string.h"

int gets_safe_ext(FILE* f, char** str, const char* term)
{
    int bufSize = 1000; /* Initial buffer size. */
    char* buf = (char*)malloc(bufSize*sizeof(char)), *newBuf = NULL;
    int c;
    char* iter = buf;
    int i = 0;
    while ( (c = getc(f)) != EOF)
    {
        ++i;
        /* Realloc if needed. */
        if (i == bufSize-1)
        {
            bufSize *= 2;
            newBuf = realloc(buf, bufSize*sizeof(char));
            if (newBuf != 0)
            {
                buf = newBuf;
                iter = buf + i-1;
            }
            else
            {
                puts("Error reallocating memory\n");
                exit(1);
            }
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