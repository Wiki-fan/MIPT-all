#pragma once
#include <cassert>

namespace my
{

// Динамический вектор.
template<typename T>
class CVector
{
public:
	// Дефолтный конструктор.
	CVector(); 
	// Создаёт вектор размера _size
	CVector( mysize _size ); 
	// Создаёт вектор размера _size и копирует туда _size элементов _items.
	CVector( mysize _size, T* items ); 
	// Копирующий конструктор.
	CVector( const CVector& other ); 
	// Деструктор. 
	~CVector();

	T operator[]( const mysize n ) const;
	T& operator[]( const mysize n );

	CVector& operator=( const CVector& other );

	// Добавление элемента в конец вектора.
	void PushBack( const T& item );
	// Удаление элемента из конца вектора.
	T PopBack();
	// Проверка пустоты вектора.
	bool isEmpty() const { return (size == 0); }
	// Указатель на начало.
	T* Begin() const { return items; }
	// Указатель на элемент, следующий за последним элементом вектора.
	T* End() const { return &(items[size]); }
	// Последний элемент.
	T& Last() 
	{
		assert( !isEmpty() );
		return items[size - 1];
	}
	// Первый элемент.
	T& First() 
	{
		assert( !isEmpty() );
		return items[0];
	}

	// Возвращает размер вектора.
	const mysize Size() const { return size; }
	// Возвращает количество памяти, отведённое в данный момент под элементы.
	const mysize MaxSize() const { return maxSize; }

private:
	mysize size;	// Размер (количество хранящихся элементов).
	mysize maxSize;	// Максимальный размер, a.k.a. размер буфера items.
	T* items;	// Указатель на элементы.

	// Копирование элементов вектора из буфера from в буфер to.
	void copy( mysize _size, const T *from, T *to );
};

} // namespace my
