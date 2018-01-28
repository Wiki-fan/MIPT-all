#pragma once
#include "CartesianTree.h"
#include "BinarySearchTree.h"

void TestCartesianTreeDepth()
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
	std::cout << abs( bst.getHeight() - ct.calcHeight() ) << std::endl;
}