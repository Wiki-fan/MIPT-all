#pragma once
#include "SortedListFromFile.h"

namespace my
{

// MinHeap для внешней сортировки.
class CHeapForMerge {

public:
	// Конструктор.
	explicit CHeapForMerge( CFile& inf ) : f( inf ) {}

	// Добавление элемента в кучу.
	void Push( mysize _size, int _chunk, mysize intsInChunk );
	// Извлечение элемента из кучи.
	const type Pop( mysize intsInPiece, mysize intsInChunk );
	// Пуста ли куча?
	bool isEmpty() const { return arr.empty(); }

private:
	my::CVector<CSortedListFromFile*> arr; // Массив, хранящий элементы кучи.
	CFile f; // Файл.

	// Сдвинуть вверх.
	void siftUp( mysize index ); 
	// Сдвинуть вниз.
	void siftDown( mysize index ); 
};

} // namespace my
