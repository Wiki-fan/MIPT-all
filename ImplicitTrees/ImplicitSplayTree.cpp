#include "../stdafx.h"
#include "ImplicitSplayTree.h"

CImplicitSplayTree::CNode* CImplicitSplayTree::subSearch( CNode* node, int i )
{
	CNode* ret;
	if( node == 0 ) { // Если нужной вершины не имеется...
		return 0; // возвращаем 0.
	} else { // Иначе ищем дальше.
		push( node );
		int lsize = getC( node->left );
		if( lsize < i && node->right != 0 ) {
			ret = subSearch( node->right, i - lsize - 1 );
		} else if( lsize > i && node->left != 0 ) {
			ret = subSearch( node->left, i );
		} else {
			ret = splay( node ); // Тащим вверх.
		}
	}
	recalcAll( node );
	return ret;
}

int CImplicitSplayTree::findNearestBiggerThan( CNode*& p, int value )
{
	/*push(p);
	if(p->val < value ) {
		return p->left ? findNearestBiggerThan( p->left, value ) : -1;
	} else if (p->right) {
		return getC(p->left) + 1 + findNearestBiggerThan( p->right, value );
	} else {
		return getC(p)-1;
	}*/

	/*push(p);
	if(p->val < value ) {
		return p->left ? findNearestBiggerThan( p->left, value ) :-1;
	} else if (p->right) {
		return getC(p->left) + 1 + findNearestBiggerThan( p->right, value );
	} else {
		return getC(p)+!p->left?0:-1;
	}*/

	CNode* cur = p, * prev = p;
	int previ = 0, curi = 0;

	while( cur != 0 ) {
		push( cur );
		if( cur->val < value ) {
			cur = cur->left;
			if( !cur ) {
				--previ;//curi -= 1 + cur->right ? getC( cur->right ) : 0;
				//previ = curi;
			}
		} else {
			curi += getC( cur->left ) + 1;
			previ = curi;
			prev = cur;
			cur = cur->right;
			if( !cur ) {
				--previ;
			}
		}
	}
	p = splay( prev );
	printf( "\nPREVI %d LEFT %d\n", previ, getC( p->left ));
	massert( previ == getC( p->left ));
	return previ;

	/*bool fl = true;
	while (fl) {
		push( cur );

		if( cur->val >= value ) {
			if( cur->right == 0 ) {
				curi += getC( cur->left ) + 1;
				previ = curi+-1;
				fl = false;
			} else {
				curi += getC( cur->left ) + 1;
				previ = curi-1;
				cur = cur->right;
			}

		} else {
			if( cur->left == 0 ) {
				fl = false;
				//++previ;
			} else {
				cur = cur->left;
				//curi -= getC( cur->left );
			}
		}
	}
	return previ;*/

}


int CImplicitSplayTree::SumSubSegment( int l, int r )
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

void CImplicitSplayTree::Assign( int i, int k )
{
	massert( i <= getC( head ));
	CNode* l, * m, * r;
	split( head, i, l, r );
	split( r, 1, m, r );
	m->val = k;
	recalcAll( m );
	head = merge( merge( l, m ), r );
}

void CImplicitSplayTree::Insert( int i, int k )
{
	massert( i <= getC( head ));
	CNode* l, * r;
	split( head, i, l, r );
	CNode* temp = new CNode( k );
	recalcAll( temp );
	head = merge( merge( l, temp ), r );
	recalcAll( head );
}

