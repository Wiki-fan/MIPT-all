#pragma once
#include "stdafx.h"

// Класс односвязного списка
template<typename T>
class CSingleLinkedList {
public:
	// Конcтруируем пустой список
	CSingleLinkedList() : first( nullptr ) {}
	explicit CSingleLinkedList( const T& key ) : first( new CNode( key )) { }
	~CSingleLinkedList();

	// Элемент списка
	struct CNode {
		// Конструирует ни с чем не связный node
		explicit CNode( const T& newVal ) : Value( newVal ), next( nullptr ) { }
		CNode( const T& newVal, CNode* _next ) : Value( newVal ), next( _next ) { }

		// Хранимое значение
		T Value;

		// Возвращает следующий элемент
		// nullptr если такого нет
		CNode* Next();

		void SetNext( CNode* _next ) { next = _next; }

	private:
		// Указатель на следующий элемент списка
		CNode* next;
	};

	// Проверка на пустоту.
	bool isEmpty() { return first == nullptr; }
	// Возвращает первый элемент списка, nullptr если список пуст
	CNode* First() const;
	// Добавляет новый элемент в список
	void AddFirst( T& newVal );
	void AddFirst( T* newVal );

	// Удаляет элемент из списка, но не вызывает его деструктор.
	CNode* DeleteAfter( CNode*& node )
	{
		if( node != 0 ) {
			CNode* tmp = node->Next();
			if( tmp != 0 ) {
				node->SetNext( node->Next()->Next());
			}
			return tmp;
		}
		return 0;
	}

	//template<class Compare>
	CSingleLinkedList* Merge( CSingleLinkedList& other );
	// Разворот списка
	// Используется константная память и один проход
	void Reverse();

private:
	CNode* first;

};

template<typename T>
CSingleLinkedList<T>::~CSingleLinkedList()
{
	if (first != 0) {
		while( CNode* temp = first->Next()) {
			delete first;
			first = temp;
		}
		delete first;
	}
}

template<typename T>
void CSingleLinkedList<T>::Reverse()
{
	CNode* temp;
	CNode* ret = nullptr;
	while( first ) {
		temp = first->Next();
		first->SetNext( ret );
		ret = first;
		first = temp;
	}
	first = ret;
}

template<typename T>
typename CSingleLinkedList<T>::CNode* CSingleLinkedList<T>::First() const
{
	return first;
}

template<typename T>
void CSingleLinkedList<T>::AddFirst( T& newVal )
{
	CNode* temp = new CNode( newVal, first );
	first = temp;
}
template<typename T>
void CSingleLinkedList<T>::AddFirst( T* newVal )
{
	CNode* temp = new CNode( *newVal, first );
	first = temp;
}

template<typename T>
typename CSingleLinkedList<T>::CNode* CSingleLinkedList<T>::CNode::Next()
{
	return next;
}

template<typename T>
//template<class Compare>
CSingleLinkedList<T>* CSingleLinkedList<T>::Merge( CSingleLinkedList& other )
{
	CSingleLinkedList<T> ret;
	CNode* iter1 = first;
	CNode* iter2 = other.first;
	while( iter1 != 0 && iter2 != 0 ) {
		if(/*Compare()(iter1, iter2)*/iter1 < iter2 ) {
			ret.AddFirst( iter1->Value );
			iter1 = iter1->Next();
		} else {
			ret.AddFirst( iter2->Value );
			iter2 = iter2->Next();
		}
	}
	while( iter1 != 0 ) {
		ret.AddFirst( iter1->Value );
		iter1 = iter1->Next();
	}
	while( iter2 != 0 ) {
		ret.AddFirst( iter2->Value );
		iter2 = iter2->Next();
	}
	other.first = 0;
	first = ret.first;
	ret.first = 0;
	return this;
}
