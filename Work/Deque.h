#pragma once

namespace my
{

const int DequeFragmentSize = 5;

template<typename T>
class CDeque {
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
	bool empty() const { return count == 0; };
	// Количество элементов.
	mysize size() const { return count; };
	// Элемент на позиции n.
	T& operator[]( mysize n ); // Индексация, как обычно, с нуля.
	const T& operator[]( mysize n ) const;
	// Последний элемент.
	T& back() { return last->buf[l]; };
	const T& back() const { return last->buf[l]; };
	// Первый элемент.
	T& front() { return first->buf[f]; }
	const T& front() const { return first->buf[f]; };

private:
	// Нода, имеющая ссылки на предыдущую и следующую ноды, а также массив с элементами.
	struct CNode {

	public:
		CNode( CNode *_prev, CNode *_next ) : prev( _prev ), next( _next )
		{
			buf = new T[DequeFragmentSize];
		}
		~CNode() { delete[] buf; }

		CNode* prev; // Предыдущая.
		CNode* next; // Следующая.
		T* buf; // Элементы.
		
	private:
		// Копирующий конструктор не нужен и не должен вызываться, посему описан в private,
		CNode( const CNode& other );
	};
	CNode *first; // Первая нода.
	CNode *last; // Последняя нода.
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
	massert( count > 0 ); // Нет элементов.
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
	if( f != DequeFragmentSize - 1 ) {
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
T& CDeque<T>::operator[]( mysize n )
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