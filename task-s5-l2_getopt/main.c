#include <stdio.h>
#include <string.h>
#include <err.h>
#include <getopt.h>
#include <stdlib.h>
#define PACK_NUCLEOTIDS 0
#define UNPACK_NUCLEOTIDS 1

int main( int argc, char* argv[] )
{
	char mode;
	int mode_optind = -1; /* Position of mode in options */
	char* in, * out;
	int optind = 0;

	int c;

	do {
		static struct option long_options[] =
				{
						{ "help",    no_argument,       0, 'h' },
						{ "version", no_argument,       0, 'v' },
						{ "pack",    required_argument, 0, 'p' },
						{ "unpack",  required_argument, 0, 'u' },
						{ 0, 0,                         0, 0 }
				};
		c = getopt_long( argc, argv, "hv", long_options, &optind );

		switch( c ) {
			case 'h':
				puts( "Usage:\n./nucleotide_pack --[pack/unpack] in_file out_file\n./nucleotide_pack -h\n./nucleotide_pack -v" );
				exit( 0 );
			case 'v':
				puts( "Version 0.1" );
				exit( 0 );
			case 'p':
				if( mode_optind != -1 ) {
					errx( 21, "You can't specify both modes simultaneously" );
				}
				mode = PACK_NUCLEOTIDS;
				mode_optind = optind;
				optind++;
				if( optind < argc && *argv[optind] != '-' ) {
					in = optarg;
					out = argv[optind];
				}
				else {
					errx( 20, "--pack option requires TWO arguments" );
				}
				break;

			case 'u':
				if( mode_optind != -1 ) {
					errx( 21, "You can't specify both modes simultaneously" );
				}
				mode = UNPACK_NUCLEOTIDS;
				mode_optind = optind;
				optind++;
				if( optind < argc && *argv[optind] != '-' ) {
					in = optarg;
					out = argv[optind];
				}
				else {
					errx( 20, "--unpack option requires TWO arguments" );
				}
				printf( "Unpacking nucleotids from %s to %s\n", in, out );
				printf( "Unpacked successful\n" );
				break;

			case '?':
				/* getopt_long writes error by itself */
				errx( 11, "Unknown parameter" );

			case -1:
				break;

			default:
				err( 3, "Unreachable code" );
		}
	}
	while( c != -1 );

	if( optind == 0 ) {
		printf( "Nucleotide packing.\nFor help see ./nucleotide_pack -h\n" );
		return 0;
	}
	printf( "%sacking nucleotids from %s to %s\n", mode == PACK_NUCLEOTIDS ? "P" : "Unp", in, out );
	/*nucleotide_pack(mode, in, out);*/
	printf( "%sacked successful\n", mode == PACK_NUCLEOTIDS ? "P" : "Unp" );


	return 0;
}