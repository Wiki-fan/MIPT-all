#include "nucleotide_pack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../common/bit_manipulations.h"
#include "../task-s2-gets_safe/safe_string.h"
#include "bit_coding.h"

#define NUCLEOTIDE_GROUP_W 10
#define NUCLEOTIDE_STRING_G 6
int readWhileNotNew( FILE* f, char** str, const char* term, int size)
{
	int bufSize = 1000; /* Initial buffer size. */
	char* buf = (char*)malloc(bufSize*sizeof(char)), *newBuf = NULL;
	int c;
	char* iter = buf;
	int i = 0;
	while ( (c = getc(f)) != EOF) {
		++i;
		/* Realloc if needed. */
		if (i == bufSize-1) {
			bufSize *= 2;
			newBuf = realloc(buf, bufSize*sizeof(char));
			if (newBuf != 0) {
				buf = newBuf;
				iter = buf + i-1;
			} else {
				puts("Error reallocating memory\n");
				exit(1);
			}
		}
		if (c != '\0' && strchr(term, c)) {
			ungetc(c, f);
			break;
		}
		*iter++ = c;
		/* Characters that should terminate input. */
	}
	/* Append '\0' if terminating character was term. */
	if (c != '\0') {
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
			symbolSize = (unsigned char) log( alphabetSize ) + 2;    /* Bits in one character. Also the size of out buffer. */
	char* inBuf, * outBuf;
	size_t count = 0;
	int ret;
	/*int inBufferSize, outBufferSize; /* In bytes. */


	if(( inf = fopen( inFileName, "rb" )) == NULL) {
		fprintf( stderr, "Failed to open file %s.\n", inFileName );
		exit( 1 );
	}
	if(( outf = fopen( outFileName, "wb" )) == NULL) {
		fprintf( stderr, "Failed to open file %s.\n", outFileName );
		exit( 1 );
	}

	while( gets_safe( inf, &header )) {
		/* Copying the header. */
		fputs( header, outf );
		if( direction == 0 ) {
			/*count = readWhileNotNew( inBuf, inBufferSize, inf, alphabet, true);*/
			count = readWhileNotNew( inf, &inBuf, ">", -1);

			/* The last portion should be coded with end symbol. */
			ret = BitEncode( inBuf, alphabet, &outBuf );
			fwrite(&ret, sizeof(int), 1, outf);
			fwrite( outBuf, sizeof( char ), (( ret + 1 ) * 3 ) / 8 + 1, outf );
		} else if( direction == 1 ) {
			int j;
			fread(&ret, sizeof(int), 1, outf);
			/*count = readWhileNotNew( inBuf, inBufferSize, inf, alphabet, false );*/
			count = readWhileNotNew( inf, &inBuf, ">", ret );

			ret = BitDecode( inBuf, alphabet, &outBuf, ret);

			while(ret>0) {
				for (j = 0; j<NUCLEOTIDE_STRING_G && ret>0; ++j) {
					fwrite( outBuf, sizeof( char ),
					        ret>NUCLEOTIDE_GROUP_W?NUCLEOTIDE_GROUP_W:ret, outf );
					ret -= NUCLEOTIDE_GROUP_W;
					putc(' ', outf);
				}
				putc('\n', outf);
			}
			/*fwrite( outBuf, sizeof( char ), ret, outf );*/

		} else {
			fprintf( stderr, "Unknown direction of packing.\n" );
			exit( 1 );
		}
		free( inBuf );
		free( outBuf );
	}

	free( header );
	fclose( inf );
	fclose( outf );
}
