#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "Smth.h"

// Размер буфера по умолчанию, когда используется дефолтный конструктор.
const mysize DefaultInitialVectorSize = 8;

namespace my
{

template<typename T>
CVector<T>::CVector() : count( 0 ), maxCount( DefaultInitialVectorSize ), items( new T[maxCount] )
{
}

template<typename T>
CVector<T>::CVector( mysize _size ) :
	count( _count ),
	maxCount( count ),
	items( new T[count] )
{
	massert( count > 0 );
}

template <typename T>
CVector<T>::CVector( mysize _count, const T* _items ) :
	count( _count ),
	maxCount( count ),
	items( new T[count] )
{
	copy( count, _items, items );
}

template <typename T>
CVector<T>::CVector( const CVector& other )
{
	if( maxCount < other.count ) {
		delete[] items;
		maxCount = other.maxCount;
		items = new T[maxCount];
	}
	count = other.count;
	copy( count, other.items, items );
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
		if( maxCount < other.count ) {
			delete[] items;
			maxCount = other.maxCount;
			items = new T[maxCount];
		}
		count = other.count;
		copy( count, other.items, items );
	}
}

template <typename T>
const T& CVector<T>::operator[] ( mysize n ) const
{
	massert( n >= 0 && n < count );
	return items[n];
}

template <typename T>
T& CVector<T>::operator[] ( mysize n )
{
	massert( n >= 0 && n < count );
	return items[n];
}

template<typename T>
void CVector<T>::push_back( const T& item )
{
	massert( count <= maxCount );
	if( count == maxCount ) {
		maxCount = 2 * maxCount;
		T* new_items = new T[maxCount];

		copy( count, items, new_items );

		new_items[count++] = item;
		delete[] items;
		items = new_items;
	} else {
		items[count++] = item;
	}
}

template<typename T>
T CVector<T>::pop_back()
{
	massert( count > 0 );
	if( count <= maxCount / 2 ) {
		shrink_to_fit();
	}

	return items[--count];
}

template<typename T>
void CVector<T>::copy( mysize _count, const T *from, T *to )
{
	for( int i = 0; i < _count; ++i ) {
		to[i] = from[i];
	}
}

template<typename T>
void CVector<T>::shrink_to_fit()
{
	maxCount = max<mysize>( count + 1, DefaultInitialVectorSize );// На всякий случай, если будет много push_back'ов и pop_back'ов на этом месте.
	T* new_items = new T[maxCount];

	copy( count, items, new_items );

	delete[] items;
	items = new_items;
}

} // namespace my

