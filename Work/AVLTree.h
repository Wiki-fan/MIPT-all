#pragma once
#include "stdafx.h"

class AVLTree {
public:
	AVLTree() : head( 0 ) { }
	~AVLTree() { delete head; }

	void Insert( int key )
	{
		head = subInsert( head, key );
	}

	void Remove( int key )
	{
		subRemove( head, key );
	}

	int getStatictics( int k )
	{
		/*CNode* p = head;
		for( ; p != 0; ) {
			if( p->left != 0 && getChildCount(p->left) > k ) {
				p = p->right;
			} else if( p->right != 0 && getChildCount(p->left) < k ) {
				p = p->right;
			} else {
				return p->key;
			}
		}*/
	}

private:

	struct CNode {
		int key;
		int childCount;
		unsigned char height;
		CNode* left;
		CNode* right;
		explicit CNode( int _key ) : key( _key ), left( 0 ), right( 0 ), height( 1 ), childCount( 0 ) { }
		~CNode()
		{
			delete left;
			delete right;
		}
	};

	CNode* head;

	unsigned char getHeight( CNode* node )
	{
		return node ? node->height : 0;
	}

	int getChildCount( CNode* node )
	{
		return node ? node->childCount : 0;
	}

	void setChildCount( CNode* node, int childCount )
	{
		if( node != 0 ) { node->childCount = childCount; }
	}

	unsigned char recalcHeight( CNode* node )
	{
		unsigned char hl = getHeight( node->left );
		unsigned char hr = getHeight( node->right );
		node->height = std::max( hl, hr ) + 1;
	}

	int balanceFactor( CNode* p )
	{
		return getHeight( p->right ) - getHeight( p->left );
	}

	CNode* rotateRight( CNode* p )
	{
		CNode* q = p->left;
		int A = getChildCount( q->left ),
				B = getChildCount( q->right ),
				C = getChildCount( p->right );
		p->left = q->right;
		q->right = p;
		setChildCount( p, B + C );
		setChildCount( q, A );
		recalcHeight( p );
		recalcHeight( q );
		return q;
	}

	CNode* rotateLeft( CNode* q ) // левый поворот вокруг q
	{
		CNode* p = q->right;
		int A = getChildCount( q->left ),
				B = getChildCount( p->left ),
				C = getChildCount( p->right );
		q->right = p->left;
		p->left = q;
		setChildCount( q, B + C );
		setChildCount( p, getChildCount( q ) + A );
		recalcHeight( q );
		recalcHeight( p );
		return p;
	}

	CNode* balance( CNode* p ) // балансировка узла p
	{
		recalcHeight( p );
		if( balanceFactor( p ) == 2 ) {
			if( balanceFactor( p->right ) < 0 ) {
				p->right = rotateRight( p->right );
			}
			return rotateLeft( p );
		}
		if( balanceFactor( p ) == -2 ) {
			if( balanceFactor( p->left ) > 0 ) {
				p->left = rotateLeft( p->left );
			}
			return rotateRight( p );
		}
		return p; // балансировка не нужна
	}

	CNode* subInsert( CNode* p, int k ) // вставка ключа k в дерево с корнем p
	{
		if( !p ) {
			CNode* q = new CNode( k );
			return q;
		}
		if( k < p->key ) {
			++( p->childCount );
			p->left = subInsert( p->left, k );
		} else {
			++( p->childCount );
			p->right = subInsert( p->right, k );
		}
		return balance( p );
	}

	CNode* findMin( CNode* p ) // поиск узла с минимальным ключом в дереве p
	{
		return p->left ? findMin( p->left ) : p;
	}

	CNode* removeMin( CNode* p ) // удаление узла с минимальным ключом из дерева p
	{
		if( p->left == 0 ) {
			return p->right;
		}
		p->left = removeMin( p->left );
		return balance( p );
	}

	CNode* subRemove( CNode* p, int k ) // удаление ключа k из дерева p
	{

		if( !p ) { return 0; }
		if( k < p->key ) {
			p->left = subRemove( p->left, k );
		} else if( k > p->key ) {
			p->right = subRemove( p->right, k );
		} else //  k == p->key
		{
			CNode* q = p->left;
			CNode* r = p->right;
			delete p;
			if( !r ) { return q; }
			CNode* min = findMin( r );
			min->right = removeMin( r );
			min->left = q;
			return balance( min );
		}
		return balance( p );
	}


};
