#include "NucleotidePack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../common/BitManipulations.h"
#include "../task-s2-gets_safe/SafeString.h"
#include "BitCoding.h"

int readWhileNotNew( char* buf, int bufSize, FILE* inf, const char* alphabet, bool skipOther )
{
	int count;
	int c;
	for( count = 0; count < bufSize; ++count ) {
		c = getc( inf );
		if( c == EOF) {
			return count;
		}
		/* New sequence check. */
		if( c == '>' ) {
			ungetc( c, inf );
			return count;
		}
		/* Cut characters that are not in alphabet. */
		if (skipOther &&!strchr(alphabet, c)) {
			continue;
		}
		*(buf++) = c;
	}
	return count;
}

void nucleotide_pack( char direction, const char* inFileName, const char* outFileName )
{
	FILE* inf, * outf;
	char* header;
	char* alphabet = "atgcu";
	const unsigned char alphabetSize = (unsigned char) strlen( alphabet ),    /* Number of symbols in alphabet. */
			symbolSize = (unsigned char) log( alphabetSize ) + 2;    /* Bits in one character. Also the size of out buffer. */
	char* inBuf, * outBuf;
	size_t count = 0;
	int ret;
	int inBufferSize, outBufferSize; /* In bytes. */

	if(( inf = fopen( inFileName, "rb" )) == NULL) {
		fprintf( stderr, "Failed to open file %s.\n", inFileName );
		exit( 1 );
	}
	if(( outf = fopen( outFileName, "wb" )) == NULL) {
		fprintf( stderr, "Failed to open file %s.\n", outFileName );
		exit( 1 );
	}

	if( direction == 0 ) {
		inBufferSize = 8;
		outBufferSize = symbolSize;
	} else if( direction == 1 ) {
		inBufferSize = symbolSize;
		outBufferSize = 8;
	} else {
		fprintf( stderr, "Unknown direction of packing.\n" );
		exit( 1 );
	}

	outBuf = (char*) malloc( inBufferSize * sizeof( char ));
	inBuf = (char*) malloc( outBufferSize * sizeof( char ));
	if( inBuf == NULL || outBuf == NULL) {
		printf( stderr, "Troubles with memory allocation.\n" );
		exit( 1 );
	}

	while( gets_safe( inf, &header )) {
		/* Copying the header. */
		fputs( header, outf );
		if( direction == 0 ) {
			do {
				count = readWhileNotNew( inBuf, inBufferSize, inf, alphabet, true);

				/* The last portion should be coded with end symbol. */
				ret = BitEncode( inBuf, alphabet, &outBuf, count == inBufferSize ? true : false , true);
				fwrite( outBuf, sizeof( char ), (ret*3)/8+1, outf );
			} while( count == inBufferSize );
		} else if( direction == 1 ) {
			bool fl = true;
			while( fl ) {
				count = readWhileNotNew( inBuf, inBufferSize, inf, alphabet, false );

				ret = BitDecode( inBuf, alphabet, &outBuf, outBufferSize, true);
				/*if (count == 0 && ret < outBufferSize ) {
					fprintf(stderr, "Missing end-of-sequence symbol.\n");
					exit(1);
				}*/
				if (ret < outBufferSize) {
					fl = false;
				}
				fwrite( outBuf, sizeof( char ), ret, outf );
			}
		}
	}

	free( header );
	free( inBuf );
	free( outBuf );
	fclose( inf );
	fclose( outf );
}
