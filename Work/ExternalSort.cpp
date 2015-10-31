#include "stdafx.h"
#include "ExternalSort.h"
#include "File.h"
#include "Sorts.h"
#include "HeapForMerge.h"

namespace my
{

// Генерация файла.
void GenFile()
{
	srand( static_cast<unsigned int>(time( 0 )) );

	FILE* outf = fopen( "in", "wb+" );
	const mysize N = 4 * 1024 / sizeof( type );

	// Будем писать числа в буфер, а потом записывать куском в файл.
	const mysize max = 1024 * 1024;
	type *buf = new type[max];
	for( int i = 0; i < N; ++i ) {
		for( int j = 0; j < max; ++j ) {
			buf[j] = rand()*rand()*rand()*rand(); // Нужно больше энтропии.
		}
		fwrite( buf, sizeof( type ), max, outf );
	}
	fclose( outf );
#ifdef STDOUT_CFILE_DEBUG
	perror( "GenFile: " );
#endif
	delete[] buf;
}

// Проверка корректости сортировки файла.
bool CheckFile()
{
	FILE* inf = fopen( "target", "rb" );
	const int N = IntsInChunk;
	type *buf = new type[N];
	type prev = LONG_LONG_MIN; // Первое значение - минимальное из возможных.
	while( mysize size = fread( buf, sizeof( type ), N, inf ) ) {
		for( int i = 0; i < size; ++i ) {
			if( prev > i ) {
				fclose( inf );
				delete[] buf;
				return false;
			}
		}
	}
	fclose( inf );
	delete[] buf;
	return true;
}

// Сравнение двух чисел для qsort'а
int Compare( const void * a, const void * b )
{
	return (int)(*(type*)a - *(type*)b);
}

#ifndef __GLIBC__
void ExternalSort( const wchar_t* sourceFileName, const wchar_t* targetFilename )
#else
void ExternalSort( const char* sourceFileName, const char* targetFilename )
#endif
{
	// Считывание, сотрировка и запись чанков по IntsInChunk int'ов.
	CFile inf( sourceFileName, false );
	type *arr = new type[my::IntsInChunk];
	type *buffer = new type[my::IntsInChunk];
	int i = 0;
	mysize offset = 0;
	while( mysize count = inf.ReadToArr( arr, IntsInChunk, offset ) ) {
		MergeSort<type>( arr, arr + count, buffer );
		// std::qsort( arr, count, sizeof( type ), Compare );
		inf.WriteFromArr( arr, count, offset );
		offset += count;
		++i;
	}

	int chunks = i; // Количество чанков.
	// Размер куска, такой, чтобы в сумме они составляли 100 МБ.
	int IntsInPiece = IntsInChunk / chunks + 1;
	delete[] arr;
	delete[] buffer;

	// Строим кучу.
	CHeapForMerge heap( inf );
	for( int i = 0; i < chunks; ++i ) {
		heap.Push( IntsInPiece, i, IntsInChunk );
	}

	//Сливаем
	type *items = new type[IntsInChunk]; // Будем писать на диск кусками по столько int'ов.
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
