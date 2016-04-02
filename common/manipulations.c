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

void read_file( FILE* f, char*** strs, int** lengths, int* lines_number, int* columns_number)
{
    char** ret;
    int* lens;
    int i = 0, j, len;
    int size = 1000; /* Initial string buffer size. */
    ret = (char**) malloc_s( size * sizeof( char* ));
    lens = (int*) malloc_s(size*sizeof(int));

    while( (len = gets_safe( f, &( ret[i] ))) )
    {
        lens[i] = len;
        ++i;
        if (len > *columns_number)
        {
            *columns_number = len;
        }
        if( i > size )
        {
            size *= 2;
            ret = realloc_s( ret, size );
            lens = realloc_s(lens, size);
        }
    }
    *strs = ret;
    *lengths = lens;
    *lines_number = i;
}
