#ifndef COMMON__UTILS
#define COMMON__UTILS

#include <stdio.h>

void* malloc_s( size_t size );
void* calloc_s( size_t nMem, size_t size );
void* realloc_s( void* buf, size_t size );
FILE* fopen_s( const char* filename, const char* mode);

#define CHN1(COMM, CODE, MESSAGE)\
if ((COMM) == -1) \
err(CODE, MESSAGE);


#endif /* COMMON__UTILS */
