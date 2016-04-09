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
		vertices.resize( size );
	}

	void AddEdge( int u, int v, int weight )
	{
		vertices[u].adjacent.push_back( v );
		vertices[v].adjacent.push_back( u );
	}

private:
	struct CVertex {
		CVertex() { }
		~CVertex() { }
		AT<T> adjacent;
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
		for( size_t i = 0; i < g->vertices.size(); ++i ) {
			if( colors[i] == Color::White ) {
				dfs( i );
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
	void dfs( size_t vNum )
	{
		colors[vNum] = Color::Gray;
		onColorGray( vNum );
		for( size_t i = 0; i < g->vertices[vNum].adjacent.size(); ++i ) {
			if( colors[g->vertices[vNum].adjacent[i]] == Color::White ) {
				dfs( g->vertices[vNum].adjacent[i] );
			}
		}
		colors[vNum] = Color::Black;
		onColorBlack( vNum );
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