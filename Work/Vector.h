#pragma once
#include <cassert>

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
	CVector( mysize _size, T* items ); 
	// ���������� �����������.
	CVector( const CVector& other ); 
	// ����������. 
	~CVector();

	T operator[]( const mysize n ) const;
	T& operator[]( const mysize n );

	CVector& operator=( const CVector& other );

	// ���������� �������� � ����� �������.
	void PushBack( const T& item );
	// �������� �������� �� ����� �������.
	T PopBack();
	// �������� ������� �������.
	bool isEmpty() const { return (size == 0); }
	// ��������� �� ������.
	T* Begin() const { return items; }
	// ��������� �� �������, ��������� �� ��������� ��������� �������.
	T* End() const { return &(items[size]); }
	// ��������� �������.
	T& Last() 
	{
		assert( !isEmpty() );
		return items[size - 1];
	}
	// ������ �������.
	T& First() 
	{
		assert( !isEmpty() );
		return items[0];
	}

	// ���������� ������ �������.
	const mysize Size() const { return size; }
	// ���������� ���������� ������, ��������� � ������ ������ ��� ��������.
	const mysize MaxSize() const { return maxSize; }

private:
	mysize size;	// ������ (���������� ���������� ���������).
	mysize maxSize;	// ������������ ������, a.k.a. ������ ������ items.
	T* items;	// ��������� �� ��������.

	// ����������� ��������� ������� �� ������ from � ����� to.
	void copy( mysize _size, const T *from, T *to );
};

} // namespace my
