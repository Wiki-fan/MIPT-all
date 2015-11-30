#pragma once
#include "stdafx.h"

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