#pragma once
#include "stdafx.h"

class CAVLTree {
public:
	CAVLTree() : head( 0 ) {}
	~CAVLTree() { delete head; }

	void Print() const
	{
		inorderWalk( head );
	}

	bool Insert( int key )
	{
		fl = 1;
		head = subInsert( head, key );
		return fl;
	}

	bool Search( int key ) const
	{
		return subSearch( head, key );
	}

	bool Remove( int key )
	{
		fl = 1;
		head = subRemove( head, key );
		return fl;
	}

	int getStatictics( int k ) const
	{
		CNode* p = head;
		for( ; p != 0; ) {
			if( p->left != 0 && getLeftChildCount( p ) > k ) {
				k -= /*getRightChildCount( p ) +*/ 1;
				p = p->left;
			} else if( p->right != 0 && getLeftChildCount( p ) < k ) {
				k -= getLeftChildCount( p ) + 1;
				p = p->right;
			} else {
				return p->key;
			}
		}
		return 0;
	}

private:

	struct CNode {
		int key;
		int childLeft, childRight;
		unsigned char height;
		CNode* left;
		CNode* right;
		explicit CNode( int _key )
			: key( _key ), left( 0 ), right( 0 ), height( 1 ), childLeft( 0 ), childRight( 0 )
		{
		}
		~CNode()
		{
			delete left;
			delete right;
		}
	};

	CNode* head;
	mutable bool fl;

	static unsigned char getHeight( CNode* node )
	{
		return node ? node->height : 0;
	}

	static int getLeftChildCount( CNode* node )
	{
		return node ? node->childLeft : 0;
	}

	static void setLeftChildCount( CNode* node, int count )
	{
		if( node != 0 ) {
			node->childLeft = count;
		}
	}
	static void setRightChildCount( CNode* node, int count )
	{
		if( node != 0 ) {
			node->childRight = count;
		}
	}
	static int getRightChildCount( CNode* node )
	{
		return node ? node->childRight : 0;
	}

	static void recalcHeight( CNode* node )
	{
		unsigned char hl = getHeight( node->left );
		unsigned char hr = getHeight( node->right );
		node->height = std::max( hl, hr ) + 1;
	}

	static void recalcChildCount( CNode* node )
	{
		node->childLeft = getLeftChildCount( node->left )
			+ getRightChildCount( node->left )
			+ (node->left == 0 ? 0 : 1);
		node->childRight = getLeftChildCount( node->right )
			+ getRightChildCount( node->right )
			+ (node->right == 0 ? 0 : 1);
	}

	static int balanceFactor( CNode* p )
	{
		return getHeight( p->right ) - getHeight( p->left );
	}

	static CNode* rotateRight( CNode* p )
	{
		CNode* q = p->left;
		/*int A = getLeftChildCount( q ),
				B = getRightChildCount( q ),
				C = getRightChildCount( p );*/
		p->left = q->right;
		q->right = p;
		/*setLeftChildCount( p, B);
		setRightChildCount( p, C );
		setLeftChildCount( q, A );
		setRightChildCount( q, getLeftChildCount( p ) + getRightChildCount( p ) + p == 0 ? 0 : 1 );*/
		recalcHeight( p );
		recalcHeight( q );
		recalcChildCount( p );
		recalcChildCount( q );
		return q;
	}

	static CNode* rotateLeft( CNode* q ) // левый поворот вокруг q
	{
		CNode* p = q->right;
		/*int A = getChildCount( q->left ),
				B = getChildCount( p->left ),
				C = getChildCount( p->right );*/
				/*int A = getLeftChildCount( q ),
					B = getLeftChildCount( p ),
					C = getRightChildCount( p );*/
		q->right = p->left;
		p->left = q;
		/*setChildCount( q, B + C );
		setChildCount( p, getChildCount( q ) + A );*/
		/*setLeftChildCount( q, A );
		setRightChildCount( q, B );
		setLeftChildCount( p, C );
		setRightChildCount( p, getLeftChildCount( q ) + getRightChildCount( q ) + q == 0 ? 0 : 1 );*/
		recalcHeight( q );
		recalcHeight( p );
		recalcChildCount( q );
		recalcChildCount( p );
		return p;
	}

	static CNode* balance( CNode* p ) // балансировка узла p
	{
		recalcHeight( p );
		recalcChildCount( p );
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
			++(p->childLeft);
			p->left = subInsert( p->left, k );
		} else if( k>=p->key ) {
			++(p->childRight);
			p->right = subInsert( p->right, k );
		} else {
			fl = false;
		}
		return balance( p );
	}

	static CNode* findMin( CNode* p ) // поиск узла с минимальным ключом в дереве p
	{
		return p->left ? findMin( p->left ) : p;
	}

	static CNode* removeMin( CNode* p ) // удаление узла с минимальным ключом из дерева p
	{
		if( p->left == 0 ) {
			return p->right;
		}
		p->left = removeMin( p->left );
		return balance( p );
	}

	CNode* subRemove( CNode* p, int k ) // удаление ключа k из дерева p
	{

		if( !p ) { fl = 0; return 0; }
		if( k < p->key ) {
			p->left = subRemove( p->left, k );
		} else if( k > p->key ) {
			p->right = subRemove( p->right, k );
		} else //  k == p->key
		{
			CNode* q = p->left;
			CNode* r = p->right;
			p->left = p->right = 0;
			delete p;
			if( !r ) { return q; }
			CNode* min = findMin( r );
			min->right = removeMin( r );
			min->left = q;
			/*recalcChildCount(min->right);
			recalcChildCount(min->left);
			recalcChildCount(min);*/
			return balance( min );
		}
		return balance( p );
	}

	static bool subSearch( CNode* node, int key )
	{
		bool ret = 0;
		if( node != 0 ) {
			if( node->key < key ) {
				ret = subSearch( node->right, key );
			} else if( node->key > key ) {
				ret = subSearch( node->right, key );
			} else {
				ret = 1;
			}
		}
		return ret;
	}

	void inorderWalk( CNode* n ) const
	{
		if( n != 0 ) {
			inorderWalk( n->left );
			std::cout << n->key << ' ' << ' ' << n->childLeft << ' ' << n->childRight << std::endl;
			inorderWalk( n->right );
		}

	}


};
