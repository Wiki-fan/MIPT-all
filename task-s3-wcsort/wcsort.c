#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wcsort.h"
#include <ctype.h>

int gets_n( FILE* f, char** str, int* symbols, int* words )
{
	int bufSize = 100; /* Initial buffer size. */
	char* buf = (char*) malloc( bufSize * sizeof( char )), * newBuf = NULL;
	int c;
	char* iter = buf;
	int i = 0;
	while(( c = getc( f )) != EOF) {
		++i;
		/* Realloc if needed. */
		if( i == bufSize - 1 ) {
			bufSize *= 2;
			newBuf = realloc( buf, bufSize * sizeof( char ));
			if( newBuf != 0 ) {
				buf = newBuf;
				iter = buf + i - 1;
			} else {
				puts( "Error reallocating memory\n" );
				exit( 1 );
			}
		}
		*iter++ = c;
		if (isgraph(c)) {
			++(*symbols);
		}
		/* Isspace, and previous character exists and isgraph. */
		if (isspace(c) && iter != buf && !isgraph(iter[-1])) {
			++(*words);
		}
		/* Characters that should terminate input. */
		if( c == '\n' ) {
			break;
		}
	}
	*str = buf;
	return i;
}

int puts_n( FILE* f, const char** str)
{
	while (*str != '\n') {
		fputc(f, *(str++));
	}
}

int strcmp_n(const void* a, const void* b)
{
	char* x = a, *y = b;
	while (*x != '\n' && *y != '\n' && *x > *y);
	if (*x == *y) {
		return 0;
	} else if (*x < *y) {
		return -1;
	} else return 1;
}

void wcsort(const char* inFileName, const char* outFileName)
{
	FILE* inf, *outf;
	char** strs;
	int symbols, words, strings;
	int i = 0, j, size = 100;
	strs = (char**)malloc(size*sizeof(char*));
	if(( inf = fopen( inFileName, "r" )) == NULL) {
		fprintf( stderr, "Failed to open file %s.\n", inFileName );
		exit( 1 );
	}
	if(( outf = fopen( outFileName, "w" )) == NULL) {
		fprintf( stderr, "Failed to open file %s.\n", outFileName );
		exit( 1 );
	}
	while(gets_n(inf, strs[i], symbols, words )) {
		++strings;
		++i;
		if (i > size) {
			char** newStrs;
			size *= 2;
			newStrs = realloc(strs, size);
		}
	}
	qsort(strs, i, sizeof(char*), strcmp_n);

	for (j = 0; j<i; ++j) {
		fputs(outf, strs[j]);
	}
	printf("%6d%6d%6d", strings, words, symbols);
}