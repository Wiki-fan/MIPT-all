#pragma once
#include "stdafx.h"
#include "ITree.h"

class CAVLTree : public ITree<int> {

public:
	CAVLTree() : head( 0 ) {}
	~CAVLTree() { delete head; }

	void Print() const;

	// Вставка элемента с ключом key.
	bool Insert( const int &key );

	// Поиск элемента с ключом key.
	bool Search( const int &key ) const;

	// Удаление элемента с ключом key.
	bool Remove( const int &key );

	// Поиск k-й порядковой статистики.
	int getStatictics( int k ) const;

private:

	struct CNode {
		int key;
		int childLeft, childRight;
		unsigned char height;
		CNode* left;
		CNode* right;
		explicit CNode( int _key )
			: key( _key ), childLeft( 0 ), childRight( 0 ), height( 1 ), left( 0 ), right( 0 )
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

	static CNode* rotateRight( CNode *p );

	static CNode* rotateLeft( CNode *q );

	static CNode* balance( CNode *p );

	CNode* subInsert( CNode *p, int k ) const;

	static CNode* findMin( CNode* p ) // Поиск узла с минимальным ключом в дереве p
	{
		return p->left ? findMin( p->left ) : p;
	}

	static CNode* removeMin( CNode *p );

	CNode* subRemove( CNode *p, int k ) const;

	// Другая, недописанная реализация subRemove.
	/*CNode* findClosest( CNode* p, int key )
	{
		if( p == 0 ) { return 0; }
		int rLeft = abs( p->left?0:p->left->key - key );
		int rRight = abs( p->right?0:p->right->key - key );
		int rCur = abs( p->key - key );
		if( std::min( rLeft, rRight ) < rCur ) {
			if( rLeft < rRight ) {
				return findClosest( p->left, key );
			} else {
				return findClosest( p->right, key );
			}
		} else {
			return p;
		}
	}
	CNode* subRemove2( CNode* p, int k )
	{
		if( p == 0 ) { fl = false; return 0; }
		// Если лист, удаляем.
		if( p->left == 0 && p->right == 0 ) {
			delete p;
			return 0;
		} else {
			CNode* tmp;
			if( getLeftChildCount( p ) > getRightChildCount( p ) ) {
				tmp = findClosest( p->left , k);
			} else {
				tmp = findClosest( p->right, k );
			}
			
		}

		return balance( p );
	}*/

	static bool subSearch( CNode *node, int key );

	void inorderWalk( CNode *n ) const;

};

inline void CAVLTree::Print() const
{
	inorderWalk( head );
}

inline bool CAVLTree::Insert( const int &key )
{
	fl = 1;
	head = subInsert( head, key );
	return fl;
}

inline bool CAVLTree::Search( const int &key ) const
{
	return subSearch( head, key );
}

inline bool CAVLTree::Remove( const int &key )
{
	fl = 1;
	head = subRemove( head, key );
	return fl;
}

int CAVLTree::getStatictics( int k ) const
{
	CNode* p = head;
	for( ; p != 0; ) {
		if( p->left != 0 && getLeftChildCount( p ) > k ) {
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

// Правый поворот вокруг p.
CAVLTree::CNode * CAVLTree::rotateRight( CNode *p )
{
	CNode* q = p->left;
	p->left = q->right;
	q->right = p;
	recalcHeight( p );
	recalcHeight( q );
	recalcChildCount( p );
	recalcChildCount( q );
	return q;
}

// Левый поворот вокруг q.
CAVLTree::CNode * CAVLTree::rotateLeft( CNode *q )
{
	CNode* p = q->right;
	q->right = p->left;
	p->left = q;
	recalcHeight( q );
	recalcHeight( p );
	recalcChildCount( q );
	recalcChildCount( p );
	return p;
}

// Балансировка вершины p.
CAVLTree::CNode * CAVLTree::balance( CNode *p )
{
	recalcHeight( p );
	recalcChildCount( p );
	if( balanceFactor( p ) == 2 ) { // Если фактор равен 2, то
		// Большое левое вращение, если левое поддерево p->right больше правого.
		if( balanceFactor( p->right ) < 0 ) {
			p->right = rotateRight( p->right );
		}
		// Малое левое вращение, иначе.
		return rotateLeft( p );
	}
	if( balanceFactor( p ) == -2 ) { //Если фактор равен -2, то
		// Большое правое вращение, если левое поддерево p->left меньше правого.
		if( balanceFactor( p->left ) > 0 ) {
			p->left = rotateLeft( p->left );
		}
		// Малое правое вращение, иначе.
		return rotateRight( p );
	}
	return p; // Дерево теперь сбалансировано.
}

// Вставка ключа k в дерево с корнем p.
CAVLTree::CNode * CAVLTree::subInsert( CNode *p, int k ) const
{
	if( !p ) { // Если дошли до листа, вставляем.
		CNode* q = new CNode( k );
		return q;
	}
	if( k < p->key ) {
		//++(p->childLeft);
		p->left = subInsert( p->left, k );
	} else if( k>p->key ) {
		//++(p->childRight);
		p->right = subInsert( p->right, k );
	} else {
		fl = false; // Такой элемент уже есть, не вставляем.
	}

	return balance( p ); // Балансируем, ведь высоты могли измениться при вставке.
}

// Удаление узла с минимальным ключом из дерева p.
CAVLTree::CNode * CAVLTree::removeMin( CNode *p )
{
	if( p->left == 0 ) {
		return p->right;
	}
	p->left = removeMin( p->left );
	return balance( p );
}

// Удаление ключа k из дерева p
CAVLTree::CNode * CAVLTree::subRemove( CNode *p, int k ) const
{
	if( !p ) { fl = 0; return 0; } // Нет нужного элемента, нечего удалять.
	if( k < p->key ) {
		p->left = subRemove( p->left, k );
	} else if( k > p->key ) {
		p->right = subRemove( p->right, k );
	} else // k == p->key
	{
		CNode* q = p->left;
		CNode* r = p->right;
		p->left = p->right = 0;
		delete p;
		if( !r ) { return q; }
		CNode* min = findMin( r );
		min->right = removeMin( r );
		min->left = q;
		return balance( min );
	}
	return balance( p );
}

bool CAVLTree::subSearch( CNode *node, int key )
{
	bool ret = false;
	if( node != 0 ) {
		if( node->key < key ) {
			ret = subSearch( node->right, key );
		} else if( node->key > key ) {
			ret = subSearch( node->right, key );
		} else {
			ret = true;
		}
	}
	return ret;
}

void CAVLTree::inorderWalk( CNode *n ) const
{
	if( n != 0 ) {
		inorderWalk( n->left );
		std::cout << n->key << ' ' << ' ' << n->childLeft << ' ' << n->childRight << std::endl;
		inorderWalk( n->right );
	}

}
