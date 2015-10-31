#pragma once
#include "stdafx.h"

class CTable
{
public:
	CTable( int _n, int _m, int defaultValue );

	int& operator()( int i, int j );

	// ������ � �����������.
	class iterator : public std::iterator<std::random_access_iterator_tag, int>
	{
	public:
		iterator operator++();
		iterator operator++( int );
	private:
		CTable& parent;
		int i;
		int j;
		friend class CTable;
		iterator( CTable& _parent, int _i, int _j );
	};
	iterator begin();
	iterator end();

private:
	int* buffer;
	int n;
	int m;
};

CTable::CTable( int _n, int _m, int defaultValue ) :
	n( _n ),
	m( _m )
{
	buffer = new int[m*n];
	::memset( buffer, m*n*sizeof( int ), defaultValue );
}

int& CTable::operator()( int i, int j )
{
	massert( 0 <= i && i < n );
	massert( 0 <= j && j < m );
	return buffer[i*m + j];
}

int main()
{
	int n = 0;
	std::cin >> n;
	int m = 0;
	std::cin >> m;
	CTable table( n, m, 0 );
	for( int i = 0; i < n; ++i ) {
		for( int j = 0; j < m; ++j ) {
			std::cin >> table( i, j );
		}
	}
	std::sort( table.begin(), table.end(), std::less<int>() );
	for( int i = 0; i < n; ++i ) {
		for( int j = 0; j < m; ++j ) {
			std::cout << table( i, j ) << ' ';
		}
		std::cout << std::endl;
	}
	return 0;

}

CTable::iterator CTable::begin()
{
	return iterator( *this, 0, 0 );
}

CTable::iterator CTable::end()
{
	return iterator( *this, n, n % 2 == 0 ? 0 : m - 1 );
}

CTable::iterator CTable::iterator::operator++()
{
	if( i % 2 == 0 ) {
		if( j != parent.m ) {
			++j;
		} else {
			++i;
		}
	} else {
		if( j != 0 ) {
			--j;
		} else {
			++i;
		}
	}
	return *this;
}
CTable::iterator CTable::iterator::operator++( int )
{
	iterator temp = *this;
	if( i % 2 == 0 ) {
		if( j != parent.m ) {
			++j;
		} else {
			++i;
		}
	} else {
		if( j != 0 ) {
			--j;
		} else {
			++i;
		}
	}
	return *this;
}

CTable::iterator::iterator( CTable& _parent, int _i, int _j ) :
	parent( _parent ),
	i( _i ),
	j( _j )
{

}