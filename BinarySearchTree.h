#pragma once
#include "stdafx.h"
#include "ITree.h"

// Обычное бинарное дерево поиска.
template<typename T>
class CBinarySearchTree : public ITree<T> {

public:
	CBinarySearchTree() : head( 0 ), height( 0 ), curHeight( 0 ), fl(false)
	{ }
	~CBinarySearchTree() { delete head; }

	// Вставка элемента с ключом k.
	bool Insert( const T& key ) override;
	// Поиск элемента с ключом key.
	bool Search( const T& key ) const override;
	// Удаление элемента с ключом key.
	bool Remove( const T& key ) override
	{
		// TODO: реализовать.
		massert( false );
		return false;
	}
	
	// Высота дерева.
	int getHeight() const { return height; }

private:
	struct CNode {
		CNode() { }
		CNode( T _key ) : key( _key ) { }
		~CNode()
		{
			delete left;
			delete right;
		}
		T key;
		CNode* left;
		CNode* right;
		CNode* parent;
	};

	CNode* head;
	int height;
	int curHeight;
	bool fl;

	CNode* subInsert( CNode* node, T key );
	static CNode* subSearch( CNode* node, T key );

};

template<typename T>
bool CBinarySearchTree<T>::Insert( const T& key )
{
	fl = 1;
	curHeight = 0;
	head = subInsert( head, key );
	return fl;
}

template <typename T>
bool CBinarySearchTree<T>::Search( const T& key ) const
{
	return (bool) subSearch( head, key );
}

// Подпрограмма вставки.
template<typename T>
typename CBinarySearchTree<T>::CNode* CBinarySearchTree<T>::subInsert( CBinarySearchTree::CNode* node, T key )
{
	if( node == 0 ) { // Если лист, то вставляем.
		node = new CNode( key );
		++curHeight;
		if( curHeight > height ) {
			height = curHeight;
		}
	} else if( key < node->key ) { // Иначе идём дальше.
		++curHeight;
		node->left = subInsert( node->left, key );
	} else if (key>node->key) {
		++curHeight;
		node->right = subInsert( node->right, key );
	} else {
		fl = 0;
		return 0;
	}
	return node;
}

// Ищет вершину с ключом key.
template <typename T>
static typename CBinarySearchTree<T>::CNode* CBinarySearchTree<T>::subSearch( CNode* node, T key )
{
	if( node == 0 || node->key == key ) { // Либо найдена, либо таковой не имеется.
		return node;
	} else { // Иначе ищем дальше.
		if( node->key < key ) {
			return subSearch( node->right, key );
		} else if( node->key > key ) {
			return subSearch( node->left, key );
		}
	}
}
