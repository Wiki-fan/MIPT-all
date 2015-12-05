#pragma once
#include "stdafx.h"
#include "MeldableHeap.h"

template<typename T, class Compare>
class CSkewHeap : public IMeldableHeap<T, Compare> {

public:
	// Конструктор по умолчанию, создаёт пустую кучу.
	CSkewHeap() : head( nullptr ) { }
	// Конструктор, создающий кучу с единственным элементом с ключом key.
	CSkewHeap( const T& key ) : head( new CNode( key )) { }
	~CSkewHeap() { delete head; }
	void Add( const T& key );
	T ExtractTop();
	bool isEmpty() { return head == nullptr; }
	IMeldableHeap<T, Compare>* Meld( CSkewHeap& other );

private:
	struct CNode {
		// Конструктор по умолчанию.
		CNode() { }
		// Вершина, являющаяся вершиной дерева из одного элемента с ключом _key.
		CNode( const T& _key ) : key( _key ), parent( 0 ), left( 0 ), right( 0 ) { }
		// Рекурсивный деструктор. Удаляет детей.
		~CNode()
		{
			std::cout << "Delete skew " << key << std::endl;
			delete left;
			delete right;
		}
		// Родитель.
		CNode* parent;
		// Левый и правый дети.
		CNode* left, * right;
		// Ключ вершины.
		T key;
	};

	CNode* subMeld( CNode* x, CNode* y );
	CNode* head; // Указатель на корневой элемент.

};

template<typename T, class Compare>
void CSkewHeap<T, Compare>::Add( const T& key )
{
	// Создаём дерево из одной вершины.
	CNode* tmp = new CNode( key );
	// Сливаем его с данным.
	CNode* node = subMeld( head, tmp );
	head = node;
	//delete tmp;
}

// Извлекает вершину кучи и возвращает её ключ.
template<typename T, class Compare>
T CSkewHeap<T, Compare>::ExtractTop()
{
	T top = head->key;
	// Сливаем детей и пишем результат на место корня.
	CNode* tmp = subMeld( head->left, head->right );
	head->left = head->right = 0;
	delete head;
	head = tmp;
	return top;
}

// Сливает кучу с данной. Другая куча становится пустой.
// Возвращает указатель на данную кучу.
template<typename T, class Compare>
IMeldableHeap<T, Compare>* CSkewHeap<T, Compare>::Meld( CSkewHeap& other )
{
	head = subMeld( head, other.head );
	return this;
}

// Подпрограмма слияния поддеревьев узла.
template<typename T, class Compare>
typename CSkewHeap<T, Compare>::CNode* CSkewHeap<T, Compare>::subMeld( CSkewHeap::CNode* x, CSkewHeap::CNode* y )
{
	if (x == y) {
		return x;
	}
	if( x == 0 ) {
		return y;
	}
	if( y == 0 ) {
		return x;
	}
	/*if( Compare()( x->key, y->key )) {
		CNode* tmp = x->right;
		x->right = x->left;
		x->left = subMeld( y, tmp );
		return x;
	} else {
		return subMeld( y, x );
	}*/
	if( Compare()( y->key, x->key )) {
		std::swap( x, y );
	}
	CNode* tmp = x->right;
	x->right = x->left;
	x->left = subMeld( y, tmp );
	return x;
}