#include <err.h>
#include <stdlib.h>
#include "utils.h"

void* malloc_s( size_t size )
{
    void* ret;
    ret = malloc( size );    /* Allocating memory for answer. */
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

FILE* fopen_s( const char* filename, const char* mode )
{
    FILE* f = fopen( filename, mode );
    if( f == NULL)
    {
        err( 2, "Error opening file %s", filename );
    }
    return f;
}
