#pragma once
#include "SortedListFromFile.h"
#include "Smth.h"
#include "Smth.cpp"

namespace my
{

// MinHeap ��� ������� ����������.
class CHeapForMerge
{
public:
	// �����������.
	CHeapForMerge( CFile& inf ) : f( inf ) {};

	// ���������� �������� � ����.
	void Push( mysize _size, int _chunk, mysize intsInChunk );
	// ���������� �������� �� ����.
	const type Pop( mysize intsInPiece, mysize intsInChunk );
	// ����� �� ����?
	bool isEmpty() { return arr.isEmpty(); }
private:
	my::CVector<CSortedListFromFile*> arr; // ������, �������� �������� ����.
	CFile f; // ����.
	void siftUp( int index ); // �������� �����.
	void siftDown( int index ); // �������� ����.
};

} // namespace my