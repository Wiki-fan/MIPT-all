#pragma once
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
	struct CNode {

	public:
		explicit CNode( const std::string &key ) : Key( new std::string( key ) )
		{
		}

		~CNode()
		{
			deleteItem();
		}

		void deleteItem() const
		{
			if( Key != 0 ) {
				delete Key;
			}
		}

		std::string *Key;

	private:
		CNode( const CNode &other );
	};

	static CNode *deleted;

	void rehash();
	int myHash( const std::string &key ) const;
	int myHash2( const std::string &key ) const;
	// Устанавливаем следующую возможную корзину, которую следует проверить.
	int setNextBucket( int hash1, int hash2, int &probe ) const;

	std::vector<CNode *> table;
};

COpenAddressHashTable::COpenAddressHashTable() : table( initialOpenAddressHashTableSize, static_cast<CNode*>(0) )
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

	// Проматываем до ближайшей пустой или удалённой корзины.
//HELL:
	int bucket = hash1;
	CNode **toWrite = 0;
	for( int probe = 1;
	table[bucket] != 0 /*&& bucket != hash1*/; // Пока не пройдём по всем и дойдём до пустой /*или пока не достигнем начальной.*/
		bucket = setNextBucket( hash1, hash2, probe ) ) {
		if( probe > table.size() ) {
			rehash();
			break;
		} // HERE
		if( *(table[bucket]->Key) == key ) {
			return false; // Уже есть такой элемент, незачем добавлять.
		}
		// Мы можем писать вместо удалённой, но должны домотать до конца, чтобы проверить, что такого элемента действительно нет.
		if( toWrite == 0 && table[bucket] == deleted ) {
			toWrite = &(table[bucket]);
		}
	}


	if( toWrite == 0 ) {
		++keysCount; // Должны инкрементировать, только если пишем вместо 0, не deleted.
		toWrite = &(table[bucket]);
	}

	*toWrite = new CNode( key );

	return true;
}

bool COpenAddressHashTable::Remove( const std::string &key )
{
	int hash1 = myHash( key );
	int hash2 = myHash2( key );
	int bucket = hash1;
	// Проматываем все не равные key элементы.
	for( int probe = 1; table[bucket] != 0; bucket = setNextBucket( hash1, hash2, probe ) ) {
		if( probe > table.size() )
			return false; // HERE
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
		if( probe > table.size() )
			return false; // HERE
		if( table[bucket] != deleted && *(table[bucket]->Key) == key ) {
			return true;
		}
	}
	return false;
}

void COpenAddressHashTable::rehash()
{
	// Создаём новую таблицу, в два раза больше.
	std::vector<CNode *> oldTable( table.size() * 2, static_cast<CNode*>(0) );
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
	mysize hash = 0;
	for( mysize i = 0; i < key.length(); ++i ) {
		hash = (hash * HashParameter1 + key[i]) % table.size();
	}
	return hash;
}

int COpenAddressHashTable::myHash2( const std::string &key ) const
{
	return (myHash( key ) * 2 + 1) % table.size();
	/*mysize hash = 0;
	for( mysize i = 0; i < key.length(); ++i ) {
		hash = 2*(hash * HashParameter2 + key[i]) % (table.size()-1)+1;
	}
	return hash;*/
}

int COpenAddressHashTable::setNextBucket( int hash1, int hash2, int &probe ) const
{
#ifdef MY_DOUBLE_HASHING
	return abs( mysize( (hash1 + (probe++) * hash2) % table.size() ) );
#else
	massert( false, "Fix me" );
	//return (bucket = (bucket + probe++) % table.size());
#endif
}

COpenAddressHashTable::CNode *COpenAddressHashTable::deleted = new CNode( "" );

