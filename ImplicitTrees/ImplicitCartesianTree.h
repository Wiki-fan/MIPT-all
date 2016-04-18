#pragma once
#include "../stdafx.h"
#include "ImplicitTree.h"

// Декартово дерево по неявному ключу.
class CImplicitCartesianTree : public IImplicitTree {

public:
	CImplicitCartesianTree() : head( 0 ) { }
	~CImplicitCartesianTree() { delete head; }

	// Вставка элемента k по индексу i.
	void Insert( int i, int k );

	// Присвоение элементу с индексом i значения k.
	void Assign( int i, int k );

	// Сумма на полуинтервале [l, r).
	int SumSubSegment( int l, int r );

	// Лексикографически следующая перестановка на полуинтервале [l, r).
	bool NextPermutation( int l, int r );

	// Распечатывает дерево и возвращает вектор со значениями дерева.
	void Print()
	{
		std::vector<int> vec;
		printf( "Cartes:" );
		print( head );
		printf( "\n" );
	}

	// Делает inorder-обход и результаты возвращает в виде вектора.
	std::vector<int> Inorder()
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
		int priority; // Приоритет декартового дерева.
		int sum; // Сумма вершин поддерева.
		int val; // Значение вершины.
		bool reversed; // Отложенная модификация.
		int lss, lsp; // Largest Sorted Suffix/Prefix.
		int last, first; // Border values
		CNode() = delete;
		explicit CNode( int _val )
				: parent( 0 ), left( 0 ), right( 0 ), c( 1 ), sum( _val ), val( _val ), lss( 1 ), lsp( 1 ), last( val ),
				  first( val ), priority( rand()), reversed( false )
		{
		}
		CNode( int _val, CNode* _left, CNode* _right, CNode* _parent )
				: parent( _parent ), left( _left ), right( _right ), val( _val ), priority( rand()), reversed( false )
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
			push( node );
			inorderTraversal( node->left, vec );
			vec.push_back( node->val );
			inorderTraversal( node->right, vec );
		}
	}

	static void print( CNode* node )
	{
		if( node != 0 ) {
			push( node );
			print( node->left );
			printf( "%d ", node->val );
			print( node->right );
		}
	}

	CNode* head;

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

	// Получить количество детей в поддереве с вершиной в данной или 0.
	static int getC( CNode* p )
	{
		return p == 0 ? 0 : p->c;
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

	static void recalcLS( CNode* node );

	// Пересчитать всё.
	static void recalcAll( CNode* node )
	{
		push( node );
		recalcC( node );
		recalcSum( node );
		recalcLS( node );
	}

	static void split( CNode* p, int i, CNode*& l, CNode*& r );

	static CNode* merge( CNode* l, CNode* r );

	static void splitByVal( CNode* t, int val, CNode*& l, CNode*& r );

};
