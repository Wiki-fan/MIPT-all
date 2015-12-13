#include "stdafx.h"
#include "AVLTree.h"
#include "SplayTree.h"

/*void Task1()
{
	CBinarySearchTree<int> bst;
	CCartesianTree<int> ct;

	int N;
	scanf( "%d", &N );
	int x, y;
	for( int i = 0; i < N; ++i ) {
		scanf( "%d %d", &x, &y );
		bst.Insert( x );
		ct.Insert( x, y );
	}
	//std::cout <<bst.getHeight() <<' ' << ct.calcHeight() <<' ';
	std::cout << abs( bst.getHeight() - ct.calcHeight()) << std::endl;
}*/

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
		printf( "%d\n", t.getStatictics( k ));
		t.Print();
		//std::cout << std::endl;
	}
	//std::cin.rdbuf( cinbuf );
	std::cin.get();
	//ifs.close();

}

void Task3()
{
	CSplayTree t;
	CAVLTree avlt;
	char command = 0;
	while( std::cin >> command ) {
		int key;
		std::cin >> key;
		switch( command ) {
			case '?':
				std::cout << (t.Search( key ) ? "OK" : "FAIL") << std::endl;
				break;
			case '+':
				std::cout << (t.Insert( key ) ? "OK" : "FAIL") << std::endl;
				break;
			case '-':
				std::cout << (t.Remove( key ) ? "OK" : "FAIL") << std::endl;
				break;
			default:
				massert( false );
		}
	}
}

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

int main()
{
	//Task2();
	AutoTestAVLTree();
	std::cin.get();
	return 0;
}

