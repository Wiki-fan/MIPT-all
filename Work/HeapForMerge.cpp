#include "stdafx.h"
#include "HeapForMerge.h"

namespace my
{

void CHeapForMerge::siftDown( int index )
{
	int top = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;
	if( left < arr.Size() && arr[left]->getItem() < arr[index]->getItem() ) {
		top = left;
	}
	if( right < arr.Size() && arr[right]->getItem() < arr[top]->getItem() ) {
		top = right;
	}
	if( top != index ) {
		swap<CSortedListFromFile*>( arr[top], arr[index] );
		siftDown( top );
	}

	return;
}

void CHeapForMerge::Push( mysize _size, int _chunk, mysize intsInChunk )
{
	arr.PushBack( new CSortedListFromFile( _size, _chunk ) );
	arr.Last()->ReadIfNeeded( f, _size, intsInChunk ); // Подгружаем кусок.
	siftUp( arr.Size() - 1 );
}

void CHeapForMerge::siftUp( int index )
{
	while( index > 0 ) {
		int parent = (index - 1) / 2;
		if( arr[index]->getItem() > arr[parent]->getItem() ) {
			return;
		}
		my::swap<CSortedListFromFile*>( arr[index], arr[parent] );
		index = parent;
	}
}

const type CHeapForMerge::Pop( mysize intsInPiece, mysize intsInChunk )
{
	assert( arr.Size() != 0 );
	type top = arr[0]->getNext( f, intsInPiece, intsInChunk );
	if( arr[0]->isEnd() ) {
		swap<CSortedListFromFile*>( arr.First(), arr.Last() );
		arr.PopBack();
	}
	if( arr.Size() != 0 ) {
		siftDown( 0 );
	}
	return top;
}

} // namespace my