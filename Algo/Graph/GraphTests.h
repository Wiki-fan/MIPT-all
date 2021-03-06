#pragma once
#include "../stdafx.h"
#include "Graph.h"
#include "GraphTraversal.h"

// [l, r)
int randBetween( const int l, const int r )
{
	return ( rand() % ( r - l )) + l;
}

size_t randBetween( const size_t l, const size_t r )
{
	return ( rand() % ( r - l )) + l;
}

// Тестировщик для класса графа и его обходов.
template<typename VT, typename ET, template<typename, typename ... args> typename AT>
class CGraphTester {

public:
	CGraphTester() = default;
	~CGraphTester() {
		std::cout << "Deletion started" <<std::endl;
	}

	// Создание рандомного графа с v вершинами и e гранями.
	void GenRandPseudoGraph( size_t v, size_t e )
	{
		g = new CGraph<VT, ET, AT>( v );
		for( int i = 0; i < e; ++i ) {
			g->AddEdge( randBetween( static_cast<size_t>(0), v ),
			            randBetween( static_cast<size_t>(0), v ),
			            randBetween( -10000, 10000 ));
		}
	}

	// Создание рандомного графа с v вершинами и e гранями.
	void GenRandGraph( size_t v)
	{
		std::cout << "Generation started" <<std::endl;
		g = new CGraph<VT, ET, AT>( v );
		for (size_t i = 0; i<v; i += rand()%(v / 1000)) {
			for (size_t j = 0; j<v; j += rand()%(v / 1000)) {
				 if (rand()%10 == 0) {
					 g->AddEdge(i, j, randBetween( -10000, 10000 ));
				 }
			}
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
			/*GenRandGraph( randBetween( static_cast<size_t>(0), v ),
			              randBetween( static_cast<size_t>(0),
			                           (int)(pow( 2, std::max(static_cast<size_t>(62), v * ( v - 1 ) / 2 ))) / 300)
			);*/
			GenRandGraph(v);
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
