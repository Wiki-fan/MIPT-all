#pragma once
#include <cstdio>
#include "Vector.h"
#include "Vector.cpp"

namespace my
{

// ������ �����, ������������ �������������. ���, ����� ������ ��� 100 ��. 100*1024*1024
const int IntsInChunk = 100 * 1024 * 1024 / sizeof( type );

// ����.
class CFile
{

public:
	// �����������, ��������� ��� ����� � 1, ���� ���� ������� �������, ���� ��� ���, � ��������, ���� �� ����.
	explicit CFile( const wchar_t* filename, bool write );
	~CFile(); // ����������.

	// ������ � ����� �� �����.
	mysize ReadToArr( type* buffer, mysize count, mysize offset );
	// ������ ���� �� ������.
	mysize WriteFromArr( type* buffer, mysize count, mysize offset );

	// ����� �����.
	bool IsEOF() { feof( f ); }

private:
	FILE* f; // �������� ����������.
};

} // namespace my
