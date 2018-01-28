#include "../stdafx.h"
#include "TestBracketSequence.h"

void TestBracketSequence()
{
	int n;
	scanf( "%d", &n );
	CVector<CPoint> points( n * 2 );
	int x;
	for( int i = 0; i < n * 2; i += 2 ) {
		scanf( "%d", &x );
		points[i].Set( x, true );
		scanf( "%d", &x );
		points[i + 1].Set( x, false );
	}
	CPoint* buffer = new CPoint[points.size()];
	MergeSort<CPoint>( points.begin(), points.end(), buffer );
	int cnt = 1;    // points[0] - начало, оно явно является открывающейся скобкой
	int sum = 0;
	for( mysize i = 1; i < points.size(); ++i ) {
		if( cnt == 1 ) {
			sum += points[i].x - points[i - 1].x;
		}
		if( points[i].b ) {
			++cnt;
		} else {
			--cnt;
		}
	}
	printf( "%d", sum );
}