#pragma once
#include "stdafx.h"
#include "Graph.h"

// Базовый класс для обходов графа.
template<typename VT, typename ET, template<typename, typename ... args> typename AT>
class CGraphTraversal {

public:

	// Цвета вершин.
	enum Color {
		White, Gray, Black
	};

	// Обработчик события начала обхода.
	virtual void onBeginWalk()
	{
		std::cout << "Begin" << std::endl;
	}

	// Обработчик события конца обхода.
	virtual void onEndWalk()
	{
		std::cout << "End" << std::endl;
	}

	// Обработчик события покраски вершины в серый.
	virtual void onColorGray( size_t v )
	{
		//std::cout << "gray" << v << std::endl;
	}

	// Обработчик события покраски вершины в чёрный.
	virtual void onColorBlack( size_t v )
	{
		//std::cout << "black" << v << std::endl;
	}

	// Возвращает вектор цветов вершин.
	const std::vector<Color>& getColors() { return colors; }

protected:

	std::vector<Color> colors;
	CGraph<VT, ET, AT>* g;
};

// Обход в глубину.
template<typename VT, typename ET, template<typename, typename ... args> typename AT>
class CDfs : public CGraphTraversal<VT, ET, AT> {

	using CGraphTraversal<VT, ET, AT>::colors;
	using CGraphTraversal<VT, ET, AT>::g;

public:
	using typename CGraphTraversal<VT, ET, AT>::Color;

	CDfs() = default;
	virtual ~CDfs() { }

	// Запуск обхода в глубину графа graph.
	void Dfs( CGraph<VT, ET, AT>& graph );

	using CGraphTraversal<VT, ET, AT>::onBeginWalk;
	using CGraphTraversal<VT, ET, AT>::onEndWalk;
	using CGraphTraversal<VT, ET, AT>::onColorGray;
	using CGraphTraversal<VT, ET, AT>::onColorBlack;

private:
	void dfs( size_t vNum );
};

template<typename VT, typename ET, template<typename, typename ... args> typename AT>
void CDfs<VT, ET, AT>::dfs( size_t vNum )
{
	colors[vNum] = Color::Gray;
	onColorGray( vNum );
	for( auto iter = g->GetOutcomingEdges(vNum).begin(); iter != g->GetOutcomingEdges(vNum).end(); ++iter ) {
		if( colors[iter->vNum] == Color::White ) {
			dfs( iter->vNum );
		}
	}
	colors[vNum] = Color::Black;
	onColorBlack( vNum );
}

template<typename VT, typename ET, template<typename, typename ... args> typename AT>
void CDfs<VT, ET, AT>::Dfs( CGraph<VT, ET, AT>& graph )
{
	g = &graph;
	onBeginWalk();
	colors.resize( g->GetSize(), Color::White );

	for( size_t i = 0; i < g->GetSize(); ++i ) {
		if( colors[i] == Color::White ) {
			dfs( i );
		}
	}
	onEndWalk();
	colors.clear();
}

// Обход в ширину.
template<typename VT, typename ET, template<typename, typename ... args> typename AT>
class CBfs : public CGraphTraversal<VT, ET, AT> {

	using CGraphTraversal<VT, ET, AT>::colors;
	using CGraphTraversal<VT, ET, AT>::g;

public:
	using typename CGraphTraversal<VT, ET, AT>::Color;

	CBfs() = default;
	virtual ~CBfs() { }

	// Запуск обзора в глубину.
	void Bfs( CGraph<VT, ET, AT>& graph );

	using CGraphTraversal<VT, ET, AT>::onBeginWalk;
	using CGraphTraversal<VT, ET, AT>::onEndWalk;
	using CGraphTraversal<VT, ET, AT>::onColorGray;
	using CGraphTraversal<VT, ET, AT>::onColorBlack;

};

template<typename VT, typename ET, template<typename, typename ... args> typename AT>
void CBfs<VT, ET, AT>::Bfs( CGraph<VT, ET, AT>& graph )
{
	g = &graph;
	std::queue<size_t> q;
	onBeginWalk();
	colors.resize( g->GetSize(), Color::White );
	colors[0] = Color::Gray;
	q.push( 0 );
	while( !q.empty()) {
		size_t vNum = q.front();
		q.pop();
		for( auto iter = g->GetOutcomingEdges(vNum).begin(); iter != g->GetOutcomingEdges(vNum).end(); ++iter ) {
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
