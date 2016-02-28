#pragma once

#include "stdafx.h"
#include "Smth.h"

namespace my
{

// Размер буфера по умолчанию, когда используется дефолтный конструктор.
const mysize DefaultInitialVectorSize = 8;

// Динамический вектор.
template<typename T>
class CVector {

public:
	// Дефолтный конструктор.
	CVector();
	// Создаёт вектор размера _size
	explicit CVector( mysize _size );
	// Создаёт вектор размера _size и копирует туда _size элементов _items.
	CVector( mysize _size, const T* items );
	// Копирующий конструктор.
	CVector( const CVector& other );
	// Деструктор.
	virtual ~CVector();

	// Ссылка на элемент на позиции n.
	T& operator[]( mysize n );
	// Оператор присваивания.
	CVector& operator=( const CVector& other );
	// Добавление элемента в конец вектора.
	virtual void push_back( const T& item );
	// Удаление элемента из конца вектора. В отличие от std::vector возвращает удаляемый элемент.
	virtual T pop_back();
	// Вставка элемента со значением k на позицию i.
	void Insert( int i, const T& k );
	// Перевыделяет память так, чтобы не было лишней неиспользуемой памяти.
	void shrink_to_fit();
	// Проверка пустоты вектора.
	bool empty() const { return ( count == 0 ); }
	// Указатель на начало.
	T* begin() const { return items; }
	// Указатель на элемент, следующий за последним элементом вектора.
	T* end() const { return &( items[count] ); }
	// Первый элемент.
	T& front() const
	{
		massert( !empty());
		return items[0];
	}
	// Последний элемент.
	T& back() const
	{
		massert( !empty());
		return items[count - 1];
	}

	// Возвращает размер вектора.
	const mysize size() const { return count; }
	// Возвращает количество памяти, отведённое в данный момент под элементы.
	const mysize max_size() const { return maxCount; }
	// Константная ссылка на элемент на позиции n.
	const T& operator[]( mysize n ) const;

private:
	mysize count; // Размер (количество хранящихся элементов).
	mysize maxCount; // Максимальный размер, a.k.a. размер буфера items.
	T* items; // Указатель на элементы.

	// Копирование элементов вектора из буфера from в буфер to.
	static void copy( mysize _size, const T* from, T* to );
	// Должна как-нибудь перевыделить память. Пока что не нужна.
	// void realloc( mysize& count, mysize otherCount, T* items, T* otherItems );
};

template<typename T>
CVector<T>::CVector() : count( 0 ), maxCount( DefaultInitialVectorSize ), items( new T[maxCount] )
{
}

template<typename T>
CVector<T>::CVector( mysize _count )
{
	massert( count > 0 );
	count = _count;
	maxCount = count;
	items = new T[count];
}

template<typename T>
CVector<T>::CVector( mysize _count, const T* _items ) :
		count( _count ),
		maxCount( count ),
		items( new T[count] )
{
	copy( count, _items, items );
}

template<typename T>
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

template<typename T>
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
	return *this;
}

template<typename T>
const T& CVector<T>::operator[]( mysize n ) const
{
	massert( n >= 0 && n < count );
	return items[n];
}

template<typename T>
T& CVector<T>::operator[]( mysize n )
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
void CVector<T>::copy( mysize _count, const T* from, T* to )
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

template<typename T>
void CVector<T>::Insert( int i, const T& k )
{
	push_back( back());
	for( int j = size() - 1 - 1; j > i; --i ) {
		items[j] = items[j - 1];
	}
	items[i] = k;
}
} // namespace my
