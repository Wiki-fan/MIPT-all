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
	struct CTableNode {

	public:
		std::string *Key;
		bool Deleted;
		explicit CTableNode( const std::string &key ) : Key( new std::string( key )), Deleted( false ) { }
		~CTableNode() { delete Key; }
		void deleteItem() { delete Key; }

	private:
		CTableNode( const CTableNode &other );
	};

	std::vector<CTableNode *> table;

	void rehash();
	int myHash( const std::string &key ) const;
	int myHash2( const std::string &key ) const;
	// Устанавливаем следующую возможную корзину, которую следует проверить.
	int setNextBucket( const std::string &key, int &bucket, int &probe ) const;
};

COpenAddressHashTable::COpenAddressHashTable() : table( initialOpenAddressHashTableSize, (CTableNode*)0 )
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
	if( keysCount >= table.size() * 3 / 4 ) {
		rehash();
	}
	int bucket = myHash( key );
	while( true ) {

		// Проматываем до ближайшей пустой или удалённой корзины.
		for( int probe = 1;
		     bucket < table.size() && table[bucket] != 0 && table[bucket]->Deleted == false;
		     setNextBucket( key, bucket, probe )) {
			if( *( table[bucket]->Key ) == key ) {
				return false; // Уже есть такой элемент, незачем добавлять.
			}
		}

		table[bucket] = new CTableNode( key );
		++keysCount;
		return true;
	}

}

bool COpenAddressHashTable::Remove( const std::string &key )
{
	int bucket = myHash( key );
	// Проматываем все не равные key элементы.
	for( int probe = 1; table[bucket] != 0; setNextBucket( key, bucket, probe )) {
		if( table[bucket]->Deleted == false && *( table[bucket]->Key ) == key ) {
			break;
		}
	}
	// Промотали все возможные и не нашли нужный.
	if( table[bucket] == 0 ) {
		return false;
	}
	// Нашли нужный, удаляем.
	table[bucket]->Deleted = true;
	table[bucket]->deleteItem();
	--keysCount;
	return true;
}

bool COpenAddressHashTable::Has( const std::string &key ) const
{
	int bucket = myHash( key );

	// Ищем элемент, равный нужному. Если доходим до удалённого, ищем дальше, если доходим до пустого, не нашли.
	for( int probe = 1; bucket < table.size() && table[bucket] != 0; setNextBucket( key, bucket, probe )) {
		if( table[bucket]->Deleted == false && *( table[bucket]->Key ) == key ) {
			return true;
		}
	}
	return false;
}

void COpenAddressHashTable::rehash()
{
	// Создаём новую таблицу, в два раза больше.
	std::vector<CTableNode *> oldTable( table.size() * 2 );
	std::swap( table, oldTable );
	keysCount = 0;
	// Добавляем все ключи в новую таблицу.
	for( size_t i = 0; i < oldTable.size(); ++i ) {
		if( oldTable[i] != 0 && oldTable[i]->Deleted == false ) {
			Add( *( oldTable[i]->Key ));
		}
		delete oldTable[i];
	}
}

int COpenAddressHashTable::myHash( const std::string &key ) const
{
	int hash = 0;
	for( mysize i = 0; i < key.length(); ++i ) {
		hash = ( hash * HashParameter + key[i] ) % table.size();
	}
	return hash;
}

int COpenAddressHashTable::myHash2( const std::string &key ) const
{
	int hash = 0;
	for( mysize i = key.length() - 1; i >= 0; --i ) {
		hash = ( hash * HashParameter + key[i] ) % table.size();
	}
	return hash;
}

int COpenAddressHashTable::setNextBucket( const std::string &key, int &bucket, int &probe ) const
{
#ifdef MY_DOUBLE_HASHING
	return ( bucket = ( myHash( key ) + ( probe++ ) * myHash2( key )) % table.size());
#else
	return (bucket = (bucket + probe++) % table.size());
#endif
}