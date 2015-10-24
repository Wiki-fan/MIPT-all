#include "stdafx.h"
#include "Heap.h"

// Размер буфера по умолчанию, когда используется дефолтный конструктор.
const mysize DefaultInitialSize = 8;

namespace my
{

template<typename T>
CHeap<T>::CHeap() :arr() {}

template<typename T>
CHeap<T>::~CHeap() {}

template<typename T>
CHeap<T>::CHeap( mysize initial_size ) :arr( initial_size ) {}

template<typename T>
CHeap<T>::CHeap( const CHeap& other ) : arr( other.arr ) {}

template<typename T>
const T& CHeap<T>::Top() const
{
	assert( Size() > 0 );
	return arr[0];
}

template<typename T>
void CHeap<T>::Push( const T& element )
{
	arr.PushBack( element );
	siftUp( arr.Size() - 1 );
}

template<typename T>
void CHeap<T>::siftUp( int index )
{
	while( index > 0 ) {
		int parent = (index - 1) / 2;
		if( arr[index] <= arr[parent] )
			return;
		swap( arr[index], arr[parent] );
		index = parent;
	}
}

template<typename T>
const T& CHeap<T>::Pop()
{
	assert( !isEmpty() );
	T top = arr[0];
	arr[0] = arr.Last();
	arr.PopBack();
	if( !isEmpty() ) {
		siftDown( 0 );
	}
	return top;
}

template<typename T>
void CHeap<T>::siftDown( int index )
{
	int top = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;
	if( left < Size() && arr[left] > arr[index] ) {
		top = left;
	}
	if( right < Size() && arr[right] > arr[top] ) {
		top = right;
	}
	if( top != index ) {
		my::swap<T>( arr[top], arr[index] );
		siftDown( top );
	}

	return;
}

/*int HeapTest()
{
	CHeap<int> h1;
	h1.Push( 4 );
	h1.Push( 6 );
	h1.Push( 2 );
	std::cout << h1.Pop() << std::endl;
	std::cout << h1.Top() << std::endl;
	std::cout << h1.Pop() << std::endl;
	std::cout << h1.Pop() << std::endl;
	std::cout << h1.Pop() << std::endl;
	return 0;
}*/

} // namespace my
