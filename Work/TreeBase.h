#pragma once
#include "stdafx.h"

template<typename T>
class CTreeBase {

public:
	/*CTreeBase() : head( 0 ) { }
	virtual ~CTreeBase() { delete head; }*/

	virtual void Insert( const T& key ) = 0;

protected:
	/*struct CNodeBase {
		CNodeBase() { }
		CNodeBase( T _key ) : key( _key ) { }
		virtual ~CNodeBase()
		{
			delete left;
			delete right;
		}
		T key;
		CNodeBase* left;
		CNodeBase* right;

	};

	CNodeBase* head;*/

};