#pragma once
#include "Vector.h"
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
	};
	// Деструктор.
	~CSortedListFromFile();

	// Извлечь следующий (с подгрузкой).
	type getNext( my::CFile& f, mysize intsInPiece, mysize intsInChunk );
	//Получить уже подгруженный элемент.
	type getItem() { return buf[index]; }
	// Дочитать, если надо.
	void ReadIfNeeded( my::CFile& f, mysize intsInPiece, mysize intsInChunk );

	// Проверка, был ли это последний piece данного chunk'а.
	bool isEnd() const { return (size == 0); }

private:
	type *buf; // Массив с числами.
	mysize index; // Индекс текущего элемента.
	mysize size; // Всего элементов.
	mysize offset; // Текущая позиция piece'а в chunk'е. 
	mysize chunkend; // Верхняя граница нужного чанка в файле.
};

} // namespace my