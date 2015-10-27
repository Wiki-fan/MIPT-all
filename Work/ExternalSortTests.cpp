#include "stdafx.h"
#include "ExternalSort.h"

void TestExternalSort()
{
	my::GenFile();
	my::ExternalSort( L"in", L"target" );
	printf( "%s", my::CheckFile() ? "OK" : "NOPE" );
}