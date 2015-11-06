#pragma once
#include "stdafx.h"
#include "OpenAddressHashTable.h"

void TestOpenAddressHashTable()
{
	COpenAddressHashTable hashTable;
	char command = 0;
	while( std::cin >> command ) {
		if( std::cin.eof() ) {
			return;
		}
		std::string key;
		std::cin >> key;
		switch( command ) {
		case '?':
			std::cout << (hashTable.Has( key ) ? "OK" : "FAIL") << std::endl;
			break;
		case '+':
			std::cout << (hashTable.Add( key ) ? "OK" : "FAIL") << std::endl;
			break;
		case '-':
			std::cout << (hashTable.Remove( key ) ? "OK" : "FAIL") << std::endl;
			break;
		default:
			massert( false );
		}
	}
}