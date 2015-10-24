#pragma once
#include "stdafx.h"

/*struct CPoint
{
	int x;
	bool b;

	CPoint() :x( 0 ), b( false ) {}
	CPoint( int _x, bool _b ) :x( _x ), b( _b ) {}

	bool operator<( const CPoint& other )
	{
		if( x < other.x )
			return true;
		else
			return false;
	}

	void Set( int _x, bool _b )
	{
		x = _x;
		b = _b;
	}

	const CPoint& operator=( const CPoint& other )
	{
		x = other.x;
		b = other.b;
		return *this;
	}
};

int main()
{
	int n;
	scanf( "%d", &n );
	my::CVector<CPoint> points( n * 2 );
	int x;
	for( int i = 0; i < n * 2; i += 2 ) {
		scanf( "%d", &x );
		points[i].Set( x, true );
		scanf( "%d", &x );
		points[i + 1].Set( x, false );
	}
	CPoint *buffer = new CPoint[points.Size()];
	my::MergeSort<CPoint>( points.Begin(), points.End(), buffer );
	int cnt = 1;	// points[0] - начало, оно явно является открывающейся скобкой
	int sum = 0;
	for( mysize i = 1; i < points.Size(); ++i ) {
		if( cnt == 1 ) {
			sum += points[i].x - points[i - 1].x;
		}
		if( points[i].b == true ) {
			++cnt;
		} else {
			--cnt;
		}
	}
	printf( "%d", sum );
	return 0;
}*/