#pragma once
#include "../stdafx.h"
#include "Graph.h"

// Базовый класс для обходов графа.
template<typename VT, typename ET, template<typename, typename ... args> typename AT>
class CGraphTraversal {

public:
	CGraphTraversal() { }
	virtual ~CGraphTraversal() { }

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

	// Запуск обхода графа graph.
	virtual void Walk( CGraph<VT, ET, AT>& graph )
	{
		g = &graph;
		colors.resize( g->GetSize(), Color::White );
		onBeginWalk();
		for( size_t i = 0; i < g->GetSize(); ++i ) {
			if( colors[i] == Color::White ) {
				walk( i );
			}
		}
		onEndWalk();
		colors.clear();
	}
protected:

	std::vector<Color> colors;
	CGraph<VT, ET, AT>* g;

	virtual void walk( size_t start ) = 0;
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

	using CGraphTraversal<VT, ET, AT>::Walk;
	using CGraphTraversal<VT, ET, AT>::onBeginWalk;
	using CGraphTraversal<VT, ET, AT>::onEndWalk;
	using CGraphTraversal<VT, ET, AT>::onColorGray;
	using CGraphTraversal<VT, ET, AT>::onColorBlack;

private:
	void walk( size_t vNum );
};

template<typename VT, typename ET, template<typename, typename ... args> typename AT>
void CDfs<VT, ET, AT>::walk( size_t vNum )
{
	colors[vNum] = Color::Gray;
	onColorGray( vNum );
	for( auto iter = g->GetOutcomingEdges( vNum ).begin(); iter != g->GetOutcomingEdges( vNum ).end(); ++iter ) {
		if( colors[iter->vNum] == Color::White ) {
			walk( iter->vNum );
		}
	}
	colors[vNum] = Color::Black;
	onColorBlack( vNum );
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

	using CGraphTraversal<VT, ET, AT>::Walk;
	using CGraphTraversal<VT, ET, AT>::onBeginWalk;
	using CGraphTraversal<VT, ET, AT>::onEndWalk;
	using CGraphTraversal<VT, ET, AT>::onColorGray;
	using CGraphTraversal<VT, ET, AT>::onColorBlack;

private:
	void walk( size_t start );
};

template<typename VT, typename ET, template<typename, typename ... args> typename AT>
void CBfs<VT, ET, AT>::walk( size_t start )
{
	std::queue<size_t> q;

	colors[0] = Color::Gray;
	q.push( 0 );
	while( !q.empty()) {
		size_t vNum = q.front();
		q.pop();
		for( auto iter = g->GetOutcomingEdges( vNum ).begin(); iter != g->GetOutcomingEdges( vNum ).end(); ++iter ) {
			if( colors[iter->vNum] == Color::White ) {
				colors[iter->vNum] = Color::Gray;
				onColorGray( iter->vNum );
				q.push( iter->vNum );
			}
		}
		colors[vNum] = Color::Black;
		onColorBlack( vNum );
	}
}

// Нерекурсивный обход в глубину.
template<typename VT, typename ET, template<typename, typename ... args> typename AT>
class CNonRecursiveDfs : public CGraphTraversal<VT, ET, AT> {

	using CGraphTraversal<VT, ET, AT>::colors;
	using CGraphTraversal<VT, ET, AT>::g;

public:
	using typename CGraphTraversal<VT, ET, AT>::Color;

	CNonRecursiveDfs() = default;
	virtual ~CNonRecursiveDfs() { }

	using CGraphTraversal<VT, ET, AT>::Walk;
	using CGraphTraversal<VT, ET, AT>::onBeginWalk;
	using CGraphTraversal<VT, ET, AT>::onEndWalk;
	using CGraphTraversal<VT, ET, AT>::onColorGray;
	using CGraphTraversal<VT, ET, AT>::onColorBlack;

private:
	void walk( size_t start );
};

template<typename VT, typename ET, template<typename, typename ... args> typename AT>
void CNonRecursiveDfs<VT, ET, AT>::walk( size_t start )
{
	std::stack<size_t> st;
	colors[start] = Color::Gray;
	st.push( start );
	while( !st.empty()) {
		size_t vNum = st.top();
		bool fl = true;
		for( auto iter = g->GetOutcomingEdges( vNum ).begin(); iter != g->GetOutcomingEdges( vNum ).end(); ++iter ) {
			if( colors[iter->vNum] == Color::White ) {
				colors[iter->vNum] = Color::Gray;
				onColorGray( iter->vNum );
				st.push( iter->vNum );
				fl = false;
				break;
			}
		}
		if( fl ) {
			colors[vNum] = Color::Black;
			onColorBlack( vNum );
			st.pop();
		}
	}
}