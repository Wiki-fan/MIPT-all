#pragma once
#include "../stdafx.h"
#include "ImplicitTree.h"

// Сплэй-дерево по неявному ключу.
class CImplicitSplayTree : public IImplicitTree {

public:
	CImplicitSplayTree() : head( 0 ), fl( 0 ) { }
	~CImplicitSplayTree() { delete head; }

	// Вставка элемента k по индексу i.
	void Insert( int i, int k );

	// Присвоение элементу с индексом i значения k.
	void Assign( int i, int k );

	// Сумма на подотрезке [l. r).
	int SumSubSegment( int l, int r );

	// Лексикографически следующая перестановка элементов подотрезка [l, r).
	bool NextPermutation( int l, int r );

	// Распечатывает дерево и возвращает вектор со значениями дерева.
	void Print()
	{
		std::vector<int> vec;
		printf( "Splay: " );
		print( head );
		printf( "\n" );
	}

	std::vector<int> InorderDump()
	{
		std::vector<int> vec;
		inorderTraversal( head, vec );
		return vec;
	}

protected:
	struct CNode {
		CNode* parent;
		CNode* left;
		CNode* right;
		int c; // Количество вершин в поддереве
		int sum; // Сумма вершин поддерева.
		int val; // Значение вершины.
		bool reversed; // Отложенная модификация.
		int lss, lsp; // Largest Sorted Suffix/Prefix.
		int last, first; // Border values
		CNode() = delete;
		explicit CNode( int _val )
				: parent( 0 ), left( 0 ), right( 0 ), c( 1 ), sum( _val ), val( _val ), lss( 1 ), lsp( 1 ), last( val ),
				  first( val )
		{
		}
		CNode( int _val, CNode* _left, CNode* _right, CNode* _parent )
				: parent( _parent ), left( _left ), right( _right ), val( _val )
		{
			recalcAll( this );
		}
		~CNode()
		{
			delete left;
			delete right;
		}

	};

	static void inorderTraversal( CNode* node, std::vector<int>& vec )
	{
		if( node != 0 ) {
			inorderTraversal( node->left, vec );
			vec.push_back( node->val );
			inorderTraversal( node->right, vec );
		}
	}

	static void print( CNode* node )
	{
		if( node != 0 ) {
			print( node->left );
			printf( "%d ", node->val );
			print( node->right );
		}
	}

	mutable CNode* head;
	bool fl;

	// Проталкивание отложенных изменений.
	static void push( CNode* p )
	{
		if( p == 0 || !p->reversed ) {
			return;
		}

		std::swap( p->left, p->right );
		reverse( p->left );
		reverse( p->right );

		recalcLS( p );
		p->reversed = false;
		push( p->left );
		push( p->right );
	}

	// Реверс отрезка, соответствующего вершине p.
	static void reverse( CNode* p )
	{
		if( p != 0 ) {
			p->reversed ^= true;
			std::swap( p->lss, p->lsp );
			std::swap( p->first, p->last );
		}
	}

	static int findNearestBiggerThan( CNode*& p, int value );

	// Получить количество детей в поддереве с вершиной в данной или 0.
	static int getC( CNode* p )
	{
		return p == 0 ? 0 : p->c;
	}

	static int getVal( CNode* p )
	{
		return p == 0 ? 0 : p->val;
	}

	// Получить сумму в поддереве с вершиной в данной или 0.
	static int getSum( CNode* p )
	{
		return p == 0 ? 0 : p->sum;
	}

	static int getLSS( CNode* p )
	{
		return p ? p->lss : 0;
	}

	static int getLSP( CNode* p )
	{
		return p ? p->lsp : 0;
	}

	// Пересчитать количество детей в поддереве с вершиной в данной по c в детях.
	static void recalcC( CNode* p )
	{
		if( p != 0 ) {
			p->c = getC( p->left ) + getC( p->right ) + 1;
		}
	}

	// Пересчитать сумму в поддереве с вершиной в данной по сумме в детях.
	static void recalcSum( CNode* p )
	{
		if( p != 0 ) {
			p->sum = getSum( p->left ) + getSum( p->right ) + p->val;
		}
	}

	// Пересчитать префиксы/суффиксы.
	static void recalcLS( CNode* node );

	// Пересчитать всё.
	static void recalcAll( CNode* node )
	{
		push( node );
		recalcC( node );
		recalcSum( node );
		recalcLS( node );
	}

	static void setParent( CNode* p, CNode* parent )
	{
		if( p != 0 ) {
			p->parent = parent;
		}
	}

	static void setChildsParent( CNode* p )
	{
		setParent( p->left, p );
		setParent( p->right, p );
	}

	static void rotate( CNode*& p, CNode*& parent );

	static CNode* splay( CNode* p );

	// Ищет вершину с ключом key (или ближайшую к ней по значению, если вершины с key нет), и тянет её вверх.
	static CNode* subSearch( CNode* node, int key );

	static CNode* nonRecursiveSubSearch( CNode* node, int i );

	static CNode* subValSearch( CNode* node, int val )
	{
		if( node == 0 ) { // Если нужной вершины не имеется...
			return 0; // возвращаем 0.
		} else { // Иначе ищем дальше.
			if( node->val > val && node->right != 0 ) {
				return subSearch( node->right, val );
			} else if( node->val < val && node->left != 0 ) {
				return subSearch( node->left, val );
			} else {
				return splay( node ); // Тащим вверх.
			}
		}
	}
	static void splitByVal( CNode* p, int val, CNode*& l, CNode*& r )
	{
		if( p == 0 ) {
			l = r = 0;
			return;
		}
		// Отыскиваем нужную вершину.
		p = subValSearch( p, val );
		// Нужным образом перенавешиваем вершины.
		if( p->val >= val ) {
			r = p->right;
			p->right = 0;
			setParent( r, 0 );
			l = p;
		} else {
			l = p->left;
			p->left = 0;
			setParent( l, 0 );
			r = p;
		}
	}

	void split( CNode* p, int key, CNode*& l, CNode*& r );

	void recursiveSplit( CNode* p, int i, CNode*& l, CNode*& r );

	// Сливает два поддерева. Гарантируется, что l->key < r->key.
	static CNode* merge( CNode* l, CNode* r );

};

