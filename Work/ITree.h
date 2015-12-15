#pragma once
#include "stdafx.h"

template<typename T>
class ITree {

public:
	ITree() { }
	virtual ~ITree() {}

	// Вставка элемента с ключом key.
	virtual bool Insert( const T& key ) = 0;
	// Поиск элемента с ключом key.
	virtual bool Search( const T& key ) const = 0;
	// Удаление элемента с ключом key.
	virtual bool Remove( const T& key ) = 0;

};

