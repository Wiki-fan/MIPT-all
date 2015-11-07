﻿#pragma once
#include "HashTable.h"

// Начальный размер хеш-таблицы. Взаимнопрост с параметром хеш-функции.
const int initialOpenAddressHashTableSize = 8;

class COpenAddressHashTable : public CHashTable {

public:
	COpenAddressHashTable();
	~COpenAddressHashTable();

	bool Add( const std::string &key );
	bool Remove( const std::string &key );
	bool Has( const std::string &key ) const;

private:
	struct CTableNode {

	public:
		explicit CTableNode( const std::string &key ) : Key( new std::string( key ) ) {}
		~CTableNode() { delete Key; }
		void deleteItem() const { delete Key; }

		std::string *Key;

	private:
		CTableNode( const CTableNode &other );
	};

	static CTableNode* deleted;

	void rehash();
	int myHash( const std::string &key ) const;
	int myHash2( const std::string &key ) const;
	// Устанавливаем следующую возможную корзину, которую следует проверить.
	int setNextBucket( int hash1, int hash2, int & probe ) const;

	std::vector<CTableNode *> table;
};

COpenAddressHashTable::COpenAddressHashTable() : table( initialOpenAddressHashTableSize, static_cast<CTableNode*>(0) )
{
}

COpenAddressHashTable::~COpenAddressHashTable()
{
	for( size_t i = 0; i < table.size(); ++i ) {
		delete table[i];
	}
}

bool COpenAddressHashTable::Add( const std::string &key )
{
	if( 4 * keysCount >= table.size() * 3 ) {
		rehash();
	}
	int hash1 = myHash( key );
	int hash2 = myHash2( key );
	int bucket = hash1;
	while( true ) {
		// Проматываем до ближайшей пустой или удалённой корзины.
		for( int probe = 1;
			table[bucket] != 0;
			bucket = setNextBucket( hash1, hash2, probe ) ) {
			/*if( probe > table.size() ) {
				rehash();
				break;
			} */// Не уверен, что это очень надо, но пусть.
			if( *(table[bucket]->Key) == key ) {
				return false; // Уже есть такой элемент, незачем добавлять.
			}
		}

		++keysCount;
		table[bucket] = new CTableNode( key );
		
		return true;
	}
}

bool COpenAddressHashTable::Remove( const std::string &key )
{
	int hash1 = myHash( key );
	int hash2 = myHash2( key );
	int bucket = hash1;
	// Проматываем все не равные key элементы.
	for( int probe = 1; table[bucket] != 0; bucket = setNextBucket( hash1, hash2, probe ) ) {
		//if( probe > table.size() )
			//return false;
		if( table[bucket] != deleted && *(table[bucket]->Key) == key ) {
			break;
		}
	}

	// Промотали все возможные и не нашли нужный.
	if( table[bucket] == 0 ) {
		return false;
	}
	// Нашли нужный, удаляем.
	table[bucket]->deleteItem();
	table[bucket] = deleted;
	//--keysCount; Для перехеширования не уменьшаем.
	return true;
}

bool COpenAddressHashTable::Has( const std::string &key ) const
{
	int hash1 = myHash( key );
	int hash2 = myHash2( key );
	int bucket = hash1;

	// Ищем элемент, равный нужному. Если доходим до удалённого, ищем дальше, если доходим до пустого, не нашли.
	for( int probe = 1; table[bucket] != 0; bucket = setNextBucket( hash1, hash2, probe ) ) {
		//if( probe > table.size() ) {
		//}
		if( table[bucket] != deleted && *(table[bucket]->Key) == key ) {
			return true;
		}
	}
	return false;
}

void COpenAddressHashTable::rehash()
{
	// Создаём новую таблицу, в два раза больше.
	std::vector<CTableNode *> oldTable( table.size() * 2, static_cast<CTableNode*>(0) );
	std::swap( table, oldTable );
	keysCount = 0;
	// Добавляем все ключи в новую таблицу.
	for( size_t i = 0; i < oldTable.size(); ++i ) {
		if( oldTable[i] != 0 && oldTable[i] != deleted ) {
			Add( *(oldTable[i]->Key) );
			delete oldTable[i];
		}
	}
}

int COpenAddressHashTable::myHash( const std::string &key ) const
{
	int hash = 0;
	for( mysize i = 0; i < key.length(); ++i ) {
		hash = (hash * HashParameter1 + key[i]) % table.size();
	}
	return hash;
}

int COpenAddressHashTable::myHash2( const std::string &key ) const
{
	return (myHash( key ) * 2 + 1) % table.size();
}

int COpenAddressHashTable::setNextBucket( int hash1, int hash2, int & probe ) const
{
#ifdef MY_DOUBLE_HASHING
	return (hash1 + (probe++) * hash2) % table.size();
#else
	massert( false, "Fix me" );
	//return (bucket = (bucket + probe++) % table.size());
#endif
}

COpenAddressHashTable::CTableNode* COpenAddressHashTable::deleted = new CTableNode( "" );
