#pragma once
#include "stdafx.h"
#include "Graph.h"

template<typename T, typename VT, typename ET, template<typename, typename ... args> typename AT>
class CGraphTraversal {

	friend class CGraphTester<T, VT, ET, AT>;

public:
	virtual void onBeginWalk()
	{
		std::cout << "Begin" << std::endl;
	}

	virtual void onEndWalk()
	{
		std::cout << "End" << std::endl;
	}

	virtual void onColorGray( T v )
	{
		std::cout << "gray" << v << std::endl;
	}

	virtual void onColorBlack( T v )
	{
		std::cout << "black" << v << std::endl;
	}

protected:

	enum Color {
		White, Gray, Black
	};
	std::vector<Color> colors;
	CGraph<T, VT, ET, AT>* g;
};

template<typename T, typename VT, typename ET, template<typename, typename ... args> typename AT>
class CDfs : public CGraphTraversal<T, VT, ET, AT> {

	using CGraphTraversal<T, VT, ET, AT>::colors;
	using CGraphTraversal<T, VT, ET, AT>::g;
	using typename CGraphTraversal<T, VT, ET, AT>::Color;

	friend class CGraphTester<T, VT, ET, AT>;

public:
	CDfs() = default;
	virtual ~CDfs() { }

	void Dfs( CGraph<T, VT, ET, AT>& graph )
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
		colors.clear();
	}

private:

	//using CGraph::CVertex;
	void dfs( T vNum )
	{
		colors[vNum] = Color::Gray;
		onColorGray( vNum );
		for( auto iter = g->vertices[vNum].adjacent.begin(); iter != g->vertices[vNum].adjacent.end(); ++iter ) {
			if( colors[iter->vNum] == Color::White ) {
				dfs( iter->vNum );
			}
		}
		colors[vNum] = Color::Black;
		onColorBlack( vNum );
	}

	using CGraphTraversal<T, VT, ET, AT>::onBeginWalk;
	using CGraphTraversal<T, VT, ET, AT>::onEndWalk;
	using CGraphTraversal<T, VT, ET, AT>::onColorGray;
	using CGraphTraversal<T, VT, ET, AT>::onColorBlack;

private:

};

template<typename T, typename VT, typename ET, template<typename, typename ... args> typename AT>
class CBfs : public CGraphTraversal<T, VT, ET, AT> {

	using CGraphTraversal<T, VT, ET, AT>::colors;
	using CGraphTraversal<T, VT, ET, AT>::g;
	using typename CGraphTraversal<T, VT, ET, AT>::Color;

	friend class CGraphTester<T, VT, ET, AT>;

public:
	CBfs() = default;
	virtual ~CBfs() { }

	void Bfs( CGraph<T, VT, ET, AT>& graph )
	{
		g = &graph;
		std::queue<T> q;
		onBeginWalk();
		colors.resize( g->vertices.size(), Color::White );
		colors[0] = Color::Gray;
		q.push( 0 );
		while( !q.empty()) {
			T vNum = q.front();
			q.pop();
			for( auto iter = g->vertices[vNum].adjacent.begin(); iter != g->vertices[vNum].adjacent.end(); ++iter ) {
				if( colors[iter->vNum] == Color::White ) {
					colors[iter->vNum] = Color::Gray;
					onColorGray( iter->vNum );
					q.push( iter->vNum );
				}
			}
			colors[vNum] = Color::Black;
			onColorBlack( vNum );
		}
		onEndWalk();
		colors.clear();
	}

	using CGraphTraversal<T, VT, ET, AT>::onBeginWalk;
	using CGraphTraversal<T, VT, ET, AT>::onEndWalk;
	using CGraphTraversal<T, VT, ET, AT>::onColorGray;
	using CGraphTraversal<T, VT, ET, AT>::onColorBlack;

private:

};