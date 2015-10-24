#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "Smth.h"
#include "Smth.cpp"

namespace my {

// Min-куча
template<typename T>
class CHeap
{

public:
	CHeap(); // Конструктор по умолчанию создаёт пустую кучу.
	explicit CHeap( mysize initialSize ); // Создаёт кучу размера initial_size.
	CHeap( const CHeap& other ); // Конструктор копирования.
	~CHeap(); // Деструктор.

	//CHeap& operator=( const CHeap& other );

	bool isEmpty() const { return (arr.Size() == 0); } // Проверка пустоты.
	mysize Size() const { return arr.Size(); } // Размер кучи.
	const T& Top() const; // Вершина.
	void Push( const T& element ); // Вставка элемента.
	virtual const T& Pop(); //Извлечение вершины.

protected:
	CVector<T> arr; // Массив, хранящий элементы кучи.

	void siftUp( int index ); // Просеять вверх.
	void siftDown( int index ); // Просеять вниз.
};

} // namespace my
