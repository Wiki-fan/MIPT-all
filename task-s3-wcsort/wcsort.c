#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* Read line until \n or EOF, count symbols and words. */
int gets_n( FILE* f, char** str, int* symbols, int* words )
{
    int bufSize = 1000; /* Initial buffer size. */
    char* buf, * newBuf = NULL;
    int c;
    bool fl = true;
    char* iter;
    int i = 0;

    if (feof(f))
        return 0;

    buf = (char*) malloc( bufSize * sizeof( char ));
    if (buf == NULL)
    {
        fprintf(stderr, "Error allocating memory.\n");
    }
    iter = buf;
    while( fl )
    {
        c = getc( f );
        ++i;
        /* Realloc if needed. */
        if( i == bufSize - 1 )
        {
            bufSize *= 2;
            newBuf = realloc( buf, bufSize * sizeof( char ));
            if( newBuf != 0 )
            {
                buf = newBuf;
                iter = buf + i - 1;
            }
            else
            {
                puts( "Error reallocating memory\n" );
                exit( 1 );
            }
        }
        /* Appending '\n' if string ended with EOF (or if '\n' founded, obviously). */
        if( c == EOF || c == '\n')
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

/* */
void puts_n( FILE* f, char* str )
{
    do
    {
        fputc( *str, f );
    }
    while (*str++ != '\n');
}

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
    strs = (char**) malloc( size * sizeof( char* ));

    while( gets_n( inf, &( strs[i] ), &symbols, &words ))
    {
        ++strings;
        ++i;
        if( i > size )
        {
            char** newStrs;
            size *= 2;
            newStrs = realloc( strs, size );
            if (newStrs == NULL)
            {
                fprintf(stderr, "Memory allocation error.\n");
                exit(1);
            }
        }
    }
    qsort( strs, i, sizeof( char* ), strcmp_n );

    for( j = 0; j < i; ++j )
    {
        puts_n( outf, strs[j] );
        free( strs[j] );
    }
    free( strs );
    printf( "%6d%6d%6d\n", strings, words, symbols );
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
            fprintf( stderr, "Failed to open file %s.\n", argv[1] );
            exit( 1 );
        }
        wcsort( inf, stdout );
        break;
    case 3:
        if(( inf = fopen( argv[1], "r" )) == NULL)
        {
            fprintf( stderr, "Failed to open file %s.\n", argv[1] );
            exit( 1 );
        }
        if(( outf = fopen( argv[2], "w" )) == NULL)
        {
            fprintf( stderr, "Failed to open file %s.\n", argv[2] );
            exit( 1 );
        }
        wcsort( inf, outf );
    default:
        printf( "Too many arguments\n" );
        return 1;
    }
    return 0;
}
