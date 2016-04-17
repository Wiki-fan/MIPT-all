#pragma once
#include "../stdafx.h"
#include "../Graph.h"
#include "../GraphTraversal.h"

// Тестировщик для класса графа и его обходов.
template<typename VT, typename ET, template<typename, typename ... args> typename AT>
class CGraphTester {

public:
	CGraphTester() = default;
	~CGraphTester() { }

	// Создание рандомного графа с v вершинами и e гранями.
	void GenRandGraph( int v, int e )
	{
		g = new CGraph<VT, ET, AT>( v );
		for( int i = 0; i < e; ++i ) {
			g->AddEdge( randBetween( 0, v ), randBetween( 0, v ), randBetween( 0, 10000 ));
		}
	}

	// Протестировать DFS на текущем графе.
	void TestDFS()
	{
		CDfs<VT, ET, AT> dfs;
		dfs.Walk( *g );
		for( int i = 0; i < dfs.getColors().size(); ++i ) {
			massert( dfs.getColors()[i] == CDfs<VT, ET, AT>::Color::Black );
		}
		std::cout << "DFS tested successfully" << std::endl;
	}

	// Протестировать нерекурсивный DFS на текущем графе.
	void TestNonRecursiveDFS()
	{
		CNonRecursiveDfs<VT, ET, AT> dfs;
		dfs.Walk( *g );
		for( int i = 0; i < dfs.getColors().size(); ++i ) {
			massert( dfs.getColors()[i] == CDfs<VT, ET, AT>::Color::Black );
		}
		std::cout << "Non-recursive DFS tested successfully" << std::endl;
	}

	// Протестировать BFS на текущем графе.
	void TestBFS()
	{
		CBfs<VT, ET, AT> bfs;
		bfs.Walk( *g );
		for( int i = 0; i < bfs.getColors().size(); ++i ) {
			massert( bfs.getColors()[i] == CBfs<VT, ET, AT>::Color::Black );
		}
		std::cout << "BFS tested successfully" << std::endl;
	}

	// Протестировать обходы на графе с <=v вершинами n раз.
	void PerformTests( size_t n, size_t v )
	{
		for( int i = 0; i < n; ++i ) {
			GenRandGraph( randBetween( 0, v ), randBetween( 0, pow( 2, v * ( v - 1 ) / 2 )) / 300 );
			TestNonRecursiveDFS();
			TestBFS();
			//TestDFS();
			delete g;
		}
		std::cout << "TESTING COMPLETED" << std::endl;
	}

private:
	CGraph<VT, ET, AT>* g;
};
