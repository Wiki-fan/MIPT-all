#pragma once
#include "stdafx.h"
#include "MeldableHeap.h"

template<typename T, class Compare>
class CLeftistHeap : public IMeldableHeap<T, Compare> {

public:
	// Конструктор по умолчанию, создаёт пустую кучу.
	CLeftistHeap() : head( nullptr ) { }
	// Конструктор, создающий кучу с единственным элементом с ключом key.
	CLeftistHeap( const T& key ) : head( new CNode( key )) { }
	~CLeftistHeap() { delete head; }
	virtual void Add( const T& key ) override;
	virtual T ExtractTop() override;
	virtual bool isEmpty() override { return head == nullptr; }
	IMeldableHeap<T, Compare>* Meld( CLeftistHeap& other );

private:
	struct CNode {
		// Конструктор по умолчанию.
		CNode() { }
		// Вершина, являющаяся вершиной дерева из одного элемента с ключом _key.
		CNode( const T& _key ) : key( _key ), parent( 0 ), left( 0 ), right( 0 ), dist( 0 ) { }
		// Рекурсивный деструктор. Удаляет детей.
		~CNode()
		{
			std::cout << "Oppa, delete leftist " << key << std::endl;
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

	CNode* head; // Указатель на корневой элемент.

	CNode* subMeld( CNode* x, CNode* y );

};

template<typename T, class Compare>
void CLeftistHeap<T, Compare>::Add( const T& key )
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
T CLeftistHeap<T, Compare>::ExtractTop()
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
IMeldableHeap<T, Compare>* CLeftistHeap<T, Compare>::Meld( CLeftistHeap& other )
{
	head = subMeld( head, other.head );
	return this;
}

// Подпрограмма слияния поддеревьев узла.
template<typename T, class Compare>
typename CLeftistHeap<T, Compare>::CNode* CLeftistHeap<T, Compare>::subMeld( CLeftistHeap::CNode* x, CLeftistHeap::CNode* y )
{
	if( x == 0 ) {
		return y;
	}
	if( y == 0 ) {
		return x;
	}
	if( Compare()( y->key, x->key )) {
		std::swap( x, y );
	}
	// Сливаем правое поддерево x с y, ибо правая ветка самая короткая в силу левостороннести кучи.
	x->right = subMeld( x->right, y );
	// Восстанавливаем (если нарушилась) левостороннесть кучи - меняем детей местами.
	if( x->right != 0 && x->left != 0 ) {
		if( x->right->dist > x->left->dist ) {
			std::swap( x->left, x->right );
		}
		// Пересчитываем расстояние.
		x->dist = std::min( x->left->dist, x->right->dist ) + 1;
	}
	return x;
}