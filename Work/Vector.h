#pragma once
#include "stdafx.h"

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
	CVector( mysize _size, const T* items );
	// Копирующий конструктор.
	CVector( const CVector& other );
	// Деструктор. 
	virtual ~CVector();

	// Элемент на позиции n.
	const T& operator[]( mysize n ) const;
	T& operator[]( mysize n );

	CVector& operator=( const CVector& other );

	// Добавление элемента в конец вектора.
	virtual void push_back( const T& item );
	// Удаление элемента из конца вектора. В отличие от std::vector возвращает удаляемый элемент.
	virtual T pop_back();
	// Перевыделяет память так, чтобы не было лишней неиспользуемой памяти.
	void shrink_to_fit();

	// Проверка пустоты вектора.
	bool empty() const { return (count == 0); }
	// Указатель на начало.
	T* begin() const { return items; }
	// Указатель на элемент, следующий за последним элементом вектора.
	T* end() const { return &(items[size]); }
	// Последний элемент.
	T& back() const
	{
		massert( !empty() );
		return items[count - 1];
	}
	// Первый элемент.
	T& front() const
	{
		massert( !empty() );
		return items[0];
	}

	// Возвращает размер вектора.
	const mysize size() const { return count; }
	// Возвращает количество памяти, отведённое в данный момент под элементы.
	const mysize max_size() const { return maxCount; }

private:
	mysize count; // Размер (количество хранящихся элементов).
	mysize maxCount; // Максимальный размер, a.k.a. размер буфера items.
	T* items; // Указатель на элементы.

	// Копирование элементов вектора из буфера from в буфер to.
	static void copy( mysize _size, const T *from, T *to );
	// Должна как-нибудь перевыделить память. Пока что не нужна.
	// void realloc( mysize& count, mysize otherCount, T* items, T* otherItems );
};

} // namespace my
