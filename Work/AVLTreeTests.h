#pragma once
#include "AVLTree.h"

void AutoTestAVLTree()
{
	srand( time( NULL ) );
	CAVLTree avlTree;
	std::set<int> set;

	while( true ) {
		char command = rand() % 2;
		switch( command ) {
			case 0://case 3:case 4:case 5:case 6:case 7:case 8:case 9:
				command = '+';
				break;
			case 1:
				command = '-';
				break;
				/*case 2:
				command = '?';
				break;*/
			default:
				massert( false );
		}

		int key = rand();
		std::cout << command << key;
		switch( command ) {
			/*case '?':
			if( set.count( key ) != avlTree.Search( key ) ) {
			massert( false );
			}
			break;*/
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

void Task2()
{
	CAVLTree t;

	/*std::ifstream ifs( "D:\\dev\\code\\didactic-woof\\Work\\in.txt", std::ios_base::in );
	std::streambuf* cinbuf = std::cin.rdbuf();
	std::cin.rdbuf( ifs.rdbuf() );*/
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
	//std::cin.rdbuf( cinbuf );
	//std::cin.get();
	//ifs.close();

}