#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "Smth.h"
#include "Smth.cpp"

namespace my {

// Min-����
template<typename T>
class CHeap
{

public:
	CHeap(); // ����������� �� ��������� ������ ������ ����.
	explicit CHeap( mysize initialSize ); // ������ ���� ������� initial_size.
	CHeap( const CHeap& other ); // ����������� �����������.
	~CHeap(); // ����������.

	//CHeap& operator=( const CHeap& other );

	bool isEmpty() const { return (arr.Size() == 0); } // �������� �������.
	mysize Size() const { return arr.Size(); } // ������ ����.
	const T& Top() const; // �������.
	void Push( const T& element ); // ������� ��������.
	virtual const T& Pop(); //���������� �������.

protected:
	CVector<T> arr; // ������, �������� �������� ����.

	void siftUp( int index ); // �������� �����.
	void siftDown( int index ); // �������� ����.
};

} // namespace my
