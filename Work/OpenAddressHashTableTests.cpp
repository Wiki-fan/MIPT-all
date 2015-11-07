#include "stdafx.h"
#include "OpenAddressHashTableTests.h"
#include "OpenAddressHashTable.h"
#include <map>

void AutoTestOpenAddressHashTable()
{
	COpenAddressHashTable hashTable;
	std::map<std::string, int> map;

	while( true ) {
		char command = rand() % 10;
		switch( command ) {
			case 0:case 1:case 2:case 3:case 4:case 5:case 6:case 7:
				command = '+';
				break;
			case 8:
				command = '-';
				break;
			case 9:
				command = '?';
				break;
			default:
				massert( false );
		}
		std::string key(5, '\0');
		for( int i = 0; i < 5; ++i ) {
			key[i] = char( rand() % 26 + 'a' );
		}
		int prevsize;
		std::string res1;
		std::string res2;
		//std::cout << command << key;
		switch( command ) {
			case '?':
				//std::cout << (hashTable.Has( key ) ? "OK" : "FAIL") << std::endl;
				if( (map.count( key ) ? "OK" : "FAIL") != (hashTable.Has( key ) ? "OK" : "FAIL") )
					massert( false );
				break;
			case '+':
				//std::cout << (hashTable.Add( key ) ? "OK" : "FAIL") << std::endl;
				prevsize = map.size();
				map.insert( std::pair<std::string, int>( key, 0 ));
				res1 = (prevsize != map.size() ? "OK" : "FAIL");
				res2 = (hashTable.Add( key ) ? "OK" : "FAIL");
				if( res1 != res2 ) {
					std::cout << res1 << ' ' << res2;
					massert( false );
				}
				break;
			case '-':
				//std::cout << (hashTable.Remove( key ) ? "OK" : "FAIL") << std::endl;
				if( (map.erase( key ) ? "OK" : "FAIL") != (hashTable.Remove( key ) ? "OK" : "FAIL") )
					massert( false );
				break;
			default:
				massert( false );
		}

	}
}

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