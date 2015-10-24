#pragma once
#include "stdafx.h"
#include "HashTable.h"

// Хэш-таблица, разрешающая коллизии методом цепочек.
class CChainHashTable : public CHashTable
{
public:
	CChainHashTable();
	~CChainHashTable();
	// Проверка наличия ключа.
	virtual bool Has( const std::string& key ) const;
	// Добавление ключа. 
	// Если число элементов становится больше размера таблицы, то таблица расширяется.
	virtual bool Add( const std::string& key );
	// Удаление ключа. Уменьшение таблицы не заложено.
	// Уменьшение таблицы не заложено.
	virtual bool Remove( const std::string& key );
private:
	// Элемент цепочки.
	struct CListNode
	{
		std::string Key;
		CListNode* Next;
		CListNode( const std::string& key ) : Key( key ), Next( 0 ) {}
		~CListNode() { delete Next; }
	};
	std::vector<CListNode*> table;
	virtual int myHash( const std::string& key ) const;
	virtual void rehash();
};