#pragma once
#include "stdafx.h"
#include "ITree.h"

// Сплэй-дерево.
class CSplayTree : public ITree<int> {

public:
	CSplayTree() : head( 0 ), fl( 0 ) { }
	~CSplayTree() { delete head; }

	// Вставка элемента с ключом key.
	bool Insert( const int& key ) override;

	// Поиск элемента с ключом key.
	bool Search( const int& key ) const override;

	// Удаление элемента с ключом key.
	bool Remove( const int& key ) override;

protected:
	struct CNode {
		int key;
		CNode* parent;
		CNode* left;
		CNode* right;
		CNode() : key( 0 ), parent( nullptr ), left( nullptr ), right( nullptr )
		{
		}
		explicit CNode( int _key )
				: key( _key ), parent( 0 ), left( 0 ), right( 0 )
		{
		}
		CNode( int _key, CNode* _left, CNode* _right, CNode* _parent )
				: key( _key ), parent( _parent ), left( _left ), right( _right )
		{
		}
		~CNode()
		{
			delete left;
			delete right;
		}
	};

	mutable CNode* head;
	bool fl;

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

	static void split( CNode* p, int key, CNode*& l, CNode*& r, CNode*& m );

	// Сливает два поддерева. Гарантируется, что l->key < r->key.
	static CNode* merge( CNode* l, CNode* r );

};

inline bool CSplayTree::Insert( const int& key )
{
	// Если такой элемент уже есть, не добавляем.
	if( Search( key )) {
		return false;
	}
	//Разбиваем дерево по ключу.
	CNode* l, * r, * m;
	split( head, key, l, r, m );
	delete m;
	// Цепляем получившиеся поддеревья к новой вершине.
	head = new CNode( key, l, r, 0 );
	setParent( l, head );
	setParent( r, head );
	return true;
}

inline bool CSplayTree::Search( const int& key ) const
{
	head = subSearch( head, key );
	return head == 0 ? 0 : ( head->key == key );
}

inline bool CSplayTree::Remove( const int& key )
{
	// Ищем нужный элемент и заодно тянем его в вершину.
	if( !Search( key )) {
		return false;
	}

	// Сливаем его детей.
	setParent( head->left, 0 );
	setParent( head->right, 0 );
	CNode* tmp = head;
	head = merge( head->left, head->right );
	// Удаляем ненужный элемент.
	tmp->left = tmp->right = 0;
	delete tmp;
	return true;
}

// Поворот p вокруг parent. Сам понимает, левый или правый.
inline void CSplayTree::rotate( CNode*& p, CNode*& parent )
{
	// Нужен только для подцепления результата.
	CNode* gparent = parent->parent;
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
}

// Поднимает данную вершину в корень.
inline CSplayTree::CNode* CSplayTree::splay( CNode* p )
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

// Подпрограмма поиска, тянет в вершину и возвращает либо вершину с ключом key, либо ближайшую к ней.
inline CSplayTree::CNode* CSplayTree::subSearch( CNode* node, int key )
{
	if( node == 0 ) { // Если нужной вершины не имеется...
		return 0; // возвращаем 0.
	} else { // Иначе ищем дальше.
		if( node->key < key && node->right != 0 ) {
			return subSearch( node->right, key );
		} else if( node->key > key && node->left != 0 ) {
			return subSearch( node->left, key );
		} else {
			return splay( node ); // Тащим вверх.
		}
	}
}

// Разбивает дерево с корнем p по ключу key. Результаты кладутся в l и r. Серединка - в m.
inline void CSplayTree::split( CNode* p, int key, CNode*& l, CNode*& r, CNode*& m )
{
	if( p == 0 ) {
		l = r = 0;
		return;
	}
	// Отыскиваем нужную вершину.
	p = subSearch( p, key );
	// Нужным образом перенавешиваем вершины.
	if( p->key == key ) {
		setParent( p->left, 0 );
		setParent( p->right, 0 );
		l = p->left;
		r = p->right;
		m = p;
		m->left = m->right = 0;
	} else if( p->key < key ) {
		r = p->right;
		p->right = 0;
		setParent( r, 0 );
		l = p;
		m = 0;
	} else {
		l = p->left;
		p->left = 0;
		setParent( l, 0 );
		r = p;
		m = 0;
	}
}

// Сливает два дерева, возвращает результат.
inline CSplayTree::CNode* CSplayTree::merge( CNode* l, CNode* r )
{
	if( r == 0 ) {
		return l;
	}
	if( l == 0 ) {
		return r;
	}
	r = subSearch( r, l->key ); // Ищем элемент с наименьшим ключом и тянем его в вершину r.
	r->left = l; // Цепляем l как левого ребёнка r.
	l->parent = r; // Выставляем правильного родителя.
	return r;
}
