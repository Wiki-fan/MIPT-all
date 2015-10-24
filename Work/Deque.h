#pragma once
#include "Vector.h"

namespace my
{

template<typename T>
class CDeque
{
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
	struct CNode
	{
		CNode* prev; // Предыдущая.
		CNode* next; // Следующая.
		T* buf; // Элементы.
		CNode( CNode *_prev, CNode *_next ) : prev( _prev ), next( _next )
		{
			buf = new T[DequeFragmentSize];
		}
		~CNode()
		{
			delete[] buf;
		}
	};
	CNode *first; // Первая нода.
	CNode *last; // Последняя нода.
	mysize f; //Индекс первого элемента в первой ноде.
	mysize l; // Индекс последнего элемента в последней ноде.
	mysize count; // Количество элементов.
};

} // namespace my