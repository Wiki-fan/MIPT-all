#pragma once
#include "../SparseTable.h"

void TestSparseTable()
{
	int n, m;
	std::vector<CPair> a;
	std::cin >> n >> m;
	for( int i = 0; i < n; ++i ) {
		int x;
		std::cin >> x;
		a.push_back( CPair( x, INT_MAX, i, i ) );
	}
	CSparseTable<CPair> st( a );
	for( int i = 0; i < m; ++i ) {
		int l, r;
		std::cin >> l >> r;
		std::cout << st.RMQ( l, r ) << std::endl;
	}
}
