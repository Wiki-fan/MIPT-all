#include "stdafx.h"

#include "Sorts.h"
#include "Smth.h"
#include "Vector.h"
#include "Vector.cpp"
#include "Heap.h"
#include "Heap.cpp"
#include "ExternalSort.h"

int main()
{
	my::GenFile();
	my::ExternalSort( L"in", L"target" );
	printf( "%s", my::CheckFile() ? "OK" : "NOPE" );
	return 0;
}
