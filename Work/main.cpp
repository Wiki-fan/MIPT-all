#include "stdafx.h"
#include "AVLTreeTests.h"

//#include "SplayTree.h"

/*void Task3()
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
}*/
#include "SparseTable.h"
void TestSparseTable()
{
	std::ifstream ifs( "in.txt", std::ios_base::in );
	std::streambuf* cinbuf = std::cin.rdbuf();
	std::cin.rdbuf( ifs.rdbuf() );
	int n, m;
	std::vector<int> a;
	std::cin >> n >> m;
	for( int i = 0; i < n; ++i ) {
		int x;
		std::cin >> x;
		a.push_back( x );
	}
	CSparseTable st( a );
	for( int i = 0; i < m; ++i ) {
		int l, r;
		std::cin >> l >> r;
		std::cout << st.RMQ( l, r );
	}
	std::cin.rdbuf( cinbuf );
}

int main()
{
	//Task3();
	//AutoTestAVLTree();
	TestSparseTable();
	std::cin.get();
	return 0;
}

