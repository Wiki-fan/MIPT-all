#pragma once
#include "stdafx.h"
#include "ITree.h"
// Декартово дерево.
template<typename T>
class CCartesianTree {

public:
	CCartesianTree() : head( 0 ), curHeight( 0 ), maxHeight( 0 ) { }
	~CCartesianTree() { delete head; }

	// Вставка ключа x с приоритутом y.
	void Insert( const T& x, const T& y );
	// Высота дерева.
	int calcHeight() const;

private:
	struct CNode {
		CNode() { }
		CNode( T x, T y ) : key( x ), prior( y ) { }
		~CNode()
		{
			delete left;
			delete right;
		}
		T key, prior;
		CNode* left;
		CNode* right;

	};

	CNode* head;
	mutable int curHeight;
	mutable int maxHeight;

	void subSplit( CNode* t, T key, CNode*& l, CNode*& r );
	void subInsert( CNode*& t, CNode* it );


	void inorderTraversal( CNode* node ) const;
};

template<typename T>
void CCartesianTree<T>::Insert( const T& x, const T& y )
{
	/*if( head == 0 ) {
		head = new CNode( x, y );
		return;
	}
	// Опускаемся вниз, пока y не станет меньше tmp.y
	CNode* tmp = head;
	while( tmp->y >= y && tmp != 0 ) {
		if( x > tmp->x && tmp->right != 0 ) {
			tmp = tmp->right;
		} else if( x < tmp->x && tmp->left != 0 ) {
			tmp = tmp->left;
		} else { break; }
	}
	// Разбиваем от найденного элемента.
	CNode* t1 = 0, * t2 = 0;
	subSplit( tmp, x, t1, t2 );
	// Цепляем полученные деревья как левый и правый сыновья добавляемого узла.
	tmp->left = t1;
	tmp->right = t2;
	// Выставляем ключи.
	tmp->x = x;
	tmp->y = y;*/
	CNode* node = new CNode( x, y );
	subInsert( head, node );

}

// Подпрограмма разбиения дерева.
// Проматывает до вершины с ключом key и режет. Результат пишет в l и r.
template<typename T>
void CCartesianTree<T>::subSplit( CCartesianTree::CNode* t, T key, CCartesianTree::CNode*& l, CCartesianTree::CNode*& r )
{
	if( t == 0 ) {
		l = r = 0;
	} else if( key < t->key ) {
		subSplit( t->left, key, l, t->left );
		r = t;
	} else {
		subSplit( t->right, key, t->right, r );
		l = t;
	}
}

template<typename T>
int CCartesianTree<T>::calcHeight() const
{
	curHeight = maxHeight = 0;
	inorderTraversal( head );

	return maxHeight;
}

// Обход, который считает высоту.
template<typename T>
void CCartesianTree<T>::inorderTraversal( CCartesianTree::CNode* node ) const
{
	if( node != 0 ) {
		++curHeight;
		if( curHeight > maxHeight ) {
			maxHeight = curHeight;
		}
		inorderTraversal( node->left );
		inorderTraversal( node->right );
		--curHeight;
	}
}

// Подпрограмма вставки it в дерево t.
template<typename T>
void CCartesianTree<T>::subInsert( CCartesianTree::CNode*& t, CCartesianTree::CNode* it )
{
	if( t == 0 ) { // Если доходим до листа, то вставляем.
		t = it;
	} else if( it->prior > t->prior ) { // Если доходим до первой вершины с меньшим приоритетом
		subSplit( t, it->key, it->left, it->right ); // Разбиваем от этой вершины.
		t = it; 
	} else {
		subInsert( it->key < t->key ? t->left : t->right, it ); //Иначе идём дальше.
	}
}
