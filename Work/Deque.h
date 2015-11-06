#pragma once

#include "stdafx.h"

namespace my {

const int DequeFragmentSize = 10;

template<typename T>
class CDeque {

private:
	class CNode;

public:
	// Конструктор по умолчанию, создающий пустой дек.
	CDeque();
	// Затолкнуть элемент сзади.
	void push_back( const T& item );
	// Вытолкнуть элемент сзади.
	T pop_back();
	// Затолкнуть элемент спереди.
	void push_front( const T& item );
	// Вытолкнуть элемент спереди.
	T pop_front();
	// Пуст ли дек?
	bool empty() const { return count == 0; }
	// Количество элементов.
	mysize size() const { return count; }
	// Элемент на позиции n. Индексация, как обычно, с нуля.
	T& operator[]( mysize n );
	const T& operator[]( mysize n ) const;
	// Последний элемент.
	T& back() { return last->buf[l]; }
	const T& back() const { return last->buf[l]; }
	// Первый элемент.
	T& front() { return first->buf[f]; }
	const T& front() const { return first->buf[f]; }

	class CIterator : public std::iterator<std::random_access_iterator_tag, T> {

	public:
		CIterator(); // Конструктор по умолчанию.
		CIterator( CDeque& _parent, int _n ); // Конструктор по номеру элемента в деке.
		CIterator( const CIterator& other ); // Копирующий конструктор.
		~CIterator(); // Деструктор.

		CIterator operator++(); // Префиксный инкремент.
		CIterator operator++( int ); // Постфиксный инкремент.
		CIterator operator--(); // Префиксный декремент.
		CIterator operator--( int ); // Постфиксный декремент.

		CIterator operator+( int a );
		CIterator operator-( int a );

		CIterator& operator=( const CIterator& other ); // Оператор присваивания.
		bool operator==( const CIterator& other ) const; // Равенство.
		bool operator!=( const CIterator& other ) const; // Неравенство.
		bool operator>( const CIterator& other ) const; // Больше.
		bool operator>=( const CIterator& other ) const; // Больше или равно.
		bool operator<( const CIterator& other ) const; // Меньше.
		bool operator<=( const CIterator& other ) const; // Меньше или равно.

		T& operator*() const; // Разыменовывание.

	private:
		// Конструктор по указателю на конкретный подмассив дека и номеру элемента в нём. Должен вызываться только внутри класса.
		CIterator( CDeque& _parent, int _i, CNode* _buf );

		CDeque* parent; // Дек, итератором когорого является *this.
		CNode* node; // Указатель на буфер, в котором находится текущий элемент.
		int iBuf; // Номер элемента в буфере.
		friend class CDeque;

	};

	CIterator begin() { return CIterator( *this, 0 ); }
	CIterator begin() const { return CIterator( *this, 0 ); };
	//const_CIterator cbegin() const { return const_CIterator( *this, 0 ); };
	CIterator end() { return CIterator( *this, l + 1, last ); };
	CIterator end() const { return CIterator( *this, l + 1, last ); };
	//const_CIterator cend() cons { return const_CIterator( *this, last, l ); }; t;
	CIterator rbegin();
	CIterator rbegin() const;
	//const_CIterator crbegin() const;
	CIterator rend();
	CIterator rend() const;
	//const_CIterator crend() const;

private:
	// Нода, имеющая ссылки на предыдущую и следующую ноды, а также массив с элементами.
	struct CNode {

	public:
		CNode( CNode* _prev, CNode* _next ) : prev( _prev ), next( _next )
		{
			buf = new T[DequeFragmentSize];
		}

		~CNode()
		{
			delete[] buf;
		}

		CNode* prev; // Предыдущая.
		CNode* next; // Следующая.
		T* buf; // Элементы.

	private:
		// Копирующий конструктор не нужен и не должен вызываться, посему описан в private,
		CNode( const CNode& other );
	};

