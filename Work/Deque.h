#pragma once
#include "stdafx.h"

namespace my {

const int DequeFragmentSize = 10;

template<typename T>
class CDeque {

private:
	struct CNode; // Предварительное объявление.

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

	template<bool is_const_iterator>
	class CMetaIterator : public std::iterator<std::random_access_iterator_tag, T> {

	public:
		typedef CMetaIterator self_type;
		typedef std::ptrdiff_t difference_type;
		typedef T value_type;
		typedef typename std::conditional<is_const_iterator, const value_type&, value_type&>::type reference;
		typedef typename std::conditional<is_const_iterator, const value_type*, value_type*>::type pointer;
		// Конструктор по умолчанию.
		CMetaIterator() :parent( 0 ), node( 0 ), iBuf( 0 )
		{
		}
		// Конструктор по номеру элемента в деке.
		CMetaIterator( CDeque& _parent, difference_type _n );
		// Копирующий конструктор.
		CMetaIterator( const self_type& other )
			: node( other.node ), iBuf( other.iBuf ), parent( other.parent )
		{
		}
		// Деструктор.
		~CMetaIterator() {};

		// Префиксный инкремент.
		self_type operator++();
		// Постфиксный инкремент.
		self_type operator++( int )
		{
			CIterator ret = *this;
			this->operator++();
			return ret;
		}
		// Префиксный декремент.
		self_type operator--();
		// Постфиксный декремент.
		self_type operator--( int )
		{
			CIterator ret = *this;
			this->operator--();
			return ret;
		}

		self_type operator+( difference_type a ) const; // Плюс.
		self_type operator-( difference_type a ) const; // Минус.
		self_type operator+=( difference_type a )
		{
			*this = (*this) + a;
			return *this;
		}
		self_type operator-=( difference_type a )
		{
			*this = (*this) - a;
			return *this;
		}

		difference_type operator-( const self_type& other );

		// Оператор присваивания.
		self_type& operator=( const self_type& other )
		{
			if( this != &other ) {
				node = other.node;
				iBuf = other.iBuf;
				parent = other.parent;
			}
			return *this;
		}
		// Сравнение.
		bool operator==( const self_type& other ) const
		{
			return (node->buf == other.node->buf && iBuf == other.iBuf);
		}// Неравенство.
		bool operator!=( const self_type& other ) const
		{
			return !(*this == other);
		}
		bool operator>( const self_type& other ) const;
		bool operator>=( const self_type& other ) const { return ((*this) > other || (*this) == other); };
		bool operator<( const self_type& other ) const { return !((*this) >= other); };
		bool operator<=( const self_type& other ) const { return !((*this) > other); };
		// Разыменовывание.
		reference operator*() const
		{
			return node->buf[iBuf];
		}
		// Оператор стрелочка.
		pointer operator->() const { return &(node->buf[iBuf]); };

		// Прибавка к константе.
		friend self_type operator+( difference_type a, const self_type &iter )
		{
			return iter.operator+( a );
		}
		friend self_type operator-( difference_type a, const self_type &iter )
		{
			return iter.operator-( a );
		}

	private:
		CMetaIterator( CDeque& _parent, CNode* _buf, difference_type _iBuf )
			: parent( &_parent ), node( _buf ), iBuf( _iBuf )
		{
		}

		CDeque* parent; // Дек, итератором когорого является *this.
		CNode* node; // Указатель на буфер, в котором находится текущий элемент.
		difference_type iBuf; // Номер элемента в буфере.
		friend class CDeque;

	};

	typedef CMetaIterator<false> CIterator;
	typedef CMetaIterator<true> CConstIterator;
	typedef std::reverse_iterator<CIterator> CReverseIterator;
	typedef std::reverse_iterator<CConstIterator> CReverseConstIterator;

