#pragma once
#include "stdafx.h"
#include "Vector.h"
#include "Sorts.h"

struct CPoint {
	int x;
	bool b;

	CPoint() : x( 0 ), b( false ) { }
	CPoint( int _x, bool _b ) : x( _x ), b( _b ) { }

	bool operator<( const CPoint& other )
	{
		return x < other.x;
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
