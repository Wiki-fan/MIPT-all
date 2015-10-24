#include "stdafx.h"
#include "HeapForMerge.h"

namespace my
{

void CHeapForMerge::siftDown( mysize index )
{
	mysize top = index;
	mysize left = 2 * index + 1;
	mysize right = 2 * index + 2;
	if( left < arr.size() && arr[left]->getItem() < arr[index]->getItem() ) {
		top = left;
	}
	if( right < arr.size() && arr[right]->getItem() < arr[top]->getItem() ) {
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
	arr.push_back( new CSortedListFromFile( _size, _chunk ) );
	arr.back()->ReadIfNeeded( f, _size, intsInChunk ); // Подгружаем кусок.
	siftUp( arr.size() - 1 );
}

void CHeapForMerge::siftUp( mysize index )
{
	while( index > 0 ) {
		mysize parent = (index - 1) / 2;
		if( arr[index]->getItem() > arr[parent]->getItem() ) {
			return;
		}
		my::swap<CSortedListFromFile*>( arr[index], arr[parent] );
		index = parent;
	}
}

const type CHeapForMerge::Pop( mysize intsInPiece, mysize intsInChunk )
{
	massert( arr.size() != 0 );
	type top = arr[0]->getNext( f, intsInPiece, intsInChunk );
	if( arr[0]->isEnd() ) {
		swap<CSortedListFromFile*>( arr.front(), arr.back() );
		arr.pop_back();
	}
	if( arr.size() != 0 ) {
		siftDown( 0 );
	}
	return top;
}

} // namespace my