	CIterator begin() { return CIterator( *this, first, f ); }
	CIterator begin() const { return CIterator( *this, first, f ); };
	CConstIterator cbegin() { return CConstIterator( *this, first, f ); };
	CConstIterator cbegin() const { return CConstIterator( *this, first, f ); };
	CIterator end() { return CIterator( *this, last, l + 1 ); };
	CIterator end() const { return CIterator( *this, last, l + 1 ); };
	CConstIterator cend() { return CConstIterator( *this, last, l + 1 ); };
	CConstIterator cend() const { return CConstIterator( *this, last, l + 1 ); };
	CReverseIterator rbegin() { return std::reverse_iterator<CIterator>( end() ); };
	CReverseIterator rbegin() const { return std::reverse_iterator<CIterator>( end() ); };
	CReverseConstIterator crbegin() { return std::reverse_iterator<CConstIterator>( cend()); };
	CReverseConstIterator crbegin() const { return std::reverse_iterator<CConstIterator>( cend()); };
	CReverseIterator rend() { return std::reverse_iterator<CIterator>( begin() ); };
	CReverseIterator rend() const { return std::reverse_iterator<CIterator>( begin() ); };
	CReverseConstIterator crend() { return std::reverse_iterator<CConstIterator>( cbegin()); };
	CReverseConstIterator crend() const { return std::reverse_iterator<CConstIterator>( cbegin()); };

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
	if( f - 1 != -1 ) {
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
template <bool is_const_iterator>
CDeque<T>::CMetaIterator<is_const_iterator>::CMetaIterator( CDeque<T>& _parent, difference_type _n ) :
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
template <bool is_const_iterator>
typename CDeque<T>::template CMetaIterator<is_const_iterator>
CDeque<T>::CMetaIterator<is_const_iterator>::operator++()
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
template <bool is_const_iterator>
typename CDeque<T>::template CMetaIterator<is_const_iterator>
CDeque<T>::CMetaIterator<is_const_iterator>::operator--()
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
template <bool is_const_iterator>
typename CDeque<T>::template CMetaIterator<is_const_iterator>
CDeque<T>::CMetaIterator<is_const_iterator>::operator+( difference_type a ) const
{
	CNode* temp = node;
	while( iBuf + a >= DequeFragmentSize ) {
		temp = temp->next;
		a -= DequeFragmentSize;
	}
	return CMetaIterator( *parent, temp, iBuf + a );
}

template<typename T>
template <bool is_const_iterator>
typename CDeque<T>::template CMetaIterator<is_const_iterator>
CDeque<T>::CMetaIterator<is_const_iterator>::operator-( difference_type a ) const
{
	CNode* temp = node;
	while( iBuf - a < 0 ) {
		temp = temp->prev;
		a -= DequeFragmentSize;
	}
	return CMetaIterator( *parent, temp, iBuf - a );
}

template <typename T>
template <bool is_const_iterator>
typename CDeque<T>::template CMetaIterator<is_const_iterator>::difference_type
CDeque<T>::CMetaIterator<is_const_iterator>::operator-( const self_type& other )
{
	massert( parent == other.parent );
	// Если они указывают на элементы в одном подмассиве, просто сравниваем индексы.
	if( other.node == node ) {
		return iBuf - other.iBuf;
	}
	difference_type diff;
	CNode* temp = other.node;
	// Проматываем от подмассива other'а до конца. Если найдём подмассив *this'а, то вернём разницу.
	bool fl = 0;
	diff = iBuf;
	while( temp != 0 ) {
		if( temp == node ) {
			fl = true;
			break;
		}
		temp = temp->next;
		diff += DequeFragmentSize;
	}
	diff -= other.iBuf;
	// Не нашли, надо было идти в другую сторону.
	if( fl == false ) {
		diff = DequeFragmentSize-iBuf;
		temp = other.node;
		while( temp != 0 ) {
			if( temp == node ) {
				break;
			}
			temp = temp->prev;
			diff -= DequeFragmentSize;
		}
		diff -= other.iBuf;
	}
	return diff;
}

template<typename T>
template <bool is_const_iterator>
bool CDeque<T>::CMetaIterator<is_const_iterator>::operator>( const self_type& other ) const
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

} // namespace my
