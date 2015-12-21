#pragma once
#include "stdafx.h"

const int INF = INT_MAX;

template<typename T>
struct CMin {
	T operator()( T a, T b ) const
	{
		return std::min( a, b );
	}
};

// F задаёт операцию.
template<class F>
class CSegmentTree {
public:
	CSegmentTree( std::vector<int>& a )
	{
		BuildBottomToTop( a );
		//buildTopToBottom( a, 1, 0, a.size() - 1 );
		
	}
	~CSegmentTree() {}

	void UpdateOnRange( int l, int r, int val )
	{
		updateOnRange( 1, 0, t.size() - 1, l, r, val );
	}

	int Query( int l, int r )
	{
		//return queryRecoursive( 1, 0, t.size() - 1, l, r );
		return query( l, r );
	}

private:
	// TODO: Что-то не работает.
	void buildTopToBottom( std::vector<int>& a, int v, int tl, int tr )
	{
		t.resize( 4 * a.size() );
		if( tl == tr ) {
			t[v] = a[tl];
		} else {
			int m = (tl + tr) / 2;
			buildTopToBottom( a, 2 * v, tl, m );
			buildTopToBottom( a, 2 * v + 1, m+1, tr );
			t[v] = F()(t[2 * v], t[2 * v + 1]);
		}
	}

	void BuildBottomToTop( std::vector<int>& a )
	{
		t = a;
		int n = (1 << ((int)log( t.size() - 1 ) + 2));
		t.resize( 4*n, INF );
		// Инициализируем листы
		for( int i = n; i < 2 * n; i++ )
			t[i] = t[i - n];

		// И все остальные вершины
		for( int i = n - 1; i > 0; i-- )
			t[i] = F()( t[2 * i], t[2 * i + 1] );
	}

	int queryRecoursive( int v, int tl, int tr, int l, int r )
	{
		if( l > r ) {
			return 0;
		}
		if( r == tr && l == tl ) {
			return t[v];
		}
		int tm = (tl + tr) / 2;
		return F()(queryRecoursive( v * 2, tl, tm, l, std::min( r, tm ) ),
			queryRecoursive( v * 2 + 1, tm + 1, tr, std::max( l, tm + 1 ), r ));
	}

	int query( int l, int r )
	{
		int ans = INF;
		int n = t.size() / 2;
		l += n - 1, r += n - 1;
		while( l <= r ) {
			// если l - правый сын своего родителя, 
			// учитываем его фундаментальный отрезок
			if( l & 1 )
				ans = std::min( ans, t[l] );
			// если r - левый сын своего родителя, 
			// учитываем его фундаментальный отрезок
			if( !(r & 1) )
				ans = std::min( ans, t[r] );
			// сдвигаем указатели на уровень выше
			l = (l + 1) / 2, r = (r - 1) / 2;
		}
		return ans;
	}

	void update( int i, int& x )
	{
		int n = t.size() / 2;
		i += n - 1;
		t[i] = x;
		while( i /= 2 )
			t[i] = F()(t[2 * i], t[2 * i + 1]);
	}

	void push( int v )
	{
		if( t[v] != -1 ) {
			t[v * 2] = t[v * 2 + 1] = t[v];
			t[v] = -1;
		}
	}

	void updateOnRange( int v, int tl, int tr, int l, int r, int val )
	{
		if( l > r )
			return;
		if( l == tl && tr == r )
			t[v] = val;
		else {
			push( v );
			int tm = (tl + tr) / 2;
			updateOnRange( v * 2, tl, tm, l, std::min( r, tm ), val );
			updateOnRange( v * 2 + 1, tm + 1, tr, std::max( l, tm + 1 ), r, val );
		}
	}

	int get( int v, int tl, int tr, int pos )
	{
		if( tl == tr )
			return t[v];
		push( v );
		int tm = (tl + tr) / 2;
		if( pos <= tm )
			return get( v * 2, tl, tm, pos );
		else
			return get( v * 2 + 1, tm + 1, tr, pos );
	}

	std::vector<int> t;
};
