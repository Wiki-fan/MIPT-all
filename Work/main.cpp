#include "stdafx.h"
#include "BinarySearchTree.h"
#include "CartesianTree.h"
#include "AVLTree.h"

void Task1()
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
	std::cout /*<<bst.getHeight() <<' ' << ct.calcHeight() <<' ' */<< abs( bst.getHeight() - ct.calcHeight()) << std::endl;
}

void Task2()
{
	AVLTree t;

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
	}

}

int main()
{
	Task2();
	std::cin.get();
	return 0;
}

