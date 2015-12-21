#pragma once
#include "stdafx.h"
#include "ITree.h"

template<typename T>
class CAVLTree : public ITree<T> {

public:
	CAVLTree() : head( 0 ), fl(false) {}
	~CAVLTree() { delete head; }

	// Вывод дерева на экран, пригодно в основном для дебажных целей.
	void Print() const;

	// Вставка элемента с ключом key.
	bool Insert( const T& key ) override;
	// Поиск элемента с ключом key.
	bool Search( const T& key ) const override;
	// Удаление элемента с ключом key.
	bool Remove( const T& key ) override;

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
	static int getRightChildCount( CNode* node )
	{
		return node ? node->childRight : 0;
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

template<typename T>
inline void CAVLTree<T>::Print() const
{
	inorderWalk( head );
}
template<typename T>
inline bool CAVLTree<T>::Insert( const T &key )
{
	fl = 1;
	head = subInsert( head, key );
	return fl;
}
template<typename T>
inline bool CAVLTree<T>::Search( const T &key ) const
{
	return subSearch( head, key );
}
template<typename T>
inline bool CAVLTree<T>::Remove( const T &key )
{
	fl = 1;
	head = subRemove( head, key );
	return fl;
}
template<typename T>
int CAVLTree<T>::getStatictics( int k ) const
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
template<typename T>
auto CAVLTree<T>::rotateRight( CNode *p ) -> CNode *
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
template<typename T>
auto CAVLTree<T>::rotateLeft( CNode *q ) -> CNode *
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
template<typename T>
auto CAVLTree<T>::balance( CNode *p ) -> CNode *
{
	if (p == 0 ) {
		return 0;
	}
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
template<typename T>
auto CAVLTree<T>::subInsert( CNode *p, int k ) const -> CNode *
{
	if( !p ) { // Если дошли до листа, вставляем.
		CNode* q = new CNode( k );
		return q;
	}
	if( k < p->key ) {
		p->left = subInsert( p->left, k );
	} else if( k>p->key ) {
		p->right = subInsert( p->right, k );
	} else {
		fl = false; // Такой элемент уже есть, не вставляем.
		return 0;
	}

	return balance( p ); // Балансируем, ведь высоты могли измениться при вставке.
}

// Отцепление узла с минимальным ключом от дерева p.
template<typename T>
auto CAVLTree<T>::removeMin( CNode *p ) -> CNode *
{
	if( p->left == 0 ) {
		return p->right;
	}
	p->left = removeMin( p->left );
	return balance( p );
}

// Удаление ключа k из дерева p
template<typename T>
auto CAVLTree<T>::subRemove( CNode *p, int k ) const -> CNode *
{
	if( !p ) { fl = 0; return 0; } // Нет нужного элемента, нечего удалять.
	if( k < p->key ) {
		p->left = subRemove( p->left, k );
	} else if( k > p->key ) {
		p->right = subRemove( p->right, k );
	} else {
		// Ежели k == p->key
		CNode* l = p->left;
		CNode* r = p->right;
		// Удаляем вершину.
		p->left = p->right = 0;
		delete p;
		if( !r ) { return l; }
		// Вставляем минимум правого поддерева вместо вершины.
		CNode* min = findMin( r );
		min->right = removeMin( r );
		min->left = l;
		return balance( min );
	}
	return balance( p );
}

// Ищет узел с ключом key.
template<typename T>
bool CAVLTree<T>::subSearch( CNode *node, int key )
{
	if( node == 0 ) {
		return false;
	} else {
		if( node->key < key ) {
			return subSearch( node->right, key );
		} else if( node->key > key ) {
			return subSearch( node->left, key );
		} else {
			return true;
		}
	}
}
template<typename T>
void CAVLTree<T>::inorderWalk( CNode *n ) const
{
	if( n != 0 ) {
		inorderWalk( n->left );
		std::cout << n->key << ' ' << ' ' << n->childLeft << ' ' << n->childRight << std::endl;
		inorderWalk( n->right );
	}

}
