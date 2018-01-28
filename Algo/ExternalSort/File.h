#pragma once
#include "../stdafx.h"

namespace my
{

// Размер куска, сортируемого единовременно. Так, чтобы размер был 100 МБ. 100*1024*1024
const int IntsInChunk = 100 * 1024 * 1024 / sizeof( type );

// Файл.
class CFile {

public:
	// Конструктор, принимает имя файла и 1, коли файл следует создать, если его нет, и обнулить, если он есть.
#ifndef __GNUC__
	explicit CFile( const wchar_t* filename, bool write );
#else
	explicit CFile( const char *filename, bool write );
#endif
	// Деструктор.
	~CFile();

	// Читать в буфер из файла.
	mysize ReadToArr( type *buffer, mysize count, mysize offset );
	// Писать файл из буфера.
	mysize WriteFromArr( type *buffer, mysize count, mysize offset );
	// Конец файла.
	bool IsEOF() const { return bool(feof( f )); }

private:
	FILE *f; // Файловый дескриптор.
};

} // namespace my