bool CImplicitSplayTree::NextPermutation( int l, int r )
{
	massert( l <= r );
	massert( r <= getC( head ));
	printf( "\nbeginsplay\n" );
	//Если надо сделать перестановку на одном элементе или на пустом множестве.
	if( l == r/* || l + 1 == r */) {
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
	// Опускаемся по дереву и ищем наибольшее k, такое что a[j]<a[k]
	mpart = subSearch( mpart, 0 );
	printf( "MPART: " );
	print( mpart );
	//int previ = findNearestBiggerThan( mpart, mpart->val );
	splitByVal(mpart, mpart->val, mpart, rpart);
	//printf("\n%d %d lol\n", previ);
	//split( mpart, previ, mpart, rpart );
	//printf("PREVI: %d LSS: %d", previ, getLSS(mpart));
	split( mpart, 1, lswap, mpart );
	split( rpart, 1, rswap, rpart );
	printf( "\nlpart:" );
	print( lpart );
	printf( "\nlswap:" );
	print( lswap );
	printf( "\nmpart:" );
	print( mpart );
	printf( "\nrswap:" );
	print( rswap );
	printf( "\nrpart:" );
	print( rpart );
	printf( "\n" );

	rpart = merge( mpart, merge( lswap, rpart ));
	reverse( rpart );
	push( rpart ); // TODO: принудительный пуш для дебага.
	mt = merge( lpart, merge( rswap, rpart ));

	head = merge( merge( lt, mt ), rt );
	printf( "\nendsplay\n" );
	return true;
}

void  CImplicitSplayTree::recalcLS( CNode* node )
{
	if( node == 0 ) {
		return;
	}

	bool fl = true;
	// Суффикс.
	if( node->right != 0 ) {
		node->lss = node->right->lss;
		node->last = node->right->last;
		if( getLSS( node->right ) == getC( node->right ) && node->val >= node->right->first ) {
			node->lss += 1;
		} else {
			fl = false;
		}
	} else {
		node->lss = 1;
		node->last = node->val;
	}
	if( node->left && node->left->last >= node->val && fl ) {
		node->lss += node->left->lss;
	}

	// Префикс.
	fl = true;
	if( node->left != 0 ) {
		node->lsp = node->left->lsp;
		node->first = node->left->first;
		if( getLSP( node->left ) == getC( node->left ) && node->val >= node->left->last ) {
			node->lsp += 1;
		} else {
			fl = false;
		}
	} else {
		node->lsp = 1;
		node->first = node->val;
	}

	if( node->right && node->right->first >= node->val && fl ) {
		node->lsp += node->right->lsp;
	}


}

// Поворот p вокруг parent. Сам понимает, левый или правый.
void CImplicitSplayTree::rotate( CNode*& p, CNode*& parent )
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
	/*recalcAll( gparent );
	recalcAll( parent );
	recalcAll( p );*/

}

// Поднимает данную вершину в корень.
CImplicitSplayTree::CNode* CImplicitSplayTree::splay( CNode* p )
{
	if( p == 0 ) {
		return 0;
	}
	// p уже в корне, некуда тянуть.
	if( p->parent == 0 ) {
		return p;
	}
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
CImplicitSplayTree::CNode* CImplicitSplayTree::nonRecursiveSubSearch( CNode* node, int i )
{
	while( node != 0 ) {
		push( node );
		int lsize = getC( node->left );
		if( lsize < i && node->right != 0 ) {
			node = node->right;
			i -= lsize + 1;
		} else if( lsize > i && node->left != 0 ) {
			node = node->left;
		} else {
			return splay( node );
		}
	}
	return 0;
}

// Разбивает дерево с корнем p по ключу key. Вершина с таким ключом будет в правом поддереве.
void CImplicitSplayTree::split( CNode* p, int i, CNode*& l, CNode*& r )
{
	if( p == 0 ) {
		l = r = 0;
		return;
	}
// Отыскиваем нужную вершину и тянем в корень.
	p = subSearch( p, i );
// Нужным образом перенавешиваем вершины.
	int key = getC( p->left );
	if( key < i ) {
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
// Пересчитываем количество детей и сумму.
	recalcAll( l );
	recalcAll( r );
}

// Сливает два дерева, возвращает результат.
CImplicitSplayTree::CNode* CImplicitSplayTree::merge( CNode* l, CNode* r )
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
	// Цепляем l как левого ребёнка r.
	r->left = l;
	l->parent = r;
	// Пересчитываем количество детей и сумму.
	recalcAll( l );
	recalcAll( r );
	return r;
}

void CImplicitSplayTree::recursiveSplit( CNode* p, int i, CNode*& l, CNode*& r )
{
	// Странный рекурсивный split.
	if( p == 0 ) {
		l = r = 0;
		return;
	}
	push( p );
	int lsize = getC( p->left );
	if( lsize < i ) {
		split( p->right, i - lsize - 1, p->right, r );
		l = p;
	} else if( lsize > i ) {
		split( p->left, i, l, p->left );
		r = p;
	}
}