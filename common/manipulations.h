#ifndef COMMON__MANIPULATIONS_H
#define COMMON__MANIPULATIONS_H

#include <stdio.h>

FILE* fopen_s( const char* filename, const char* mode);
int strcnt (char *s, char c);
void* malloc_s( size_t size );
void* realloc_s( void* buf, size_t size );
void read_file( FILE* f, char*** strs, int* lines_number, int* columns_number);

#endif /* COMMON__MANIPULATIONS_H */