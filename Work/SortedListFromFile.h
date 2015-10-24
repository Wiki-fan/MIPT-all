#pragma once
#include "Vector.h"
#include "File.h"

namespace my {

// ������ type'�� � ��������������. ���� ������ �������� �������, size == 0.
class CSortedListFromFile
{
public:
	// �����������.
	CSortedListFromFile( mysize intsInPiece, int _chunk ) :
		size( 0 ),
		buf( new type[intsInPiece] ),
		index( 0 ),
		chunkend( (_chunk + 1)*IntsInChunk ),
		offset( _chunk*IntsInChunk )
	{
	};
	// ����������.
	~CSortedListFromFile();

	// ������� ��������� (� ����������).
	type getNext( my::CFile& f, mysize intsInPiece, mysize intsInChunk );
	//�������� ��� ������������ �������.
	type getItem() { return buf[index]; }
	// ��������, ���� ����.
	void ReadIfNeeded( my::CFile& f, mysize intsInPiece, mysize intsInChunk );

	// ��������, ��� �� ��� ��������� piece ������� chunk'�.
	bool isEnd() const { return (size == 0); }

private:
	type *buf; // ������ � �������.
	mysize index; // ������ �������� ��������.
	mysize size; // ����� ���������.
	mysize offset; // ������� ������� piece'� � chunk'�. 
	mysize chunkend; // ������� ������� ������� ����� � �����.
};

} // namespace my