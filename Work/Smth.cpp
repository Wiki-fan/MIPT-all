#include "stdafx.h"
#include "Smth.h"

namespace my
{

template <typename T>
void swap( T& a, T& b )
{
	if( &a != &b ) { // �������� �� ���������.
		T temp = a;
		a = b;
		b = temp;
	}
}

} // namespace my