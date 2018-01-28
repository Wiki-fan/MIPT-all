#include <stdlib.h>
#include "manipulations.h"
#include "../task-s2-gets_safe/safe_string.h"
#include "utils.h"

int strcnt( char* s, char c )
{
    int cnt = 0;

    while( *s )
    {
        if( *s == c )
        {
            ++cnt;
        }
        ++s;
    }
    return cnt;

}
