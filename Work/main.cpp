#include "stdafx.h"

#include "Sorts.h"
#include "Smth.h"
#include "Vector.h"
#include "Vector.cpp"
#include "Heap.h"
#include "Heap.cpp"
#include "ExternalSort.h"
#include "ChainHashTable.h"
#include "OpenAddressHashTable.h"
#include "Stack.h"
#include "Stack.cpp"
#include "Deque.h"
#include "Deque.cpp"

int main()
{
	COpenAddressHashTable hashTable;
	while( true ) {
		char command = 0;
		std::cin >> command;
		if( std::cin.eof() ) {
			return 0;
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
