#pragma once
#include "stdafx.h"
#include "ImplicitTree.h"

// Сплэй-дерево.
class CImplicitSplayTree : public IImplicitTree {

public:
	CImplicitSplayTree() : head( 0 ), fl( 0 ) { }
	~CImplicitSplayTree() { delete head; }

	// Вставка элемента со значением k на позицию i.
	void Insert( int i, int k ) override
	{
		CNode* l, * r;
		split( head, i, l, r );
		CNode* temp = new CNode( k );
		head = merge( merge( l, temp ), r );
	}

	// Присвоение элементу с индексом i значения k.
	void Assign( int i, int k ) override
	{
		CNode* l, * r;
		split( head, i, l, r );
		r->val = k;
		recalcSum( r );
		head = merge( l, r );
	}

	int SumSubSegment( int l, int r ) override
	{
		CNode* lt, * mt, * rt;
		split( head, l, lt, mt );
		split( mt, r - l, mt, rt );
		int res = getSum( mt );
		head = merge( merge( lt, mt ), rt );
		return res;
	}

	bool NextPermutation( int l, int r ) override
	{
		// Вырезаем кусок, подлежащий перестановке.
		CNode* lt, * mt, * rt;
		split( head, l, lt, mt );
		//printf( "\nlt: " );print( lt );
		split( mt, r - l, mt, rt );
		//printf( "\nmt: " );print( mt );
		//printf( "\nrt: " );print( rt );

		// lss - наибольший упорядоченный суффикс, то есть наибольший такой индекс, что a[lss]<a[lss]+1.
		CNode* p1, * p2;
		// В p1 неотсортированный кусок, в p2 - упорядоченные элементы. Нас интересует второе.
		//printf( "\nmt.lss: %d", mt->lss );
		//Если надо сделать перестановку на пустом множестве.
		if( mt == 0 ) {
			head = merge( lt, rt );
			return true;
		}
		// Если перестановки закончились, и нужно начать сначала.
		if( mt->lss == mt->c ) {
			mt->reversed ^= true;
			head = merge( merge( lt, mt ), rt );
			return false;
		}
		split( mt, mt->c - mt->lss - 1, p1, p2 );
		//printf( "\np1: " );print( p1 );
		//printf( "\np2: " );print( p2 );
		// Опускаемся по дереву и ищем наибольшее k, такое что a[j]<a[k]
		p2 = subSearch( p2, 0 );
		CNode* cur = p2, * prev = p2;
		int previ = 0, curi = 0;
		while( cur != 0 ) {
			if( cur->val < prev->val ) {
				//previ -= getC( cur->right ) + 1;
				curi += 1;
				cur = cur->left;
			} else {
				curi += getC( /*prev*/cur->left ) + 1;
				previ = curi;
				cur = cur->right;
			}
		}
		//printf( "\nprev: %d", previ );

		// Теперь нужно обменять значения prev и p1, корректно пересчитав суммы.
		CNode* r2, * r3;
		split( p2, ( previ - 1 ) == 0 ? 1 : ( previ - 1 )/*previ+1*/, r2, r3 );
		//printf( "\nr3: " );print( r3 );
		r2 = subSearch( r2, 0 );
		//printf( "\nr2: " );print( r2 );

		// Меняем и пересчитываем суммы.
		std::swap( r2->val, r3->val );
		recalcSum( r3 );
		recalcSum( r2 );
		recalcLS( r3 );
		recalcLS( r2 );

		r2 = merge( r2, r3 );
		//printf( "\nr2: " );print( r2 );
		split( r2, 1, r2, r3 ); // Отрезаем элемент j, чтобы потом отреверсить кусок (j+1)..n
		//printf( "\nr3: " );print( r3 );
		//printf( "\nr2: " );print( r2 );
		r3->reversed ^= true;
		push( r3 ); //TODO

		r2 = merge( r2, r3 );
		//printf( "\nr2: " );print( r2 );
		// Слияние.
		mt = merge( p1, r2 );
		//printf( "\nmt: " );print( mt );
		head = merge( merge( lt, mt ), rt );
		return true;
	}

//protected:
public:
	struct CNode {
		CNode* parent;
		CNode* left;
		CNode* right;
		int c; // Количество вершин в поддереве
		int sum; // Сумма вершин поддерева.
		int val; // Значение вершины.
		bool reversed; // Отложенная модификация.
		int lss; // Largest Sorted Suffix.
		CNode() = delete;
		explicit CNode( int _val )
				: parent( 0 ), left( 0 ), right( 0 ), c( 1 ), sum( _val ), val( _val ), lss( 1 )
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

	void inorderTraversal( CNode* node, std::vector<int>& vec ) const
	{
		if( node != 0 ) {
			inorderTraversal( node->left, vec );
			vec.push_back( node->val );
			inorderTraversal( node->right, vec );
		}
	}

	void print( CNode* node )
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
		if( p == 0 || p->reversed == false ) {
			return;
		}
		if( p->left != 0 ) {
			p->left->reversed ^= p->reversed;
		}
		if( p->right != 0 ) {
			p->right->reversed ^= p->reversed;
		}
		std::swap( p->left, p->right );
		recalcLS( p );
		p->reversed = false;
		push( p->left );
		push( p->right );
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

