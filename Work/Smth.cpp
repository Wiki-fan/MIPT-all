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

template<typename T>
T max( const T& val1, const T& val2 )
{
	return val1 >= val2 ? val1 : val2;
}

} // namespace my