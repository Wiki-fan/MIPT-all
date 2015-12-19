#pragma once
#include "stdafx.h"
#include "ITree.h"

class CSplayTree : public ITree<int> {

public:
	CSplayTree() : head( 0 ), fl(0) {}
	~CSplayTree() { delete head; }

	// Вставка элемента с ключом key.
	bool Insert( const int& key ) override
	{
		// Если такой элемент уже есть, не добавляем.
		if( !Search( key ) ) {
			return false;
		}
		//Разбиваем дерево по ключу.
		CNode* l, *r;
		split( head, key, l, r );
		// Цепляем получившиеся поддеревья к новой вершине.
		head = new CNode( key, l, r, 0 );
		setParent( l, head );
		setParent( r, head );
		return true;
	}

	// Поиск элемента с ключом key.
	bool Search( const int& key ) const override
	{
		return subSearch( head, key );
	}

	// Удаление элемента с ключом key.
	bool Remove( const int& key ) override
	{
		// Ищем нужный элемент и заодно тянем его в вершину.
		head = subSearch( head, key );
		if( head == 0) {
			return false; // Не нашли нужного.
		}

		// Сливаем его детей.
		setParent( head->left, 0 );
		setParent( head->right, 0 );
		head = Merge( head->left, head->right );
		// Удаляем ненужный элемент.
		CNode* tmp = head;
		tmp->left = tmp->right = 0;
		delete tmp;
		return true;
	}

private:
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

	CNode* head;
	bool fl;

	static CNode* rotateRight( CNode* p )
	{
		CNode* q = p->left;
		p->left = q->right;
		q->right = p;
		return q;
	}

	static CNode* rotateLeft( CNode* q ) // левый поворот вокруг q
	{
		CNode* p = q->right;
		q->right = p->left;
		p->left = q;
		return p;
	}

	static void rotate2( CNode*& p, CNode*& parent )
	{
		CNode* gparent = parent->parent;
		if( gparent != 0 ) {
			if( parent->left == p ) {
				parent = rotateRight( parent );
			} else {
				parent = rotateRight( parent );
			}
		}

	}
	static void rotate( CNode* p, CNode* parent )
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
		// Крутим влево или вправо.
		if( parent->left == p ) {
			p = rotateRight( parent );
		} else {
			p = rotateLeft( parent );
		}

	}

	static CNode* splay( CNode* p )
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
			return parent;
		} else {
			bool zigzig = (gparent->left == parent) == (parent->left == p);
			// zigzig
			if( zigzig ) {
				rotate( parent, gparent );
				rotate( p, parent );
			} else { // zigzag
				rotate( p, parent );
				rotate( parent, gparent );
			}
		}
		return splay( p );
	}

	// Ищет вершину с ключом key (или ближайшую к ней по значению, если вершины с key нет), и тянет её вверх.
	static CNode* subSearch( CNode* node, int key )
	{
		if( node == 0 ) { // Если нужной вершины не имеется.
			return 0;
		} else { // Иначе ищем дальше.
			if( node->key < key && node->right != 0 ) {
				return subSearch( node->right, key );
			} else if( node->key > key && node->left != 0 ) {
				return subSearch( node->left, key );
			} else { // Если найдена нужная.
				return splay( node );
			}
		}
		//return splay( node ); // Тащим вверх.
	}

	static void split( CNode* p, int key, CNode*& l, CNode*& r )
	{
		if( p == 0 ) {
			l = r = 0;
			return;
		}
		p = subSearch( p, key );
		if( p->key == key ) {
			setParent( p->left, 0 );
			setParent( p->right, 0 );
			l = p->left;
			r = p->right;
		} else if( p->key < key ) {
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

	static void setParent( CNode* p, CNode* parent )
	{
		if( p != 0 ) {
			p->parent = parent;
		}
	}

	// Сливает два поддерева. Гарантируется, что l->key < r->key.
	static CNode* Merge( CNode* l, CNode* r )
	{
		if( r == 0 ) {
			return l;
		}
		if( l == 0 ) {
			return r;
		}
		r = subSearch( r, l->key ); // Ищем элемент с наименьшим ключом
		r->left = l;
		l->parent = r;
		return r;
	}

	/*static CNode* splay( int key, CNode* root )
	{
		if( !root ) {
			return 0;
		}
		if ( )
		CNode header;
		CNode* LeftTreeMax = &header;
		CNode* RightTreeMin = &header;

		while( 1 ) {
			if( key < root->key ) {
				if( !root->left ) {
					break;
				}
				if( key < root->left->key ) {
					root = rightRotate( root );
					if( !root->left ) {
						break;
					}
				}
				RightTreeMin->left = root;
				RightTreeMin = RightTreeMin->left;
				root = root->left;
				RightTreeMin->left = 0;
			}
			else if( key > root->key ) {
				if( !root->right ) {
					break;
				}
				if( key > root->right->key ) {
					root = rightRotate( root );
					if( !root->right ) {
						break;
					}
					LeftTreeMax->right = root;
					LeftTreeMax = LeftTreeMax->right;
					root = root->right;
					LeftTreeMax->right = 0;
				}
			} else { break; }
		}
		LeftTreeMax->right = root->left;
		RightTreeMin->left = root->right;
		root->left = header.right;
		root->right = header.left;

		return root;
	}

	static CNode* subInsert( int key, CNode* root )
	{
		static CNode* p_node = NULL;
		if( !p_node ) {
			p_node = new CNode( key );
		} else { // could take advantage of the node remains because of there was duplicate key before.
			p_node->key = key;
		}
		if( !root ) {
			root = p_node;
			p_node = NULL;
			return root;
		}
		root = splay( key, root );
		if( key < root->key ) {
			p_node->left = root->left;
			p_node->right = root;
			root->left = NULL;
			root = p_node;
		}
		else if( key > root->key ) {
			p_node->right = root->right;
			p_node->left = root;
			root->right = NULL;
			root = p_node;
		}
		else {
			return root;
		}
		p_node = NULL;
		return root;
	}

	static CNode* subRemove( int key, CNode* root )
	{
		CNode* temp;
		if( !root ) {
			return NULL;
		}
		root = splay( key, root );
		if( key != root->key ) { // No such node in splay tree
			return root;
		} else {
			if( !root->left ) {
				temp = root;
				root = root->right;
			}
			else {
				temp = root;
				root = splay( key, root->left );
				root->right = temp->right;
			}
			delete temp;
			return root;
		}
	}*/


};