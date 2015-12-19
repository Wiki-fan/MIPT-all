#include "stdafx.h"

//#include "AVLTreeTests.h"
//#include "SplayTreeTests.h"
//#include "SegmentTreeTests.h"
#include "SparseTableTests.h"

int main()
{
	std::ifstream ifs( "in.txt", std::ios_base::in );
	std::streambuf* cinbuf = std::cin.rdbuf();
	std::cin.rdbuf( ifs.rdbuf() );
	//TestSegmentTree();
	//Task3Old();
	//AutoTestAVLTree();
	TestSparseTable();	

	std::cin.rdbuf( cinbuf );
	std::cin.get();
	return 0;
}

