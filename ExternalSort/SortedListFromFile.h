#pragma once
#include "../StandardDataStructures/Vector.h"
#include "File.h"

namespace my {

// Массив type'ов с автоподгрузкой. Если больше неоткуда грузить, size == 0.
class CSortedListFromFile
{
public:
	// Конструктор.
	CSortedListFromFile( mysize intsInPiece, int _chunk ) :
		size( 0 ),
		buf( new type[intsInPiece] ),
		index( 0 ),
		chunkend( (_chunk + 1)*IntsInChunk ),
		offset( _chunk*IntsInChunk )
	{
	}
	// Деструктор.
	~CSortedListFromFile();

	// Извлечь следующий (с подгрузкой).
	type getNext( my::CFile& f, mysize intsInPiece, mysize intsInChunk );
	// Дочитать, если надо.
	void ReadIfNeeded( my::CFile& f, mysize intsInPiece, mysize intsInChunk );

	//Получить уже подгруженный элемент.
	type getItem() const { return buf[index]; }
	// Проверка, был ли это последний piece данного chunk'а.
	bool isEnd() const { return (size == 0); }

private:
	mysize size; // Всего элементов.
	type *buf; // Массив с числами.
	mysize index; // Индекс текущего элемента.
	mysize chunkend; // Верхняя граница нужного чанка в файле.
	mysize offset; // Текущая позиция piece'а в chunk'е. 

	// Не нужен и не должен вызываться.
	CSortedListFromFile( const CSortedListFromFile& other ) = delete;
};

} // namespace my