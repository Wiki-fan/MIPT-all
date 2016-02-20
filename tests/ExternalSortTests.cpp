#include "../stdafx.h"
#include "../ExternalSort.h"

void TestExternalSort()
{
	my::GenFile();
#ifndef __GNUC__
	my::ExternalSort( L"in", L"target" );
#else
	my::ExternalSort( "in", "target" );
#endif
	printf( "%s", my::CheckFile() ? "OK" : "NOPE" );
}