	// Обновить lss и lsp.
	static void recalcLS( CNode* node )
	{
		if( node == 0 ) {
			return;
		}

		if( node->right != 0 ) {
			node->lss = node->right->lss; // LSS - как минимум LSS правого ребёнка.
			// Если правый сын есть отсорт. суффикс себя, то можно расширить.
			if( node->right->lss/*-1*/ == node->right->c ) {
				//node->lss += node->left == 0 ? 0 : ( node->left->lss );
				if( node->val >= node->right->val ) {
					node->lss += 1;

				}

			}
		} else {
			node->lss = 1;
		}
		if( node->left != 0 ) {
			// node->left = subSearch( node->left, node->left->c -1 );
			CNode* tmp = node->left;
			while( tmp->right ) {
				tmp = tmp->right;
			}
			if( tmp->val >= node->val ) {
				node->lss += node->left->lss;
			}
		}


		/*// Префиксы - симметрично.
		if( node->left != 0 ) {
			node->lsp = node->left->lsp;
			if( node->left->lsp == node->left->c && node->left->val > node->val ) {
				node->lsp += 1 + node->right == 0 ? 0 : ( node->right->lsp );
			}
		} else {
			node->lsp = 0;
		}*/
	}

	static void recalcAll( CNode* node )
	{
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

	static void split( CNode* p, int key, CNode*& l, CNode*& r );

	// Сливает два поддерева. Гарантируется, что l->key < r->key.
	static CNode* merge( CNode* l, CNode* r );

};

// Поворот p вокруг parent. Сам понимает, левый или правый.
inline void CImplicitSplayTree::rotate( CNode*& p, CNode*& parent )
{
	// Нужен только для подцепления результата.
	CNode* gparent = parent->parent;
	push( gparent );
	push( parent );
	push( p );
	// Встраиваем результат поворота в исходное дерево.
	if( gparent != 0 ) {
		// p будет новой верхушкой.
		if( gparent->left == parent ) {
			gparent->left = p;
		} else {
			gparent->right = p;
		}
	}

	if( parent->left == p ) {
		parent->left = p->right;
		p->right = parent;
	} else {
		parent->right = p->left;
		p->left = parent;
	}
	setChildsParent( p );
	setChildsParent( parent );
	p->parent = gparent;

	recalcAll( parent );
	recalcAll( p );
	recalcAll( gparent );
}

// Поднимает данную вершину в корень.
inline CImplicitSplayTree::CNode* CImplicitSplayTree::splay( CNode* p )
{
	if( p == 0 ) {
		return 0;
	}
	// p уже в корне, некуда тянуть.
	if( p->parent == 0 ) {
		return p;
	}
	push( p );
	CNode* parent = p->parent;
	CNode* gparent = parent->parent;
	// zig
	if( gparent == 0 ) {
		rotate( p, parent );
		return p;
	} else {
		bool zigzig = ( gparent->left == parent ) == ( parent->left == p );
		// zigzig
		if( zigzig ) {
			rotate( parent, gparent );
			rotate( p, parent );
		} else { // zigzag
			rotate( p, parent );
			rotate( p, gparent );
		}
	}

	return splay( p );
}

// Подпрограмма поиска, вызывает splay от вершины i.
inline CImplicitSplayTree::CNode* CImplicitSplayTree::subSearch( CNode* node, int i )
{
	if( node == 0 ) { // Если нужной вершины не имеется...
		return 0; // возвращаем 0.
	} else { // Иначе ищем дальше.
		push( node );
		int lsize = getC( node->left );
		if( lsize < i && node->right != 0 ) {
			return subSearch( node->right, i - lsize - 1 );
		} else if( lsize > i && node->left != 0 ) {
			return subSearch( node->left, i );
		} else {
			return splay( node ); // Тащим вверх.
		}
	}
	/*while( node != 0 ) {
		int lsize = getC( node->left );
		if( lsize < i && node->right != 0 ) {
			node = node->right;
			i = i - lsize - 1;
		} else if( lsize > i && node->left != 0 ) {
			node = node->left;
		} else {
			return splay( node );
		}
	}
	return 0;*/
}

// Разбивает дерево с корнем p по ключу key.
// В левой половине будут вершины с номером меньше (i штук), в правой - больше или равно.
inline void CImplicitSplayTree::split( CNode* p, int i, CNode*& l, CNode*& r )
{
	if( p == 0 ) {
		l = r = 0;
		return;
	}
	push( p );
	// Отыскиваем нужную вершину и тянем в корень.
	p = subSearch( p, i );
	// Отцепляем у корня левого ребёнка.
	l = p->left;
	p->left = 0;
	setParent( l, 0 );
	r = p;
	// Пересчитываем количество детей и сумму.
	recalcAll( l );
	recalcAll( r );
}

// Сливает два дерева, возвращает результат.
inline CImplicitSplayTree::CNode* CImplicitSplayTree::merge( CNode* l, CNode* r )
{
	if( r == 0 ) {
		return l;
	}
	if( l == 0 ) {
		return r;
	}

	//Проталкиваем отложенные модификации.
	push( l );
	push( r );
	// Тянем в корень нулевую вершину правого дерева
	r = subSearch( r, 0 );
	r->left = l; // Цепляем l как левого ребёнка r.
	l->parent = r; // Выставляем правильного родителя.
	// Пересчитываем количество детей и сумму.
	recalcAll( l );
	recalcAll( r );
	return r;
}
