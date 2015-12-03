#pragma once
#include "stdafx.h"
#include "MeldableHeap.h"
#include "LeftistHeap.h"


/*template<typename T, class Compare>
class CSkewHeap : public CLeftistHeap<T, Compare> {

public:
	// Конструктор по умолчанию, создаёт пустую кучу.
	CSkewHeap() : head( nullptr ) { }
	// Конструктор, создающий кучу с единственным элементом с ключом key.
	CSkewHeap( const T& key ) : head( new CNode( key )) { }
	~CSkewHeap() { delete head; }
	void Add( const T& key ) { CLeftistHeap::Add(key); }
	T ExtractTop(){return CLeftistHeap::ExtractTop(); }
	bool isEmpty(){return CLeftistHeap::isEmpty(); }
	IMeldableHeap<T, Compare>* Meld( CSkewHeap& other );

private:
	struct CNode {
		// Конструктор по умолчанию.
		CNode() { }
		// Вершина, являющаяся вершиной дерева из одного элемента с ключом _key.
		CNode( T& _key ) : key( _key ), parent( 0 ), left( 0 ), right( 0 ), dist( 0 ) { }
		// Рекурсивный деструктор. Удаляет детей.
		~CNode()
		{
			delete left;
			delete right;
		}
		// Родитель.
		CNode* parent;
		// Левый и правый дети.
		CNode* left, * right;
		// Расстояние до ближайшей свободной позиции (для листьев - 0).
		int dist;
		// Ключ вершины.
		T key;
	};

	CNode* subMeld( CNode* x, CNode* y );

	CNode* head; // Указатель на корневой элемент.

};

template<typename T, class Compare>
CSkewHeap::CNode* CSkewHeap::subMeld( CSkewHeap::CNode* x, CSkewHeap::CNode* y )
{
	return nullptr;
}

template<typename T, class Compare>
IMeldableHeap<T, Compare>* CSkewHeap::Meld( CSkewHeap& other )
{
	return nullptr;
}
*/