#include "stdafx.h"
#include "OpenAddressHashTable.h"

// ��������� ������ ���-�������. ������������ � ���������� ���-�������.
const int initialOpenAddressHashTableSize = 8;
const int m = 32;
// �������� ���-�������.
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
	for( ; table[bucket] != 0; ++bucket ) {
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
	int bucket = hash;
	for( int i = 0; bucket < table.size() && table[bucket] != 0 && table[bucket]->Deleted == false; ++bucket ) { // ����������� �� ��������� ������ ��� �������� �������.
																 //TODO:������������ ������������.
	}
	if( bucket == table.size() )
		rehash(); // WAT? O_o ���� �������� �� �������.
	table[bucket] = new CTableNode( key );
	++keysCount;
	return true;
}

bool COpenAddressHashTable::Remove( const std::string& key )
{
	const int hash = myHash( key );
	if( table[hash] == 0 ) { // ��� �������� � ����� ����� � �������, � ������� �� ����.
		return false;
	}
	int bucket = hash;
	for( ; table[bucket] != 0; ++bucket ) {
		if( table[bucket]->Deleted == false && *(table[bucket]->Key) == key ) {
			break;
		}
	}
	if( table[bucket] == 0 ) { // ��������� ��� ��������� � �� ����� ������.
		return false;
	}
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
	std::vector<CTableNode*> oldTable( table.size() * 2 );
	std::swap( table, oldTable );
	keysCount = 0;
	for( size_t i = 0; i < oldTable.size(); ++i ) {
		if( oldTable[i] != 0 && oldTable[i]->Deleted == false ) {
			Add( *(oldTable[i]->Key) );
		}
		delete oldTable[i];
	}
}