#include "stdafx.h"
#include "Vector.h"

// Размер буфера по умолчанию, когда используется дефолтный конструктор.
const mysize DefaultInitialVectorSize = 8;

namespace my
{

template<typename T>
CVector<T>::CVector() : size( 0 ), maxSize( DefaultInitialVectorSize ), items( new T[maxSize] )
{
}

template<typename T>
CVector<T>::CVector( mysize _size ) :
	size( _size ),
	maxSize( size ),
	items( new T[size] )
{
	assert( size > 0 );
}

template <typename T>
CVector<T>::CVector( mysize _size, T* _items ) :
	size( _size ),
	maxSize( size ),
	items( new T[size] )
{
	for( int i = 0; i < size; ++i )
		items[i] = _items[i];
}

template <typename T>
CVector<T>::CVector( const CVector& other )
{
	if( maxSize < other.size ) {
		delete[] items;
		size = other.size;
		maxSize = other.maxSize;
		items = new T[maxSize];
	}
	for( int i = 0; i < size; ++i ) {
		items[i] = other.items[i];
	}
}

template <typename T>
CVector<T>::~CVector()
{
	delete[] items;
}

template<typename T>
CVector<T>& my::CVector<T>::operator=( const CVector& other )
{
	if( &other == this ) {
		return *this;
	} else {
		delete[] items;
		size = other.size;
		maxSize = other.maxSize;
		items = new T[maxSize];

		for( int i = 0; i < size; ++i ) {
			items[i] = other.items[i];
		}
	}
}

template <typename T>
T CVector<T>::operator[] ( const mysize n ) const
{
	assert( n >= 0 && n < size );
	return items[n];
}

template <typename T>
T& CVector<T>::operator[] ( const mysize n )
{
	assert( n >= 0 && n < size );
	return items[n];
}

template<typename T>
void CVector<T>::PushBack( const T& item )
{
	assert( size <= maxSize );
	if( size == maxSize ) {
		maxSize = 2 * maxSize;
		T* new_items = new T[maxSize];

		for( int i = 0; i < size; ++i ) {
			new_items[i] = items[i];
		}
		new_items[size++] = item;
		delete[] items;
		items = new_items;
	} else {
		items[size++] = item;
	}
}

template<typename T>
T CVector<T>::PopBack()
{
	assert( size > 0 );
	--size;
	return items[size];
}

template<typename T>
void CVector<T>::copy( mysize _size, const T *from, T *to )
{
	for( int i = 0; i < _size; ++i ) {
		to[i] = from[i];
	}
}

} // namespace my
