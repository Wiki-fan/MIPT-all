#pragma once
#include "../stdafx.h"
#include "Vector.h"
#include "../Other/Smth.h"

// Min-куча
template<typename T>
class CHeap{

public:
	// Конструктор по умолчанию создаёт пустую кучу.
	CHeap(); 
	// Создаёт кучу размера initial_size.
	explicit CHeap( mysize initialSize ); 
	// Конструктор копирования.
	CHeap( const CHeap& other ); 
	// Деструктор.
	~CHeap(); 

	// Вставка элемента.
	void Push( const T& element ); 
	//Извлечение вершины.
	T Pop(); 
	// Проверка пустоты.
	bool isEmpty() const { return (Size() == 0); } 
	// Размер кучи.
	mysize Size() const { return arr.size(); }
	// Вершина.
	const T& Top() const; 
	
private:
	CVector<T> arr; // Массив, хранящий элементы кучи.

	// Просеять вверх.
	void siftUp( mysize index );
	// Просеять вниз.
	void siftDown( mysize index );
};

// Размер буфера по умолчанию, когда используется дефолтный конструктор.
const mysize DefaultInitialSize = 8;

template<typename T>
CHeap<T>::CHeap() :arr() {}

template<typename T>
CHeap<T>::~CHeap() {}

template<typename T>
CHeap<T>::CHeap( mysize initial_size ) :arr( initial_size ) {}

template<typename T>
CHeap<T>::CHeap( const CHeap& other ) : arr( other.arr ) {}

template<typename T>
void CHeap<T>::Push( const T& element )
{
	arr.push_back( element );
	siftUp( arr.size() - 1 );
}

template<typename T>
T CHeap<T>::Pop()
{
	massert( !isEmpty() );
	T top = arr[0];
	arr[0] = arr.back();
	arr.pop_back();
	if( !isEmpty() ) {
		siftDown( 0 );
	}
	return top;
}

template<typename T>
const T& CHeap<T>::Top() const
{
	massert( Size() > 0 );
	return arr[0];
}

template<typename T>
void CHeap<T>::siftUp( mysize index )
{
	while( index > 0 ) {
		mysize parent = (index - 1) / 2;
		if( arr[index] <= arr[parent] ) {
			return;
		}
		std::swap( arr[index], arr[parent] );
		index = parent;
	}
}

template<typename T>
void CHeap<T>::siftDown( mysize index )
{
	mysize top = index;
	mysize left = 2 * index + 1;
	mysize right = 2 * index + 2;
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
}