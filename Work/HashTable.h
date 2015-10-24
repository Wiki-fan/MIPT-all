#pragma once
#include "stdafx.h"

class CHashTable
{
public:
	CHashTable() : keysCount( 0 ) {}
	virtual ~CHashTable() {}

	// Проверка наличия ключа.
	virtual bool Has( const std::string& key ) const = 0;
	// Добавление ключа. 
	// Если число элементов становится больше размера таблицы, то таблица расширяется.
	virtual bool Add( const std::string& key ) = 0;
	// Удаление ключа. Уменьшение таблицы не заложено.
	// Уменьшение таблицы не заложено.
	virtual bool Remove( const std::string& key ) = 0;

protected:

	size_t keysCount;
	virtual int myHash( const std::string& key ) const = 0;
	virtual void rehash() = 0;
};