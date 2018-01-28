#pragma once
#include "AVLTree.h"
//#define BOOST_TEST_MODULE DequeTest
#include <boost/test/included/unit_test.hpp>

void Task2()
{
	CAVLTree<int> t;

	int N;
	scanf( "%d", &N );
	int a, k;
	for( int i = 0; i < N; ++i ) {
		scanf( "%d %d", &a, &k );
		if( a > 0 ) {
			t.Insert( a );
		} else {
			t.Remove( -a );
		}
		printf( "%d\n", t.getStatictics( k ) );
		//t.Print();
		//std::cout << std::endl;
	}

}

//void AutoTestAVLTree()
BOOST_AUTO_TEST_CASE( AVLTreeAutoTest )
{
	srand( time( NULL ) );
	CAVLTree<int> avlTree;
	std::set<int> set;

	for( int i = 0; i < 10000; ++i ) {
		char command = rand() % 2;
		switch( command ) {
			case 0://case 3:case 4:case 5:case 6:case 7:case 8:case 9:
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
				if( set.count( key ) != avlTree.Search( key ) ) {
					massert( false );
				}
				break;
			case '+':
				if( set.insert( key ).second != avlTree.Insert( key ) ) {
					massert( false );
				}
				break;
			case '-':
				if( set.erase( key ) != avlTree.Remove( key ) ) {
					massert( false );
				}
				break;
			default:
				massert( false );
		}

	}
}
