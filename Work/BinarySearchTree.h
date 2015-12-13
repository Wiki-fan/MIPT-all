#pragma once
#include "stdafx.h"
#include "TreeBase.h"

template<typename T>
class CBinarySearchTree : public CTreeBase<T> {

public:
	CBinarySearchTree() : head( 0 ), height( 0 ), curHeight( 0 ) { }
	~CBinarySearchTree() { delete head; }

	void Insert( const T& key );
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

	CNode* subInsert( CNode* node, T key );

};

template<typename T>
void CBinarySearchTree<T>::Insert( const T& key )
{
	curHeight = 0;
	head = subInsert( head, key );
}

template<typename T>
typename CBinarySearchTree<T>::CNode* CBinarySearchTree<T>::subInsert( CBinarySearchTree::CNode* node, T key )
{
	if( node == 0 ) {
		node = new CNode( key );
		++curHeight;
		if( curHeight > height ) {
			height = curHeight;
		}
	} else if( key < node->key ) {
		++curHeight;
		node->left = subInsert( node->left, key );
	} else {
		++curHeight;
		node->right = subInsert( node->right, key );
	}
	return node;
}
