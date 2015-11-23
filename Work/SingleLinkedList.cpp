#pragma once
#include "stdafx.h"

//
//  main.cpp
//  SingleLinkedList
//
//  Created by Владислав Воротилов on 22/11/15.
//  Copyright © 2015 Vorotilov. All rights reserved.
//

// Класс односвязного списка
class CSingleLinkedList {
public:
	// Конcтруируем пустой список
	CSingleLinkedList();
	~CSingleLinkedList();

	// Элемент списка
	struct CNode {
		// Конструирует ни с чем не связный node
		CNode( int newVal ) : Value( newVal ), next( nullptr ) { }
		CNode( int newVal, CNode* next );

		// Хранимое значение
		int Value;

		// Возвращает следующий элемент
		// nullptr если такого нет
		CNode* Next();

		void SetNext( CNode* _next ) { next = _next; }

	private:
		// Указалетель на следующий элемент списка
		CNode* next;
	};

	// Разворот списка
	// Используется константная память и один проход
	void Reverse();

	// Возвращает первый элемент списка, nullptr если список пуст
	CNode* First();
	// Добавляет новый элемент в список
	void AddFirst( int newVal );

	CNode* first;

};

// Класс тестирующий список CSingleLinkedList
class CSingleLinkedListTester {
public:
	CSingleLinkedListTester() { }

	// Тестируем метод Reverse, true если удачно
	// Заполняет список случайными числами, запоминает их, делает reverse и проверяет что числа те же в обратом порядке
	bool TestReverse();

	// Тестируем добавление и просмотр некоторого количества элементов
	// true если удачно
	// Заполняет список случайными числами, запоминает их и проверяет что числа те же
	bool TestAddAndList();

private:
	const int N = 101;
};


int main2( int argc, const char* argv[] )
{
	CSingleLinkedListTester tester;

	// Тестируем all and list
	if( tester.TestAddAndList()) {
		std::cout << "add and list successful" << std::endl;
	} else {
		std::cout << "add and list failed" << std::endl;
	}

	// Тестируем reverse
	if( tester.TestReverse()) {
		std::cout << "reverse successful" << std::endl;
	} else {
		std::cout << "reverse failed" << std::endl;
	}
	return 0;
}

CSingleLinkedList::CNode::CNode( int newVal, CSingleLinkedList::CNode* _next )
		: Value( newVal ), next( _next )
{
}

CSingleLinkedList::CSingleLinkedList() : first( nullptr )
{
}

CSingleLinkedList::~CSingleLinkedList()
{
	while( CNode* temp = first->Next()) {
		delete first;
		first = temp;
	}
	delete first;
}

void CSingleLinkedList::Reverse()
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

CSingleLinkedList::CNode* CSingleLinkedList::First()
{
	return first;
}

void CSingleLinkedList::AddFirst( int newVal )
{
	CNode* temp = new CNode( newVal, first );
	first = temp;
}

CSingleLinkedList::CNode* CSingleLinkedList::CNode::Next()
{
	return next;
}

bool CSingleLinkedListTester::TestReverse()
{
	std::vector<int> testElem( N, rand());
	CSingleLinkedList lst;
	for( int i = 0; i < N; ++i ) {
		lst.AddFirst( testElem[i] );
	}
	lst.Reverse();
	CSingleLinkedList::CNode* node = lst.First();
	for( int i = N - 1; i >= 0; --i ) {
		if( node->Value != testElem[i] ) {
			return false;
		}
		node = node->Next();
	}
	return true;
}

bool CSingleLinkedListTester::TestAddAndList()
{
	std::vector<int> testElem( N, rand());
	CSingleLinkedList lst;
	for( int i = 0; i < N; ++i ) {
		lst.AddFirst( testElem[i] );
	}
	CSingleLinkedList::CNode* node = lst.First();
	for( int i = 0; i < N; ++i ) {
		if( node->Value != testElem[i] ) {
			return false;
		}
		node = node->Next();
	}
	return true;
}
