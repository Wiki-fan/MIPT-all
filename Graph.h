#pragma once
#include "stdafx.h"
#include "Smth.h"

// [l, r)
int randBetween( const int l, const int r )
{
	return ( rand() % ( r - l )) + l;
}

template<typename T, template<typename, typename ... args> typename VT, template<typename, typename ... args> typename AT>
class CDfs;

template<typename T, template<typename, typename ... args> typename VT, template<typename, typename ... args> typename AT>
class CBfs;

template<typename T, template<typename, typename ... args> typename VT, template<typename, typename ... args> typename AT>
class CGraphTester;

// Граф, хранящийся списком смежности.
// VT - тип контейнера вершин, AT - тип контейнера смежности.
//template<template<typename> typename VT, template<typename> typename AT>
template<typename T, template<typename, typename ... args> typename VT, template<typename, typename ... args> typename AT>
class CGraph {

public:
	CGraph() = default;
	CGraph( size_t size )
	{
		vertices.reserve( size );
		for( int i = 0; i < size; ++i ) {
			vertices.push_back( CVertex( i ));
		}
	}
	virtual ~CGraph() { }

	void AddEdge( int u, int v )
	{
		vertices[u].adjacent.push_back( &vertices[v] );
		vertices[v].adjacent.push_back( &vertices[u] );
	}

private:
	struct CVertex {
		explicit CVertex( int n ) : num( n ) { }
		~CVertex() { }
		AT<CVertex*> adjacent;
		int num;
	};

	VT<CVertex> vertices;

	friend class CDfs<T, VT, AT>;

	friend class CBfs<T, VT, AT>;
};

template<typename T, template<typename, typename ... args> typename VT, template<typename, typename ... args> typename AT>
class CDfs {

public:
	CDfs() = default;
	virtual ~CDfs() { }

	void Dfs( CGraph<T, VT, AT>& graph )
	{
		g = &graph;
		onBeginWalk();
		colors.resize( g->vertices.size(), Color::White );
		size_t i = 0;
		for( auto iter = g->vertices.begin(); iter != g->vertices.end(); ++i, ++iter ) {
			if( colors[i] == Color::White ) {
				dfs( *iter );
			}
		}
		onEndWalk();
		colors.clear();
	}

	virtual void onBeginWalk()
	{
		std::cout << "Begin DFS" << std::endl;
	}

	virtual void onEndWalk()
	{
		std::cout << "End DFS" << std::endl;
	}

	virtual void onColorGray( size_t v )
	{
		//std::cout << "gray" << v << std::endl;
	}

	virtual void onColorBlack( size_t v )
	{
		//std::cout << "black" << v << std::endl;
	}

private:
	enum class Color {
		White, Gray, Black
	};

	//using CGraph::CVertex;
	void dfs( typename CGraph<T, VT, AT>::CVertex& v )
	{
		colors[v.num] = Color::Gray;
		onColorGray( v.num );
		for( auto iter = v.adjacent.begin(); iter != v.adjacent.end(); ++iter ) {
			if( colors[( *iter )->num] == Color::White ) {
				dfs( **iter );
			}
		}
		colors[v.num] = Color::Black;
		onColorBlack( v.num );
	}

	std::vector<Color> colors;
	CGraph<T, VT, AT>* g;

	friend class CGraphTester<T, VT, AT>;
};

template<typename T, template<typename, typename ... args> typename VT, template<typename, typename ... args> typename AT>
class CBfs {

public:
	CBfs() = default;
	virtual ~CBfs() { }
	void Bfs( CGraph<T, VT, AT>& graph )
	{
		g = &graph;
		std::queue<typename CGraph<T, VT, AT>::CVertex*> q;
		onBeginWalk();
		colors.resize( g->vertices.size(), Color::White );
		colors[g->vertices[0].num] = Color::Gray;
		q.push( &g->vertices[0] );
		while( !q.empty()) {
			typename CGraph<T, VT, AT>::CVertex* v = q.front();
			q.pop();
			for( auto iter = v->adjacent.begin(); iter != v->adjacent.end(); ++iter ) {
				if( colors[( *iter )->num] == Color::White ) {
					colors[( *iter )->num] = Color::Gray;
					onColorGray(( *iter )->num );
					q.push(( *iter ));
				}
			}
			colors[v->num] = Color::Black;
			onColorBlack( v->num );
		}
		onEndWalk();
		colors.clear();
	}

	virtual void onBeginWalk()
	{
		std::cout << "Begin BFS" << std::endl;
	}

	virtual void onEndWalk()
	{
		std::cout << "End BFS" << std::endl;
	}

	virtual void onColorGray( size_t v )
	{
		//std::cout << "gray" << v << std::endl;
	}

	virtual void onColorBlack( size_t v )
	{
		//std::cout << "black" << v << std::endl;
	}

private:
	enum class Color {
		White, Gray, Black
	};

	std::vector<Color> colors;
	CGraph<T, VT, AT>* g;

	friend class CGraphTester<T, VT, AT>;
};

template<typename T, template<typename, typename ... args> typename VT, template<typename, typename ... args> typename AT>
class CGraphTester {

public:
	CGraphTester() = default;
	~CGraphTester() { }

	void GenRandGraph( int v )
	{
		size_t vertices = randBetween( 0, v ), edges = randBetween( 0, v * ( v - 1 ) / 2 ) / 100;
		g = CGraph<T, VT, AT>( vertices );
		for( int i = 0; i < edges; ++i ) {
			g.AddEdge( randBetween( 0, vertices ), randBetween( 0, vertices ));
		}
	}

	void TestDFS()
	{
		CDfs<T, VT, AT> dfs;
		dfs.Dfs( g );
		for( int i = 0; i < dfs.colors.size(); ++i ) {
			massert( dfs.colors[i] == CDfs<T, VT, AT>::Color::Black );
		}
	}

	void TestBFS()
	{
		CBfs<T, VT, AT> bfs;
		bfs.Bfs( g );
		for( int i = 0; i < bfs.colors.size(); ++i ) {
			massert( bfs.colors[i] == CBfs<T, VT, AT>::Color::Black );
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
	CGraph<T, VT, AT> g;
};
