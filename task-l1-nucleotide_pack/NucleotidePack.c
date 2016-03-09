#include "NucleotidePack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../common/BitManipulations.h"
#include "../task-s2-gets_safe/SafeString.h"

void nucleotide_pack( const char* inFileName, const char* outFileName )
{
	FILE* inf, * outf;
	char* header;
	char* alphabet = "atgcu";
	const unsigned char alphabetSize = (unsigned char)strlen( alphabet ),    /* Number of symbols in alphabet. */
			symbolSize =  (unsigned char)log( alphabetSize ) + 2;    /* Bits in one character. */
	int in; /* Input character. */
	char character; /* Encoded character. */
	char out;    /* Output character. */
	char bit,  /* Bit that we'll read and write. */
			bitPosToWrite = 0; /* Position of bit. */

	char i;
	bool fl = true;
	char* temp;

	if(( inf = fopen( inFileName, "rb" )) == NULL) {
		fprintf( stderr, "Failed to open file %s.\n", inFileName );
		exit( 1 );
	}
	if(( outf = fopen( outFileName, "wb" )) == NULL) {
		fprintf( stderr, "Failed to open file %s.\n", outFileName );
		exit( 1 );
	}

	/* Copying the header. */
	gets_safe( inf, &header );
	fputs( header, outf );

	while( fl ) {
		in = getc( inf );
		/* Integer that will represent character encoded. */
		if( in == EOF) {
			character = alphabetSize; /* End character. */
			fl = false;
		} else {
			if(( temp = strchr( alphabet, in )) == NULL) {
				fprintf( stderr, "Character %c is not in the alphabet.\n", in );
				fclose( inf );
				fclose( outf );
				exit( 1 );
			}
			character = (char) (temp - alphabet);
		}
		for( i = 0; i < symbolSize; ++i ) {
			READ_BIT( bit, i, character );

			/* Writing bit. */
			WRITE_BIT( bit, bitPosToWrite, out );
			++bitPosToWrite;
			if( bitPosToWrite > 7 ) {
				bitPosToWrite = 0;
				fputc(out, outf);
			}
		}
	}

	if (bitPosToWrite != 0) {
		fputc(out, outf);
	}

	free( header );
	fclose( inf );
	fclose( outf );
}

void nucleotide_unpack( const char* inFileName, const char* outFileName )
{
	FILE* inf, * outf;
	char* header;
	int in; /* Input character. */
	const char* alphabet = "atgcu";
	const unsigned char alphabetSize = (unsigned char)strlen( alphabet ),    /* Number of symbols in alphabet. */
			symbolSize =  (unsigned char)log( alphabetSize ) + 2;    /* Bits in one character. */
	unsigned char i;
	char bit,    /* Bit that we'll read and write. */
			out = 0,    /* Encoded character. */
			bitPosToRead = 0; /* Position of bit. */
	bool fl = true;

	if(( inf = fopen( inFileName, "rb" )) == NULL) {
		fprintf( stderr, "Failed to open file %s.\n", inFileName );
		exit( 1 );
	}
	if(( outf = fopen( outFileName, "wb" )) == NULL) {
		fprintf( stderr, "Failed to open file %s.\n", outFileName );
		exit( 1 );
	}

	/* Copying the header. */
	gets_safe( inf, &header );
	fputs( header, outf );

	in = getc( inf );
	/*c = getc(inf);*/
	if( in == EOF) {
		fprintf(stderr, "There is no sequence data in file %s. \n", inFileName );
		exit( 1 );
	}

	while( fl ) {
		out = 0;

		/* Reading symbolSize bits. */
		for( i = 0; i < symbolSize; ++i ) {
			/* Reading bit to bit. */
			READ_BIT( bit, bitPosToRead, in );
			++bitPosToRead;
			if( bitPosToRead > 7 ) {
				in = getc( inf );
				if( in == EOF) {
					fprintf(stderr, "End of file reached before end character. Sequence is coded incorrectly.\n" );
					exit( 1 );
				}
				bitPosToRead = 0;
			}

			/* Writing bit to out. */
			WRITE_BIT( bit, i, out );
		}
		if( out == alphabetSize ) {
			fl = false;
		} else {
			putc( alphabet[(unsigned char) out], outf );
		}
	}

	free( header );
	fclose( inf );
	fclose( outf );
}
