#pragma once

namespace my {

// Обмен значений двух элементов.
template <typename T>
inline void swap( T& a, T& b );

// Максимальное из значений. Использует operator>= .
template<typename T>
inline T max( const T& val1, const T& val2 );

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
template<class T>
T randBetween( const T l, const T r )
{
	return ( rand() % ( r - l + 1 )) + l;
}


