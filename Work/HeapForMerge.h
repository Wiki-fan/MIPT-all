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
	bool isEmpty() { return arr.empty(); }
private:
	my::CVector<CSortedListFromFile*> arr; // ������, �������� �������� ����.
	CFile f; // ����.
	void siftUp( mysize index ); // �������� �����.
	void siftDown( mysize index ); // �������� ����.
};

} // namespace my