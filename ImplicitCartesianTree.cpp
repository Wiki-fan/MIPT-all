#include "ImplicitCartesianTree.h"

// Разделяем по индексу, вставляем в промежуток элемент.
void CImplicitCartesianTree::Insert( int i, int k )
{
	massert( i <= getC( head ));
	CNode* l, * r;
	split( head, i, l, r );
	CNode* temp = new CNode( k );
	recalcAll( temp );
	head = merge( merge( l, temp ), r );
	recalcAll( head );
}

// Вырезаем элемент, меняем значение, сливаем.
void CImplicitCartesianTree::Assign( int i, int k )
{
	massert( i <= getC( head ));
	CNode* l, * m, * r;
	split( head, i, l, r );
	split( r, 1, m, r );
	m->val = k;
	recalcAll( m );
	head = merge( merge( l, m ), r );
}

//Вырезаем нужный полуинтервал, возвращаем накопленную в корне сумму.
int CImplicitCartesianTree::SumSubSegment( int l, int r )
{
	massert( l <= r );
	massert( r <= getC( head ));
	CNode* lt, * mt, * rt;
	split( head, l, lt, mt );
	split( mt, r - l, mt, rt );
	int res = getSum( mt );
	head = merge( merge( lt, mt ), rt );
	return res;
}

bool CImplicitCartesianTree::NextPermutation( int l, int r )
{
	massert( l <= r );
	massert( r <= getC( head ));
	//Если надо сделать перестановку на одном элементе или на пустом множестве.
	if( l == r ) {
		return false;
	}

	// Вырезаем кусок, подлежащий перестановке.
	CNode* lt, * mt, * rt;
	split( head, l, lt, mt );
	split( mt, r - l, mt, rt );

	// Если перестановки закончились (нет лексикографически большей), и нужно начать сначала.
	if( mt->lss == mt->c ) {
		reverse( mt );
		head = merge( merge( lt, mt ), rt );
		return false;
	}

	// lss - наибольший упорядоченный суффикс, то есть наибольший такой индекс, что a[lss]<a[lss]+1.
	CNode* lswap, * rswap, * lpart, * mpart, * rpart;
	// В lpart будет неотсортированный кусок, в mpart - упорядоченные элементы. Нас интересует второе.
	split( mt, mt->c - mt->lss - 1, lpart, mpart );
	// Отрезаем первый элемент.
	split( mpart, 1, lswap, mpart );
	// Разбиваем оставшуюся часть по значению этого элемента.
	splitByVal( mpart, lswap->val, mpart, rpart );
	// Второй элемент для обмена - последний элемент mpart.
	split( mpart, getC( mpart ) - 1, mpart, rswap );
	// Обмениваем элементы и реверсим кусок.
	rpart = merge( mpart, merge( lswap, rpart ));
	reverse( rpart );

	//Сливаем всё.
	mt = merge( lpart, merge( rswap, rpart ));
	head = merge( merge( lt, mt ), rt );
	return true;
}

void CImplicitCartesianTree::split( CNode* p, int i, CNode*& l, CNode*& r )
{
	if( p == 0 ) {
		l = r = 0;
		return;
	}
	push( p );
	// Разделяем дерево в зависимости от индекса искомого и индекса текущей вершины.
	int curIndex = getC( p->left ) + 1;
	if( curIndex > i ) {
		split( p->left, i, l, p->left );
		r = p;
	} else {
		split( p->right, i - curIndex, p->right, r );
		l = p;
	}
	recalcAll( l );
	recalcAll( r );
}

CImplicitCartesianTree::CNode* CImplicitCartesianTree::merge( CNode* l, CNode* r )
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
	// Перевешиваем деревья в зависимости от приоритета.
	if( l->priority >= r->priority ) {
		l->right = merge( l->right, r );
		recalcAll( l );
		return l;
	} else {
		r->left = merge( l, r->left );
		recalcAll( r );
		return r;
	}
}

void CImplicitCartesianTree::splitByVal( CNode* t, int val, CNode*& l, CNode*& r )
{
	if( t == 0 ) {
		l = r = 0;
		return;
	}
	push( t );
	if( t->val <= val ) {
		splitByVal( t->left, val, l, t->left );
		r = t;
		recalcAll( l );
		recalcAll( r );
	} else {
		splitByVal( t->right, val, t->right, r );
		l = t;
		recalcAll( l );
		recalcAll( r );
	}
}