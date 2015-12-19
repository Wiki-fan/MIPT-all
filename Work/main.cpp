#include "stdafx.h"

#include "AVLTreeTests.h"
#include "SplayTreeTests.h"
//#include "SegmentTreeTests.h"
#include "SparseTableTests.h"

#define MY_CIN_REDIR

int main()
{
	srand( time( 0 ) );
#ifdef MY_CIN_REDIR
	std::ifstream ifs( "in.txt", std::ios_base::in );
	std::streambuf* cinbuf = std::cin.rdbuf();
	std::cin.rdbuf( ifs.rdbuf() );
#endif
	//TestSegmentTree();
	TestSparseTable();	

	//ManualTest();
#ifdef MY_CIN_REDIR
	std::cin.rdbuf( cinbuf );
#endif
	std::cin.get();
	return 0;
}

