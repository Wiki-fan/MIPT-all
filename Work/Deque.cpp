#pragma once
#include "stdafx.h"
#include "Deque.h"

namespace my
{

const int DequeFragmentSize = 5; 

template<typename T>
CDeque<T>::CDeque()
	: f( DequeFragmentSize/2 ),
	l( DequeFragmentSize/2-1 ), 
	count( 0 )
{
	CNode *node = new CNode( 0, 0 );
	first = last = node;
}

template<typename T>
void CDeque<T>::push_back( const T& item )
{
	++count;
	if( l + 1 != DequeFragmentSize ) { // Не выходим за границу массива.
		last->buf[++l] = item;
	} else {
		CNode *node = new CNode( last, 0 );
		last->next = node;
		last = node;
		l = 0;
		last->buf[l] = item;
	}
}

template<typename T>
T CDeque<T>::pop_back()
{
	massert( count>0 ); // Нет элементов.
	--count;
	if( l != 0 ) {
		return last->buf[l--];
	} else {
		T ret = last->buf[l];
		CNode *n = last->prev;
		delete last;
		last = n;
		last->next = 0;
		l = DequeFragmentSize - 1;
		return ret;
	}
}

template<typename T>
void CDeque<T>::push_front( const T& item )
{
	++count;
	if( f != 0 ) {
		first->buf[--f] = item;
	} else {
		first->buf[f] = item;
		CNode *node = new CNode( 0, first );
		first->prev = node;
		first = node;
		f = DequeFragmentSize - 1;
	}
}

template<typename T>
T CDeque<T>::pop_front()
{
	massert( count > 0 ); // Нет элементов.
	--count;
	if( f != DequeFragmentSize-1 ) {
		return first->buf[f++];
	} else {
		T ret = first->buf[f];
		CNode *n = first->next;
		delete first;
		first = n;
		first->prev = 0;
		f = 0;
		return ret;
	}
}

template<typename T>
T& CDeque<T>::operator[](mysize n)
{
	massert( count > 0 ); // Нет элементов.
	if( n < DequeFragmentSize - f ) { // В первой ноде.
		return first->buf[n + f];
	} else {
		CNode* cur = first;
		mysize curpos = count - (DequeFragmentSize - f);
		while( curpos > DequeFragmentSize - 1 ) {
			curpos -= DequeFragmentSize;
			cur = cur->next;
		}
		return cur->buf[curpos];
	}
}

template<typename T>
const T& CDeque<T>::operator[]( mysize n ) const
{
	return this[n];
}

} // namespace my

/*int main4()
{
	my::CDeque<int> deque;
	deque.push_back( 0 );
	deque.push_back( 1 );
	deque.push_back( 2 );
	std::cout << deque.pop_back();
	deque.push_front( 3 );
	std::cout << deque.pop_front();
	std::cout << deque[2];
	return 0;
}*/