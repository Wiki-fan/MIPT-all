#ifndef COMMON__UTILS
#define COMMON__UTILS

#include <stdio.h>

void* malloc_s( size_t size );
void* realloc_s( void* buf, size_t size );
FILE* fopen_s( const char* filename, const char* mode);

#endif /* COMMON__UTILS */