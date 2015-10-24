#include "stdafx.h"
#include "ChainHashTable.h"

// Начальный размер хеш-таблицы. Взаимнопрост с параметром хеш-функции.
const int initialSize = 4;
// Параметр хеш-функции.
const int HashParameter = 91;

CChainHashTable::CChainHashTable() :
	table( initialSize, 0 )
{
	keysCount = 0;
}

CChainHashTable::~CChainHashTable()
{
	for( size_t i = 0; i < table.size(); ++i ) {
		delete table[i];
	}
}

bool CChainHashTable::Has( const std::string& key ) const
{
	const int hash = myHash( key );
	for( CListNode* current = table[hash]; current != 0; current = current->Next ) {
		if( current->Key == key ) {
			return true;
		}
	}
	return false;
}

bool CChainHashTable::Add( const std::string& key )
{
	if( keysCount >= table.size() ) {
		rehash();
	}
	const int hash = myHash( key );
	for( CListNode* current = table[hash]; current != 0; current = current->Next ) {
		if( current->Key == key ) {
			return false;
		} 
	}
	CListNode* newNode = new CListNode( key );
	newNode->Next = table[hash];
	table[hash] = newNode;
	++keysCount;
	return true;
}

bool CChainHashTable::Remove( const std::string& key )
{
	const int hash = myHash( key );
	if( table[hash] == 0 ) {
		return false;
	}
	if( table[hash]->Key == key ) {
		CListNode* nodeToDelete = table[hash];
		table[hash] = nodeToDelete->Next;
		nodeToDelete->Next = 0;
		delete nodeToDelete;
		--keysCount;
		return true;
	}
	CListNode* prev = table[hash];
	for( ; prev->Next != 0 && prev->Next->Key != key; prev = prev->Next ) {};
	if( prev->Next != 0 ) {
		return false;
	}
	CListNode* nodeToDelete = prev->Next;
	prev->Next = nodeToDelete->Next;
	nodeToDelete->Next = 0;
	delete nodeToDelete;
	--keysCount;
	return true;
}

// Полиномиальная хеш-функция, вычисленная методом Горнера.
int CChainHashTable::myHash( const std::string& key ) const
{
	int hash = 0;
	for( size_t i = 0; i < key.length(); ++i ) {
		hash = (hash*HashParameter + key[i]) % table.size();
	}
	return hash;
}

// Перехеширование с удвоением таблицы.
void CChainHashTable::rehash()
{
	std::vector<CListNode*> oldTable( table.size() * 2 );
	std::swap( table, oldTable );
	keysCount = 0;
	for( size_t i = 0; i < oldTable.size(); ++i ) {
		for( CListNode* node = oldTable[i]; node != 0; node = node->Next ) {
			Add( node->Key );
		}
		delete oldTable[i];
	}
}