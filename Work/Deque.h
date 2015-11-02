#pragma once

namespace my
{

const int DequeFragmentSize = 5;

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

	class iterator : public std::iterator<std::random_access_iterator_tag, T> {

	public:
		iterator( CDeque& _parent, int _n ); // Конструктор по номеру элемента в деке.
		iterator( const iterator& other); // Копирующий конструктор.
		~iterator(); // Деструктор.

		iterator operator++(); // Префиксный инкремент.
		iterator operator++( int ); // Постфиксный инкремент.

		iterator& operator=( const iterator& other ); // Оператор присваивания.
		bool operator==( const iterator& other) const; // Равенство.
		bool operator!=( const iterator& other) const; // Неравенство.

		reference operator*() const; // Разыменовывание.

	private:
		// Конструктор по указателю на конкретный подмассив дека и номеру элемента в нём. Должен вызываться только внутри класса.
		iterator( CDeque& _parent, int _i, CNode *_buf ); 

		CDeque& parent; // Дек, итератором когорого является *this.
		CNode* buf; // Указатель на буфер, в котором находится текущий элемент.
		int iBuf; // Номер элемента в буфере.
		int n; // Номер элемента во всём деке.
		friend class CDeque;

	};

	iterator begin() { return iterator( *this, 0 ); }
	iterator begin() const { return iterator( *this, 0 ); };
	//const_iterator cbegin() const { return const_iterator( *this, 0 ); };
	iterator end() { return iterator( *this, last, l ); };
	iterator end() const { return iterator( *this, last, l ); };;
	//const_iterator cend() cons { return const_iterator( *this, last, l ); }; t;
	iterator rbegin();
	iterator rbegin() const;
	//const_iterator crbegin() const;
	iterator rend(); 
	iterator rend() const;
	//const_iterator crend() const;

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

template<typename T>
CDeque<T>::iterator::iterator( CDeque& _parent, int _iBuf, CNode *_buf )
	:parent(_parent), iBuf(_iBuf), buf(_buf)
{
	int temp = 0;
	CNode* cur = parent.first;
	while( cur != buf ) {
		++temp;
		cur = cur->next;
	}
	n = temp*DequeFragmentSize + iBuf;
}

template<typename T>
CDeque<T>::iterator::iterator( CDeque<T>& _parent, int _n ) :
	parent(_parent), n(_n)
{
	buf = parent.first;
	n = _n;
	if( n < DequeFragmentSize - parent.f ) { // В первой ноде.
		buf = parent.first;
		iBuf = n + parent.f;
	} else {
		CNode* cur = parent.first;
		mysize curpos = parent.count - (DequeFragmentSize - parent.f);
		while( curpos > DequeFragmentSize - 1 ) {
			curpos -= DequeFragmentSize;
			cur = cur->next;
		}
		buf = cur;
		iBuf = curpos;
	}
}

template<typename T>
CDeque<T>::iterator::iterator( const iterator& other )
	:n(other.n), buf(other.buf), iBuf(other.iBuf), parent(other.parent)
{
}

template<typename T>
CDeque<T>::iterator::~iterator()
{
	// TODO
}

template<typename T>
CDeque<T>::iterator& CDeque<T>::iterator::operator=( const iterator& other)
{
	if( this != &other ) {
		n = other.n;
		buf = other.buf;
		iBuf = other.iBuf;
		parent = other.parent;
	}
	return *this;
}

template<typename T>
bool CDeque<T>::iterator::operator==( const iterator& other ) const
{
	return (n == other.n);
}

template<typename T>
bool CDeque<T>::iterator::operator!=( const iterator& other ) const
{
	return !(*operator==(*this, other));
}

template<typename T>
CDeque<T>::iterator CDeque<T>::iterator::operator++()
{
	++n;
	if( iElem + 1 >= DequeFragmentSize ) {
		buf = buf->next;
		iElem = 0;
	} else {
		++iElem;
	}
	return *this;
}

template<typename T>
CDeque<T>::iterator CDeque<T>::iterator::operator++(int)
{
	iterator ret = *this;
	this->operator++();
	return ret;
}

template<typename T>
reference CDeque<T>::iterator::operator*() const
{
	return buf[iBuf];
}

} // namespace my
