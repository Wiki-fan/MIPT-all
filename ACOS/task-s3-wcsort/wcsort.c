#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <err.h>
#include "../common/utils.h"

/* Read line until \n or EOF, count symbols and words. */
int gets_n( FILE* f, char** str, int* symbols, int* words )
{
    int bufSize = 100; /* Initial buffer size. */
    char* buf;
    int c;
    bool fl = true;
    char* iter;
    int i = 0;

    if (feof(f))
        return 0;

    buf = (char*) malloc_s( bufSize * sizeof( char ));
    iter = buf;
    while( fl )
    {
        c = getc( f );
        ++i;
        /* Realloc if needed. */
        if( i == bufSize-1 )
        {
            bufSize *= 2;
            buf = realloc_s( buf, bufSize * sizeof( char ));
            iter = buf + i-1;
        }
        /* Appending '\n' if string ended with EOF (or if '\n' founded, obviously). */
        if ( c == '\n' || c == EOF)
        {
            *iter = '\n';
            fl = false;
        }
        else
        {
            *iter = c;
        }
        /* Isspace, and previous character exists and isgraph. */
        if( isspace( *iter ) && iter != buf && isgraph( iter[-1] ))
        {
            ++( *words );
        }

        ++( *symbols );
        ++iter;
    }
    *str = buf;
    return i;
}

/* Puts string str (ended with '\n') to file f. */
void puts_n( FILE* f, char* str )
{
    do
    {
        fputc( *str, f );
    }
    while (*str++ != '\n');
}

/* Compare strings ended with '\n'. */
int strcmp_n( const void* a, const void* b )
{
    char* x = *(char**) a, * y = *(char**) b;
    int ret = 0;
    bool fl = true;
    while( *x != '\n' && *y != '\n' )
    {
        if (fl && *x != *y)
        {
            if (*x < *y)
            {
                ret = -1;
            }
            else
            {
                ret = 1;
            }
            fl = false;
        }

        ++x;
        ++y;
    }
    if (*x == '\n' && *y != '\n')
    {
        return -1;
    }
    else if (*x != '\n' && *y == '\n')
    {
        return 1;
    }
    else
    {
        return ret;
    }

}

void wcsort( FILE* inf, FILE* outf )
{
    char** strs;
    int symbols = 0, words = 0, strings = 0;
    int i = 0, j, size = 100;
    strs = (char**) malloc_s( size * sizeof( char* ));
    while( gets_n( inf, &( strs[i] ), &symbols, &words ))
    {
        ++strings;
        ++i;
        if( i >= size )
        {
            size *= 2;
            strs = realloc_s( strs, size*sizeof(strs[0]) );
        }
    }
    qsort( strs, i, sizeof( char* ), strcmp_n );

    for( j = 0; j < i; ++j )
    {
        puts_n( outf, strs[j] );
        free( strs[j] );
    }
    free( strs );
    printf( "%6d%6d%6d\n", strings-1, words, symbols-1 );
}

int main( int argc, char* argv[] )
{
    FILE* inf, *outf;
    switch( argc )
    {
    case 1:
        wcsort( stdin, stdout );
        break;
    case 2:
        if(( inf = fopen( argv[1], "r" )) == NULL)
        {
            err(2, "Failed to open file %s", argv[1] );
        }
        wcsort( inf, stdout );
        break;
    case 3:
        if(( inf = fopen( argv[1], "r" )) == NULL)
        {
            err( 2, "Failed to open file %s", argv[1] );
        }
        if(( outf = fopen( argv[2], "w" )) == NULL)
        {
            err(2, "Failed to open file %s", argv[2] );
        }
        wcsort( inf, outf );
    default:
        errx( 3, "Too many arguments" );
    }
    return 0;
}
