#pragma once
#include "SplayTree.h"
#include "AVLTree.h"

void Task3()
{
	CSplayTree SplayTree;
	CAVLTree AvlTree;
	std::set<int> StdSet;

	while( true ) {
		char command = rand() % 2;
		switch( command ) {
			case 0:
				command = '+';
				break;
			case 1:
				command = '-';
				break;
			case 2:
				command = '?';
				break;
			default:
				massert( false );
		}

		int key = rand();
		std::cout << command << key;
		switch( command ) {
			case '?':
				if( StdSet.count( key ) != AvlTree.Search( key ) || StdSet.count(key) != SplayTree.Search( key )) {
					massert( false );
				}
				break;
			case '+':
				if( StdSet.insert( key ).second != AvlTree.Insert( key ) ||  StdSet.insert( key ).second != SplayTree.Insert( key ) ) {
					massert( false );
				}
				break;
			case '-':
				if( StdSet.erase( key ) != AvlTree.Remove( key ) || StdSet.erase( key ) != SplayTree.Remove( key ) ) {
					massert( false );
				}
				break;
			default:
				massert( false );
		}

	}
}