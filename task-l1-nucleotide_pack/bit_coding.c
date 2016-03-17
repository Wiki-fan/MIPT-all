#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "bit_coding.h"
#include "../common/bit_manipulations.h"

int BitEncode( const char* toEncode, const char* alphabet, char** answer)
{
    unsigned char alphabetSize = (unsigned char) strlen( alphabet ),    /* Number of symbols in alphabet. */
                  symbolSize = (unsigned char) ( log( alphabetSize ) + 2 );    /* Bits in one character. */
    int strSize = (int) strlen( toEncode ),    /* Lenght of input string. */
        answerSize = (int) ( strSize * symbolSize );    /* Length of output string (in bits). */
    const char* in = toEncode;    /* Input iterator. */
    char* out;    /* Output iterator. */
    char character;
    char c;
    char bitPosToWrite = 0,
         i,
         bit;
    char* temp;
    int count = 0;

    *answer = malloc( answerSize / 8 + 1 );    /* Allocating memory for answer. */
    out = *answer;
    while( c = *in )
    {
        c = tolower(*in);
        temp = strchr( alphabet, c );    /* Integer that will represent character encoded. */
        if( temp == NULL)
        {
            ++in;
            continue; /* Character is not in our alphabet. Skip. */
        }
        else
        {
            character = (char) ( temp - alphabet );
        }

        for( i = 0; i < symbolSize; ++i )
        {
            READ_BIT( bit, i, character );

            /* Writing bit. */
            WRITE_BIT( bit, bitPosToWrite, *out );
            ++bitPosToWrite;
            if( bitPosToWrite > 7 )
            {
                ++out;
                bitPosToWrite = 0;
            }
        }
        ++in;
        ++count;
    }

    return count; /* Encoded symbols, without end-of-string character. */
}

int BitDecode( const char* toDecode, const char* alphabet, char** answer, int size )
{
    unsigned char alphabetSize = (unsigned char) strlen( alphabet ),    /* Number of symbols in alphabet. */
                  symbolSize = (unsigned char) ( log( alphabetSize ) + 2 );    /* Bits in one character. */
    int i = 0, j;
    const char* in = toDecode;    /* Input iterator. */
    char bit,    /* Bit that we'll read and write. */
         symbol,    /* Encoded character. */
         * out,    /* Output iterator. */
         bitPosToRead = 0;
    *answer = malloc((size_t) size + 1 );    /* Allocating memory for answer. */
    out = *answer;

    while (i< size )
    {
        symbol = 0;
        /* Reading symbolSize bits. */
        for( j = 0; j < symbolSize; ++j )
        {
            /* Reading bit. */
            READ_BIT( bit, bitPosToRead, *in );
            ++bitPosToRead;
            if( bitPosToRead > 7 )
            {
                ++in;
                bitPosToRead = 0;
            }

            /* Writing bit to symbol. */
            WRITE_BIT( bit, j, symbol );
        }
        ++i;
        *out = alphabet[(unsigned char) symbol];
        ++out;
    }
    *out = '\0'; /* Adding end-of-string symbol. */
    return i;
}
