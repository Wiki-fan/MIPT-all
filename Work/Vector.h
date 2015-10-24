#pragma once
#include "stdafx.h"

namespace my
{

// ������������ ������.
template<typename T>
class CVector
{
public:
	// ��������� �����������.
	CVector(); 
	// ������ ������ ������� _size
	CVector( mysize _size );
	// ������ ������ ������� _size � �������� ���� _size ��������� _items.
	CVector( mysize _size, const T* items );
	// ���������� �����������.
	CVector( const CVector& other );
	// ����������. 
	virtual ~CVector();

	// ������� �� ������� n.
	const T& operator[]( mysize n) const;
	T& operator[]( mysize n );

	CVector& operator=( const CVector& other );

	// ���������� �������� � ����� �������.
	virtual void push_back( const T& item );
	// �������� �������� �� ����� �������. �� ��������� � �������� � STL, �.�. ���������� ��������� �������.
	virtual T pop_back();
	// ������������ ������ ���, ����� �� ���� ������ �������������� ������.
	void shrink_to_fit();

	// �������� ������� �������.
	bool empty() const { return (count == 0); }
	// ��������� �� ������.
	T* begin() const { return items; }
	// ��������� �� �������, ��������� �� ��������� ��������� �������.
	T* end() const { return &(items[size]); }
	// ��������� �������.
	T& back() const
	{
		massert( !empty() );
		return items[count - 1];
	}
	// ������ �������.
	T& front() const
	{
		massert( !empty() );
		return items[0];
	}

	// ���������� ������ �������.
	const mysize size() const { return count; }
	// ���������� ���������� ������, ��������� � ������ ������ ��� ��������.
	const mysize max_size() const { return maxCount; }

private:
	mysize count; // ������ (���������� ���������� ���������).
	mysize maxCount; // ������������ ������, a.k.a. ������ ������ items.
	T* items; // ��������� �� ��������.

	// ����������� ��������� ������� �� ������ from � ����� to.
	void copy( mysize _size, const T *from, T *to );
	// ������ ���-������ ������������ ������. ���� ��� �� �����.
	// void realloc( mysize& count, mysize otherCount, T* items, T* otherItems );
};

} // namespace my
