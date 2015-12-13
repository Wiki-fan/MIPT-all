#pragma once
#include "stdafx.h"

class CSplayTree {
public:
	CSplayTree() : head( 0 ) { }
	~CSplayTree() { delete head; }
private:
	struct CNode {
		int key;
		CNode* parent;
		CNode* left;
		CNode* right;
		CNode() { }
		explicit CNode( int _key )
				: key( _key ), left( 0 ), right( 0 ), parent( 0 ) { }
		~CNode()
		{
			delete left;
			delete right;
		}
	};

	CNode* head;

	CNode* rightRotate( CNode* k2 )
	{
		CNode* k1 = k2->left;
		k2->left = k1->left;
		k1->right = k2;
		return k1;
	}
	CNode* leftRotate( CNode* k2 )
	{
		CNode* k1 = k2->right;
		k2->right = k1->left;
		k1->left = k2;
		return k1;
	}

	CNode* splay( int key, CNode* root )
	{
		if( !root ) {
			return 0;
		}
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

	CNode* subInsert( int key, CNode* root )
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
		/* This is BST that, all keys <= root->key is in root->left, all keys > 
		   root->key is in root->right. (This BST doesn't allow duplicate keys.) */
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

	CNode* Delete( int key, CNode* root )
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
				/*Note: Since key == root->key, so after Splay(key, root->left), 
				  the tree we get will have no right child tree. (key > any key in 
				  root->left)*/
				root = splay( key, root->left );
				root->right = temp->right;
			}
			delete temp;
			return root;
		}
	}
};