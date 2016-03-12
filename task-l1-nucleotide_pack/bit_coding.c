#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "BitCoding.h"
#include "../common/bit_manupulations.h"

int BitEncode( const char* toEncode, const char* alphabet, char** answer, bool tightBuffer, bool memoryAllocated  )
{
	unsigned char alphabetSize = (unsigned char) strlen( alphabet ),    /* Number of symbols in alphabet. */
			symbolSize = (unsigned char) ( log( alphabetSize ) + 2 );    /* Bits in one character. */
	int strSize = (int) strlen( toEncode ),    /* Lenght of input string. */
			answerSize = (int) ( strSize * symbolSize );    /* Length of output string (in bits). */
	const char* in = toEncode;    /* Input iterator. */
	char* out;    /* Output iterator. */
	char character;
	char bitPosToWrite = 0,
			i,
			bit;
	char* temp;
	bool fl = true;
	int count = 0;

	if( !memoryAllocated ) {
		*answer = malloc( answerSize * sizeof( char ) / 8 + 1 );    /* Allocating memory for answer. */
	}
	out = *answer;
	while( fl ) {
		if( !*in ) {
			if( tightBuffer ) {
				return count;
			}
			character = alphabetSize; /* End character. */
			fl = false;
		} else {
			temp = strchr( alphabet, *in );    /* Integer that will represent character encoded. */
			if( temp == NULL) {
				continue; /* Character is not in our alphabet. Skip. */
			} else {
				character = (char) ( temp - alphabet );
			}
		}
		for( i = 0; i < symbolSize; ++i ) {
			READ_BIT( bit, i, character );

			/*
			 * Writing bit.
			 */
			WRITE_BIT( bit, bitPosToWrite, *out );
			++bitPosToWrite;
			if( bitPosToWrite > 7 ) {
				++out;
				bitPosToWrite = 0;
			}
		}
		++in;
		++count;
	}
	return count;
}

int
BitDecode( const char* toDecode, const char* alphabet, char** answer, size_t size, bool memoryAllocated )
{
	int alphabetSize = (int) strlen( alphabet ),    /* Number of symbols in alphabet. */
			symbolSize = (int) log( alphabetSize ) + 2;    /* Bits in one character. */
	int i = 0, j;
	const char* in = toDecode;    /* Input iterator. */
	char bit,    /* Bit that we'll read and write. */
			symbol,    /* Encoded character. */
			* out,    /* Output iterator. */
			bitPosToRead = 0;
	if( !memoryAllocated ) {
		*answer = malloc((size_t) size / symbolSize + 1 );    /* Allocating memory for answer. */
	}
	out = *answer;

	while( i < size ) {
		symbol = 0;
		/* Reading symbolSize bits. */
		for( j = 0; j < symbolSize; ++j ) {
			/* Reading bit to bit. */
			READ_BIT( bit, bitPosToRead, *in );
			++bitPosToRead;
			if( bitPosToRead > 7 ) {
				++in;
				bitPosToRead = 0;
			}

			/* Writing bit to symbol. */
			WRITE_BIT( bit, j, symbol );
		}
		if (symbol == alphabetSize) {
			break;
		}
		*out = alphabet[(unsigned char) symbol];
		++out;
		++i;
	}
	*out = '\0';        /* Adding "end of string" symbol. */
	return i/*(int) ( out - *answer )*/;
}
