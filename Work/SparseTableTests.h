#pragma once
#include "SparseTable.h"

struct CPair {
	CPair() = default;
	CPair( int _s0, int _s1 ) : s0( _s0 ), s1( _s1 ) {}
	int s0, s1;
};

namespace std {
// Минимум здесь - специально созданная новая пара с минимальными двумя значениями исходных пар.
template<>
const CPair& min<CPair>( const CPair& p1, const CPair& p2 )
{
	if( p1.s0 < p2.s0 ) {
		return CPair( p1.s0, std::min( p1.s1, std::min( p2.s0, p2.s1 ) ) );
	} else if( p1.s0 > p2.s0 ) {
		return CPair( p2.s0, std::min( p1.s0, std::min( p1.s1, p2.s1 ) ) );
	} else { return CPair( p1.s0, p1.s0 ); }
}
}

void TestSparseTable()
{
	int n, m;
	std::vector<CPair> a;
	std::cin >> n >> m;
	for( int i = 0; i < n; ++i ) {
		int x;
		std::cin >> x;
		a.push_back( CPair( x, INT_MAX ) );
	}
	CSparseTable<CPair> st( a );
	for( int i = 0; i < m; ++i ) {
		int l, r;
		std::cin >> l >> r;
		std::cout << st.RMQ( l, r ) << std::endl;
	}
}
