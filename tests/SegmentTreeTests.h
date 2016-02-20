#pragma once
#include "../SegmentTree.h"

void TestSegmentTree()
{
	int n;
	std::cin >> n;
	int c, d, r, g, b, e, f;
	std::vector<int> a;
	for( int i = 0; i < n; ++i ) {
		std::cin >> r >> g >> b;
		a.push_back( r + g + b );
	}
	CSegmentTree<CMin<int>> st( a );
	int k;
	std::cin >> k;
	for( int i = 0; i < k; ++i ) {
		std::cin >> c >> d >> r >> g >> b >> e >> f;
		st.UpdateOnRange( c, d, r + g + b );
		std::cout << st.Query( e, f );

	}

}