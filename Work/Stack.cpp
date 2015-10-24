#include "stdafx.h"
#include "Stack.h"

namespace my
{

template<typename T>
CStack<T>::CStack()
{
	
}

template<typename T>
void CStack<T>::push( T elem )
{
	vec.push_back( elem );
}

template<typename T>
T CStack<T>::pop()
{
	massert( !empty() );
	return vec.pop_back();
}

template<typename T>
T& CStack<T>::top()
{
	massert( !empty() );
	return vec.back();
}

}