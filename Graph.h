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
class CGraphTester;

// Граф, хранящийся списком смежности.
// VT - тип контейнера вершин, AT - тип контейнера смежности.
//template<template<typename> typename VT, template<typename> typename AT>
template<typename T, template<typename, typename ... args> typename VT, template<typename, typename ... args> typename AT>
class CGraph {
public:
	CGraph() { }

	CGraph( size_t size )
	{
		vertices.reserve( size );
		for( int i = 0; i < size; ++i ) {
			vertices.push_back( CVertex( i ));
		}
	}

	void AddEdge( int u, int v, int weight )
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
};

template<typename T, template<typename, typename ... args> typename VT, template<typename, typename ... args> typename AT>
class CDfs {
public:
	CDfs() { }
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
	}

	void onBeginWalk()
	{
		std::cout << "Begin" << std::endl;
	}

	void onEndWalk()
	{
		std::cout << "End" << std::endl;
	}

	void onColorGray( size_t v )
	{
		std::cout << "gray" << v << std::endl;
	}

	void onColorBlack( size_t v )
	{
		std::cout << "black" << v << std::endl;
	}

private:
	enum class Color {
		White, Gray, Black
	};
	std::vector<Color> colors;

	//using CGraph::CVertex;
	void dfs( typename CGraph<T, VT, AT>::CVertex& v )
	{
		colors[v.num] = Color::Gray;
		onColorGray( v.num );
		size_t i = 0;
		for( auto iter = v.adjacent.begin(); iter != v.adjacent.end(); ++i, ++iter ) {
			if( colors[( *iter )->num] == Color::White ) {
				dfs( **iter );
			}
		}
		colors[v.num] = Color::Black;
		onColorBlack( v.num );
	}

	CGraph<T, VT, AT>* g;

	friend class CGraphTester<T, VT, AT>;
};

template<typename T, template<typename, typename ... args> typename VT, template<typename, typename ... args> typename AT>
class CGraphTester {
public:
	void GenRandGraph( int v )
	{
		size_t vertices = randBetween( 0, v ), edges = randBetween( 0, v * ( v - 1 ) / 2 );
		g = CGraph<T, VT, AT>( vertices );
		for( int i = 0; i < edges; ++i ) {
			g.AddEdge( randBetween( 0, vertices ), randBetween( 0, vertices ), randBetween( 0, INT_MAX ));
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


	void PerformTests( int n, int v )
	{
		for( int i = 0; i < n; ++i ) {
			GenRandGraph( v );
			TestDFS();
		}
	}
private:
	CGraph<T, VT, AT> g;
};