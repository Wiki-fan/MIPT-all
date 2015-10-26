#pragma once
#include "stdafx.h"
#include "OpenAddressHashTable.h"

// Начальный размер хеш-таблицы. Взаимнопрост с параметром хеш-функции.
const int initialOpenAddressHashTableSize = 8;
// TODO: выяснить, что такое m
const int m = 32;
// Параметр хеш-функции.
const int HashParameter = 91;

COpenAddressHashTable::COpenAddressHashTable() :table( initialOpenAddressHashTableSize, 0 )
{
}

COpenAddressHashTable::~COpenAddressHashTable()
{
	for( size_t i = 0; i < table.size(); ++i ) {
		delete table[i];
	}
}

bool COpenAddressHashTable::Has( const std::string& key ) const
{
	const int hash = myHash( key );
	int bucket = hash;

	// Ищем элемент, равный нужному. Если доходим до удалённого, ищем дальше, если доходим до пустого, не нашли.
	for( int probe = 1; table[bucket] != 0; bucket = (bucket + probe) % m, ++probe ) {
		if( table[bucket]->Deleted == false && *(table[bucket]->Key) == key ) {
			return true;
		}
	}
	return false;
}

bool COpenAddressHashTable::Add( const std::string& key )
{
	if( keysCount >= table.size() * 3 / 4 ) {
		rehash();
	}
	const int hash = myHash( key );
	int bucket;
	while( true ) {
		bucket = hash;
		// Проматываем до ближайшей пустой или удалённой корзины.
		for( int i = 0, probe = 1;
			bucket < table.size() && table[bucket] != 0 && table[bucket]->Deleted == false;
			bucket = (bucket + probe) % m, ++probe ) 
		{
			if( *(table[bucket]->Key) == key ) {
				return false; // Уже есть такой элемент, незачем добавлять.
			}
		}

		if( bucket == table.size() ) {
			rehash(); // Если вылезает за границы, перехешируем с удвоением размера и повторим.
		} else {
			table[bucket] = new CTableNode( key );
			++keysCount;
			return true;
		}
	}

}

bool COpenAddressHashTable::Remove( const std::string& key )
{
	const int hash = myHash( key );
	if( table[hash] == 0 ) { // Нет элемента с таким хешем в таблице, и никогда не было.
		return false;
	}
	int bucket = hash;
	// Проматываем все не равные key элементы.
	for( int probe = 1; table[bucket] != 0; bucket = (bucket + probe) % m, ++probe ) {
		if( table[bucket]->Deleted == false && *(table[bucket]->Key) == key ) {
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

int COpenAddressHashTable::myHash( const std::string& key ) const
{
	int hash = 0;
	for( size_t i = 0; i < key.length(); ++i ) {
		hash = (hash*HashParameter + key[i]) % table.size();
	}
	return hash;
}

void COpenAddressHashTable::rehash()
{
	// Создаём новую таблицу, в два раза больше.
	std::vector<CTableNode*> oldTable( table.size() * 2 );
	std::swap( table, oldTable );
	keysCount = 0;
	// Добавляем все ключи в новую таблицу.
	for( size_t i = 0; i < oldTable.size(); ++i ) {
		if( oldTable[i] != 0 && oldTable[i]->Deleted == false ) {
			Add( *(oldTable[i]->Key) );
		}
		delete oldTable[i];
	}
}