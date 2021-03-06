#include "nucleotide_pack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <err.h>
#include <ctype.h>
#include "../task-s2-gets_safe/safe_string.h"
#include "bit_coding.h"
#include "../common/utils.h"

#define NUCLEOTIDE_GROUP_W 10
#define NUCLEOTIDE_STRING_G 6

int readWhileNotNew( FILE* f, char** str, bool checkTerm, int size )
{
    int bufSize = 10; /* Initial buffer size. */
    int c;
    int i = 0;
    char* iter;
    char* buf = (char*) malloc_s( bufSize * sizeof( char ));
    iter = buf;

    if( size == -1 )
    {
        size = INT_MAX;
    }
    while( i < size && ( c = getc( f )) != EOF)
    {
        /* Character that should terminate input. */
        if( checkTerm && c == '>' )
        {
            ungetc( c, f );
            ++i;
            break;
        }
        if (checkTerm && !strchr("atgcu", tolower(c)) )
        {
            continue;
        }
        ++i;
        /* Realloc if needed. */
        if( i == bufSize - 1 )
        {
            bufSize *= 2;
            buf = realloc_s( buf, bufSize * sizeof( char ));
            iter = buf + i - 1;
        }
        *iter++ = c;
    }
    /* Append '\0' if terminating character was term. */
    if( c != '\0' )
    {
        *iter = '\0';
    }
    *str = buf;
    return i;
}

void nucleotide_pack( char direction, const char* inFileName, const char* outFileName )
{
    FILE* inf, * outf;
    char* header;
    char* alphabet = "atgcu";
    const unsigned char alphabetSize = (unsigned char) strlen( alphabet ),    /* Number of symbols in alphabet. */
                        symbolSize = (unsigned char) log( alphabetSize ) + 2;    /* Bits in one character. */
    char* inBuf, * outBuf;
    int ret;

    inf = fopen_s( inFileName, "rb" );
    outf = fopen_s( outFileName, "wb" );

    while( gets_safe( inf, &header ))
    {
        /* Copying the header. */
        fputs( header, outf );
        free( header );
        if( direction == 0 )
        {
            /* Pack. */
            if (!readWhileNotNew( inf, &inBuf, true, -1 ))
                break;
            ret = BitEncode( inBuf, alphabet, &outBuf );
            fwrite( &ret, sizeof( int ), 1, outf );
            fwrite( outBuf, sizeof( char ), ((ret * symbolSize) % 8 == 0? ret * symbolSize / 8 : ret * symbolSize / 8 + 1 )/* + 1*/, outf );
        }
        else /*if( direction == 1 )*/
        {
            /* Unpack. */
            char* iter;
            fread( &ret, sizeof( int ), 1, inf );
            readWhileNotNew( inf, &inBuf, false, ret * symbolSize / 8 + 1 );
            ret = BitDecode( inBuf, alphabet, &outBuf, ret );
            iter = outBuf;
            while( ret > 0 )
            {
                int j;
                for( j = 0; j < NUCLEOTIDE_STRING_G && ret > 0; ++j )
                {
                    fwrite( iter, sizeof( char ),
                            ret > NUCLEOTIDE_GROUP_W ? NUCLEOTIDE_GROUP_W : ret, outf );
                    ret -= NUCLEOTIDE_GROUP_W;
                    iter += NUCLEOTIDE_GROUP_W;
                    putc( ' ', outf );
                }
                putc( '\n', outf );
            }

        }
        free( inBuf );
        free( outBuf );

    }

    free( header );
    fclose( inf );
    fclose( outf );
}
