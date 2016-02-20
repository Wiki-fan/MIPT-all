#include "stdafx.h"

void mfassert( char* file, unsigned int line, bool check, char *format, ... )
{
	if( !check ) {
		printf( "ASSERT: file %s, line %u\n\t", file, line );
		va_list args;
		va_start( args, format );
		vprintf( format, args );
		va_end( args );
		std::cin.get();
		exit( 1 );
	}
}

void mfrassert( bool check, ... )
{
	if( !check ) {
		printf( "error" );
		exit( 1 );
	}
}