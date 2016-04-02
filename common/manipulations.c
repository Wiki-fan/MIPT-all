#include <err.h>
#include <stdlib.h>
#include "manipulations.h"
#include "../task-s2-gets_safe/safe_string.h"

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

void* malloc_s( size_t size )
{
    void* ret;
    ret = malloc((size_t) size + 1 );    /* Allocating memory for answer. */
    if( ret == NULL)
    {
        err( 1, "Memory allocation error" );
    }
    return ret;
}

void* realloc_s( void* buf, size_t size )
{
    void* ret;
    ret = realloc( buf, size );    /* Allocating memory for answer. */
    if( ret == NULL)
    {
        err( 1, "Memory reallocation error" );
    }
    return ret;
}

void read_file( FILE* f, char*** strs, int* lines_number, int* columns_number)
{
    char** ret;
    int i = 0, j, len;
    int size = 100; /* Initial string buffer size. */
    ret = (char**) malloc_s( size * sizeof( char* ));

    while( (len = gets_safe( f, &( ret[i] ))) )
    {
        ++i;
        if (len > *columns_number)
        {
            *columns_number = len;
        }
        if( i > size )
        {
            size *= 2;
            ret = realloc_s( ret, size );
        }
    }
    *strs = ret;
    *lines_number = i;
}
