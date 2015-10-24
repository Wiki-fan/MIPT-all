#include "stdafx.h"
#include "Smth.h"

namespace my
{

template <typename T>
void swap( T& a, T& b )
{
	if( &a != &b ) { // Проверка на самообмен.
		T temp = a;
		a = b;
		b = temp;
	}
}

} // namespace my