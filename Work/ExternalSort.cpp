#include "stdafx.h"
#include <ctime>
#include <cstdlib>
#include <limits.h>
#include <algorithm>
#include "ExternalSort.h"
#include "File.h"
#include "Sorts.h"
#include "HeapForMerge.h"

namespace my
{

// ��������� �����.
void GenFile()
{
	srand( static_cast<unsigned int>(time( 0 )) );

	FILE* outf = fopen( "in", "wb+" );
	const mysize N = 4 * 1024 * 1024 / sizeof( type ); // ������� type'�� ������ 4 ��.

	// ����� ������ ����� � �����, � ����� ���������� ������ � ����.
	const mysize max = 1024;
	type *buf = new type[max];
	for( int i = 0; i < N; ++i ) {
		for( int j = 0; j < max; ++j ) {
			buf[j] = rand()*rand()*rand()*rand(); // ����� ������ ��������.
		}
		int count = fwrite( buf, sizeof( type ), max, outf );
	}
	fclose( outf );
#ifdef STDOUT_DEBUG
	perror( "GenFile: " );
#endif
	delete[] buf;
}

// �������� ����������� ���������� �����.
bool CheckFile()
{
	FILE* inf = fopen( "target", "rb" );
	const int N = IntsInChunk;
	type *buf = new type[N];
	int prev = _I64_MIN; // ������ �������� - ����������� �� ���������.
	while( int size = fread( buf, sizeof( type ), N, inf ) ) {
		for( int i = 0; i < size; ++i ) {
			if( prev > i ) {
				fclose( inf );
				delete buf;
				return false;
			}
		}
	}
	fclose( inf );
	delete buf;
	return true;

}

// ��������� ���� ����� ��� qsort'�
int Compare( const void * a, const void * b )
{
	return (int)(*(type*)a - *(type*)b);
}

void ExternalSort( const wchar_t* sourceFileName, const wchar_t* targetFilename )
{
	// ����������, ���������� � ������ ������ �� IntsInChunk int'��.
	CFile inf( sourceFileName, false );
	type *arr = new type[my::IntsInChunk];
	type *buffer = new type[my::IntsInChunk];
	int i = 0;
	mysize offset = 0;
	while( mysize count = inf.ReadToArr( arr, IntsInChunk, offset ) ) {
		// MergeSort<type>( arr, arr + count, buffer );
		std::qsort( arr, count, sizeof( type ), Compare );
		mysize written = inf.WriteFromArr( arr, count, offset );
		// printf( "%d %d\n", count, written );
		offset += count;
		++i;
	}

	int chunks = i; // ���������� ������.
	// ������ �����, �����, ����� � ����� ��� ���������� 100 ��.
	int IntsInPiece = IntsInChunk / chunks + 1;
	delete[] arr;
	delete[] buffer;

	// ������ ����.
	CHeapForMerge heap( inf );
	for( int i = 0; i < chunks; ++i ) {
		heap.Push( IntsInPiece, i, IntsInChunk );
	}

	//�������
	type *items = new type[IntsInChunk]; // ����� ������ �� ���� ������� �� ������� int'��.
	offset = 0;
	CFile outf( targetFilename, true );
	while( !heap.isEmpty() ) {
		int i = 0;
		for( ; i < IntsInChunk && !heap.isEmpty(); ++i ) {
			items[i] = heap.Pop( IntsInPiece, IntsInChunk );
			// printf( "%d %lld\n", i, items[i] );
		}
		// printf( "%d \n", i );
		outf.WriteFromArr( items, i, offset );
		offset += i;
	}
	delete[] items;
}

} // namespace my
