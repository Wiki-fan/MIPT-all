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
