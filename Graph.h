#pragma once
#include "stdafx.h"
#include "Smth.h"

// [l, r)
int randBetween( const int l, const int r )
{
	return ( rand() % ( r - l )) + l;
}

template<typename T, typename VT, typename ET, template<typename, typename ... args> typename AT>
class CDfs;

template<typename T, typename VT, typename ET, template<typename, typename ... args> typename AT>
class CBfs;

template<typename T, typename VT, typename ET, template<typename, typename ... args> typename AT>
class CGraphTester;

// Граф, хранящийся списком смежности.
// VT - тип контейнера вершин, AT - тип контейнера смежности.
//template<template<typename> typename template<typename> typename AT>
template<typename T, typename VT, typename ET, template<typename, typename ... args> typename AT>
class CGraph {

	friend class CDfs<T, VT, ET, AT>;

	friend class CBfs<T, VT, ET, AT>;

public:
	CGraph() { }
	CGraph( size_t size )
	{
		vertices.resize( size );
	}

	// u -> v
	void AddEdge( T u, T v, ET weight )
	{
		vertices[u].adjacent.push_back( CEdge( v, weight ));
		// vertices[v].adjacent.push_back( u );
	}

	void AddVertex( VT val )
	{
		vertices.push_back( CVertex( val ));
	}

private:
	struct CEdge {
		ET val;
		T vNum;
		CEdge( T _vNum, ET _val ) : vNum( _vNum ), val( _val ) { }
	};

	struct CVertex {
		VT val;
		CVertex() : val( 0 ) { }
		explicit CVertex( int _val ) : val( _val ) { }
		~CVertex() { }
		AT<CEdge> adjacent;
	};

	std::vector<CVertex> vertices;
};

