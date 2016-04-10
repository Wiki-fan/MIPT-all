#pragma once
#include "../stdafx.h"
#include "../Graph.h"
#include "../GraphTraversal.h"

template<typename T, typename VT, typename ET, template<typename, typename ... args> typename AT>
class CGraphTester {

public:
	CGraphTester() = default;
	~CGraphTester() { }

	void GenRandGraph( int v )
	{
		size_t vertices = randBetween( 0, v ), edges = randBetween( 0, v * ( v - 1 ) / 2 ) / 100;
		g = CGraph<T, VT, ET, AT>( vertices );
		for( int i = 0; i < edges; ++i ) {
			g.AddEdge( randBetween( 0, vertices ), randBetween( 0, vertices ), randBetween( 0, 10000 ));
		}
	}

	void TestDFS()
	{
		CDfs<T, VT, ET, AT> dfs;
		dfs.Dfs( g );
		for( int i = 0; i < dfs.colors.size(); ++i ) {
			massert( dfs.colors[i] == CDfs<T, VT, ET, AT>::Color::Black );
		}
	}

	void TestBFS()
	{
		CBfs<T, VT, ET, AT> bfs;
		bfs.Bfs( g );
		for( int i = 0; i < bfs.colors.size(); ++i ) {
			massert( bfs.colors[i] == CBfs<T, VT, ET, AT>::Color::Black );
		}
	}

	void PerformTests( int n, int v )
	{
		for( int i = 0; i < n; ++i ) {
			GenRandGraph( v );
			TestDFS();
			TestBFS();
		}
		std::cout << "TESTING COMPLETED" << std::endl;
	}
private:
	CGraph<T, VT, ET, AT> g;
};
