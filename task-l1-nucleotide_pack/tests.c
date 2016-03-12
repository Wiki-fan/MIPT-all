#include "Tests.h"
#include "BitCoding.h"
#include "NucleotidePack.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool TestBitEncodeDecode()
{
	const char* alphabet = "atgcu";
	const char* str = "atgcuuuuat";
	char* ans;
	char* ans2;
	int count,
			count2;
	bool ret;

	count = BitEncode( str, alphabet, &ans, false, false );
	printf( "%d: %s\n", count, ans);

	count2 = BitDecode( ans, alphabet, &ans2, count, false );
	printf( "%d: %s\n", count2, ans2 );

	ret = !(strcmp(str, ans2));

	free( ans );
	free( ans2 );

	return ret;
}

extern void TestL1()
{
	nucleotide_pack(0, "in.fasta", "out.gfasta" );
	nucleotide_pack(1, "out.gfasta", "res.fasta" );
}