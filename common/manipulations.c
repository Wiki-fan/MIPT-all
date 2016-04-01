#include <err.h>
#include <stdlib.h>
#include "manipulations.h"

FILE* fopen_s( const char* filename, const char* mode )
{
    FILE* f = fopen( filename, mode );
    if( f == NULL)
    {
        err( 2, "Error opening file %s", filename );
    }
    return f;
}

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

void* malloc_s(size_t size)
{
    void* ret;
    ret = malloc((size_t) size + 1 );    /* Allocating memory for answer. */
    if( ret == NULL)
    {
        err( 1, "Error (re)allocating memory" );
    }
    return ret;
}