	CNode* first; // Первая нода.
	CNode* last; // Последняя нода.
	mysize f; //Индекс первого элемента в первой ноде.
	mysize l; // Индекс последнего элемента в последней ноде.
	mysize count; // Количество элементов.
};

template<typename T>
CDeque<T>::CDeque()
	: f( DequeFragmentSize / 2 ),
	l( DequeFragmentSize / 2 - 1 ),
	count( 0 )
{
	CNode* node = new CNode( 0, 0 );
	first = last = node;
}

template<typename T>
void CDeque<T>::push_back( const T& item )
{
	++count;
	if( l + 1 != DequeFragmentSize ) { // Не выходим за границу массива.
		last->buf[++l] = item;
	} else {
		CNode* node = new CNode( last, 0 );
		last->next = node;
		last = node;
		l = 0;
		last->buf[l] = item;
	}
}

template<typename T>
T CDeque<T>::pop_back()
{
	massert( count > 0 ); // Нет элементов.
	--count;
	if( l != 0 ) {
		return last->buf[l--];
	} else {
		T ret = last->buf[l];
		CNode* node = last->prev;
		delete last;
		last = node;
		last->next = 0;
		l = DequeFragmentSize - 1;
		return ret;
	}
}

template<typename T>
void CDeque<T>::push_front( const T& item )
{
	++count;
	if( f -1 != -1 ) {
		first->buf[--f] = item;
	} else {
		CNode* node = new CNode( 0, first );
		first->prev = node;
		first = node;
		f = DequeFragmentSize - 1;
		first->buf[f] = item;
	}
}

template<typename T>
T CDeque<T>::pop_front()
{
	massert( count > 0 ); // Нет элементов.
	--count;
	if( f != DequeFragmentSize - 1 ) {
		return first->buf[f++];
	} else {
		T ret = first->buf[f];
		CNode* node = first->next;
		delete first;
		first = node;
		first->prev = 0;
		f = 0;
		return ret;
	}
}

template<typename T>
T& CDeque<T>::operator[]( mysize n )
{
	massert( count > 0 ); // Нет элементов.
	if( n < DequeFragmentSize - f ) { // В первой ноде.
		return first->buf[n + f];
	} else {
		CNode* cur = first->next;
		n -= DequeFragmentSize - f;
		while( n > DequeFragmentSize - 1 ) {
			n -= DequeFragmentSize;
			cur = cur->next;
		}
		return cur->buf[n];
	}
}

template<typename T>
const T& CDeque<T>::operator[]( mysize n ) const
{
	return this[n];
}

template<typename T>
CDeque<T>::CIterator::CIterator()
	:parent( 0 ), node( 0 ), iBuf( 0 )
{

}

template<typename T>
CDeque<T>::CIterator::CIterator( CDeque& _parent, int _iBuf, CNode* _buf )
	: parent( &_parent ), iBuf( _iBuf ), node( _buf )
{
}

template<typename T>
CDeque<T>::CIterator::CIterator( CDeque<T>& _parent, int _n ) :
	parent( &_parent )
{
	node = parent->first;
	if( _n < DequeFragmentSize - parent->f ) { // В первой ноде.
		node = parent->first;
		iBuf = _n + parent->f;
	} else {
		CNode* cur = parent->first;
		mysize curpos = parent->count - (DequeFragmentSize - parent->f);
		while( curpos > DequeFragmentSize - 1 ) {
			curpos -= DequeFragmentSize;
			cur = cur->next;
		}
		node = cur;
		iBuf = curpos;
	}
}

template<typename T>
CDeque<T>::CIterator::CIterator( const CIterator& other )
	: node( other.node ), iBuf( other.iBuf ), parent( other.parent )
{
}

template<typename T>
CDeque<T>::CIterator::~CIterator()
{
	// TODO
}

template<typename T>
typename CDeque<T>::CIterator& CDeque<T>::CIterator::operator=( const CIterator& other )
{
	if( this != &other ) {
		node = other.node;
		iBuf = other.iBuf;
		parent = other.parent;
	}
	return *this;
}

template<typename T>
bool CDeque<T>::CIterator::operator==( const CIterator& other ) const
{
	return (node->buf == other.node->buf && iBuf == other.iBuf);
}

template<typename T>
bool CDeque<T>::CIterator::operator!=( const CIterator& other ) const
{
	return !(*this == other);
}

template<typename T>
typename CDeque<T>::CIterator CDeque<T>::CIterator::operator++()
{
	if( iBuf + 1 >= DequeFragmentSize ) {
		node = node->next;
		//massert( node != 0, "Deque operator++ out of range" );
		iBuf = 0;
	} else {
		++iBuf;
	}
	return *this;
}

template<typename T>
typename CDeque<T>::CIterator CDeque<T>::CIterator::operator++( int )
{
	CIterator ret = *this;
	this->operator++();
	return ret;
}

template<typename T>
typename CDeque<T>::CIterator CDeque<T>::CIterator::operator--()
{
	if( iBuf - 1 < 0 ) {
		node = node->prev;
		//massert( node != 0, "Deque operator-- out of range" );
		iBuf = DequeFragmentSize - 1;
	} else {
		--iBuf;
	}
	return *this;
}

template<typename T>
typename CDeque<T>::CIterator CDeque<T>::CIterator::operator--( int )
{
	CIterator ret = *this;
	this->operator--();
	return ret;
}

template<typename T>
typename CDeque<T>::CIterator CDeque<T>::CIterator::operator+( int a )
{
	CNode* temp = node;
	while( iBuf + a >= DequeFragmentSize ) {
		temp = temp->next;
		a -= DequeFragmentSize;
	}
	return CIterator( parent, temp, a );
}

template<typename T>
typename CDeque<T>::CIterator CDeque<T>::CIterator::operator-( int a )
{
	CNode* temp = node;
	while( iBuf - a < 0 ) {
		temp = temp->prev;
		a -= DequeFragmentSize;
	}
	return CIterator( parent, temp, a );
}

template<typename T>
bool CDeque<T>::CIterator::operator>( const CIterator& other ) const
{
	massert( parent == other.parent );
	// Если они указывают на элементы в одном подмассиве, просто сравниваем индексы.
	if( other.node == node ) {
		if( iBuf > other.iBuf ) {
			return true;
		} else {
			return false;
		}
	}
	// Проматываем от подмассива other'а до конца. Если найдём подмассив *this'а, то он действительно больше.
	CNode* temp = other.node;
	while( temp != 0 ) {
		if( temp == node ) {
			return true;
		}
		temp = temp->next;
	}
	return false;
}

template<typename T>
bool CDeque<T>::CIterator::operator>=( const CIterator& other ) const
{
	return ((*this) > other || (*this) == other);
}

template<typename T>
bool CDeque<T>::CIterator::operator<( const CIterator& other ) const
{
	return !((*this) >= other);
}

template<typename T>
bool CDeque<T>::CIterator::operator<=( const CIterator& other ) const
{
	return !((*this) > other);
}

template<typename T>
T& CDeque<T>::CIterator::operator*() const
{
	return node->buf[iBuf];
}

} // namespace my
