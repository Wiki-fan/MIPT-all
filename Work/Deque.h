#pragma once
#include "Vector.h"

namespace my
{

template<typename T>
class CDeque
{
public:
	// ����������� �� ���������, ��������� ������ ���.
	CDeque();
	// ���������� ������� �����.
	void push_back( const T& item );
	// ���������� ������� �����.
	T pop_back();
	// ���������� ������� �������.
	void push_front( const T& item );
	// ���������� ������� �������.
	T pop_front();
	// ���� �� ���?
	bool empty() const { return count == 0; };
	// ���������� ���������.
	mysize size() const { return count; };
	// ������� �� ������� n.
	T& operator[]( mysize n ); // ����������, ��� ������, � ����.
	const T& operator[]( mysize n ) const;
	// ��������� �������.
	T& back() { return last->buf[l]; };
	const T& back() const { return last->buf[l]; };
	// ������ �������.
	T& front() { return first->buf[f]; }
	const T& front() const { return first->buf[f]; };

private:
	// ����, ������� ������ �� ���������� � ��������� ����, � ����� ������ � ����������.
	struct CNode
	{
		CNode* prev; // ����������.
		CNode* next; // ���������.
		T* buf; // ��������.
		CNode( CNode *_prev, CNode *_next ) : prev( _prev ), next( _next )
		{
			buf = new T[DequeFragmentSize];
		}
		~CNode()
		{
			delete[] buf;
		}
	};
	CNode *first; // ������ ����.
	CNode *last; // ��������� ����.
	mysize f; //������ ������� �������� � ������ ����.
	mysize l; // ������ ���������� �������� � ��������� ����.
	mysize count; // ���������� ���������.
};

} // namespace my