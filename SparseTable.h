#pragma once
#include "stdafx.h"
struct CPair {
	CPair() = default;
	CPair( int _s0, int _s1, int _i0, int _i1 ) : s0( _s0 ), s1( _s1 ), i0( _i0 ), i1( _i1 ) {}
	int s0, s1;
	int i0, i1;
	int request() const
	{
		return s1;
	}
};

// Минимум здесь - специально созданная новая пара с минимальными двумя значениями исходных пар.
const CPair& myMin( const CPair& p1, const CPair& p2 )
{
	/*if( p1.s0 < p2.s0 ) {
		return CPair( p1.s0, std::min( p1.s1, std::min( p2.s0, p2.s1 ) ) );
	} else if( p1.s0 > p2.s0 ) {
		return CPair( p2.s0, std::min( p1.s0, std::min( p1.s1, p2.s1 ) ) );
	} else {
		if( p1.s1 < p2.s1 ) {
			return CPair( p1.s0, p1.s1 );
		} else if( p1.s1>p2.s1 ) {
			return CPair( p1.s0, p2.s1 );
		} else {
			return CPair( p1.s0, p1.s1 );
		}
	}*/
	if( p1.i0 != p2.i0 && p1.i1 != p2.i1 && p1.i0 != p2.i1 && p2.i0 != p1.i1) {
		if( p1.s0 < p2.s0 ) {
			return CPair( p1.s0, std::min( p1.s1, std::min( p2.s0, p2.s1 ) ), p1.i0, p1.i0 );
		} else if( p1.s0 > p2.s0 ) {
			return CPair( p2.s0, std::min( p2.s1, std::min(p1.s0, p1.s1  ) ), p2.i0, p2.i0 );
		} else {
			return CPair( p1.s0, p1.s0, p1.i0, p1.i1 );
		}
	} else {
		return CPair( p1.s0, std::min( p1.s1, p2.s1 ), p1.i0, p1.i0 );
	}
}

template<typename T>
class CMatrix {
public:
	CMatrix( int _w, int _h ) :w( _w ), h( _h )
	{
		m = new T*[h];
		for( int i = 0; i < h; ++i ) {
			m[i] = new T[w];
		}
	}

	~CMatrix()
	{
		for( int i = 0; i < h; ++i ) {
			delete[] m[i];
		}
		delete[] m;
	}

	T& operator()( int a, int j )
	{
		return m[a][j];
	}

	int getHeight() const { return h; }
	int getWidth() const { return w; }

private:
	T** m; // Первый индекс - строка, второй - столбец.
	int w; // Количество столбцов.
	int h; // Количество строк.
};

template<class T>
class CSparseTable {
public:
	explicit CSparseTable( std::vector<T>& a ) :m( a.size(), log2( a.size() ) + 1 )
	{
		// Первая строка - исходный массив.
		for( int i = 0; i < a.size(); ++i ) {
			m( 0, i ) = a[i];
		}
		// Заполняем остальные.
		for( int i = 1; i < m.getHeight(); ++i ) {
			for( int j = 0; j < m.getWidth(); ++j ) {
				m( i, j ) = myMin( m( i - 1, j ), m( i - 1, j + pow( 2, i - 1 ) ) );
			}
		}
		// Предпросчитываем логарифмы.
		for( int i = 1; i < m.getWidth() + 2; ++i ) {
			logs.push_back( log2( i ) );
		}
	}

	~CSparseTable() {}

	int RMQ( int l, int r )
	{
		r -= 1; l -= 1;
		int k = logs[r - l + 1]; // Логарифм расстояния между краями.
		return myMin( m( k, l ), m( k, r - pow( 2, k ) + 1 ) ).request();
	}
private:
	CMatrix<T> m;
	std::vector<int> logs;

	static int roundedLog( int x )
	{
		if( x == 1 ) {
			return 0;
		} else {
			return roundedLog( x / 2 ) + 1;
		}
	}
};