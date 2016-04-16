#pragma once
#include "stdafx.h"
#include "Smth.h"

// [l, r)
int randBetween( const int l, const int r )
{
	return ( rand() % ( r - l )) + l;
}

// Граф, хранящийся списком смежности.
// VT - тип контейнера вершин, AT - тип контейнера смежности.
//template<template<typename> typename template<typename> typename AT>
template<typename VT, typename ET, template<typename, typename ... args> typename AT>
class CGraph {
	// Forward declaration.
	struct CVertex;
	struct CEdge;
public:
	// Конструктор по умолчанию - пустой граф.
	CGraph() { }
	//Граф с size вершинами.
	CGraph( size_t size )
	{
		vertices.resize( size );
	}

	// Добавить ребро u -> v.
	void AddEdge( size_t u, size_t v, ET weight )
	{
		vertices[u].out.push_back( CEdge( v, weight ) );
		vertices[v].in.push_back( CEdge( u, weight ) );
	}

	// Добавить вершину со значением val. Возвращает номер добавленной вершины.
	size_t AddVertex( VT val )
	{
		vertices.push_back( CVertex( val ));
		return vertices.size()-1;
	}

	// Получить контейнер рёбер, входящих в вершину.
	AT<CEdge>& GetIncomingEdges(size_t vNum)
	{
		return vertices[vNum].in;
	}

	// Получить контейнер рёбер, выходящих из вершины.
	AT<CEdge>& GetOutcomingEdges(size_t vNum)
	{
		return vertices[vNum].out;
	}

	// Получить количество вершин.
	size_t GetSize() { return vertices.size(); }

private:
	struct CEdge {
		ET val;
		size_t vNum;
		CEdge( size_t _vNum, ET _val ) : vNum( _vNum ), val( _val ) { }
		ET getVal() { return val; }
		size_t getNum() { return vNum; }
	};

	struct CVertex {
		VT val;
		CVertex() : val( 0 ) { }
		explicit CVertex( int _val ) : val( _val ) { }
		~CVertex() { }
		AT<CEdge> out;
		AT<CEdge> in;
		VT getVal() { return val; }
		const AT<CEdge>& getIn() { return in; }
		const AT<CEdge>& getOut() { return out; }
	};

	std::vector<CVertex> vertices;
};

