#pragma once
#include "SortedListFromFile.h"
#include "Smth.h"
#include "Smth.cpp"

namespace my
{

// MinHeap для внешней сортировки.
class CHeapForMerge
{
public:
	// Конструктор.
	CHeapForMerge( CFile& inf ) : f( inf ) {};

	// Добавление элемента в кучу.
	void Push( mysize _size, int _chunk, mysize intsInChunk );
	// Извлечение элемента из кучи.
	const type Pop( mysize intsInPiece, mysize intsInChunk );
	// Пуста ли куча?
	bool isEmpty() { return arr.empty(); }
private:
	my::CVector<CSortedListFromFile*> arr; // Массив, хранящий элементы кучи.
	CFile f; // Файл.
	void siftUp( mysize index ); // Сдвинуть вверх.
	void siftDown( mysize index ); // Сдвинуть вниз.
};

} // namespace my