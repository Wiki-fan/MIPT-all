#pragma once
#include "Vector.h"

namespace my
{

// Стек LIFO, организованный на динамическом векторе. Подходит в основном для калькулятора из Calc.cpp.
template<typename T>
class CStack {

public:
	CStack();

	void push( T elem );
	T pop();
	T& top();
	bool empty() const { return vec.empty(); }

private:
	CVector<T> vec;
};

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

} // namespace my