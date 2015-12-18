#pragma once
#include "stdafx.h"

class CMatrix {
public:
	CMatrix(int _w, int _h) :w(_w), h(_h)
	{
		m = new int*[h];
		for( int i = 0; i < h; ++i ) {
			m[i] = new int[w];
		}
	}

	~CMatrix()
	{
		for( int i = 0; i < h; ++i ) {
			delete[] m[i];
		}
		delete[] m;
	}

	int& operator()( int a, int j )
	{
		return m[a][j];
	}

	int getHeight() const { return h; }
	int getWidth() const { return w; }

private:
	int** m; // Первый индекс - строка, второй - столбец.
	int w; // Количество столбцов.
	int h; // Количество строк.
};
class CSparseTable {
public:
	explicit CSparseTable( std::vector<int>& a ) :m( a.size(), roundedLog( a.size() )+1)
	{
		// Первая строка - исходный массив.
		for( int i = 0; i < a.size(); ++i ) {
			m( 0, i ) = a[i];
		}
		// Заполняем остальные.
		for( int i = 1; i < m.getHeight(); ++i ) {
			for( int j = 0; j < m.getWidth(); ++j ) {
				m( i, j ) = std::min( m( i - 1, j ), m( i - 1, j + pow( 2, i - 1 ) ) );
			}
		}
		// Предпросчитываем логарифмы.
		for( int i = 1; i < m.getWidth()+2;++i ) {
			logs.push_back( roundedLog( i ) );
		}
	}

	~CSparseTable() {}

	int RMQ(int l, int r)
	{
		r -= 1; l -= 1;
		int k = logs[ r - l+1 ]; // Логарифм расстояния между краями.
		return std::min( m( k,l ), m(  k,r - pow( 2, k )+1  ) );
	}
private:
	CMatrix m;
	std::vector<int> logs;

	static int roundedLog(int x)
	{
		if (x == 1 ) {
			return 0;
		} else {
			return roundedLog( x / 2 ) + 1;
		}
	}
};