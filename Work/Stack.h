#pragma once
#include "Vector.h"
#include "Vector.cpp"

namespace my
{

// ���� LIFO, �������������� �� ������������ �������. �������� � �������� ��� ������������ �� Calc.cpp.
template<typename T>
class CStack
{
public:
	CStack();
	void push( T elem );
	T pop();
	T& top();
	bool empty() { return vec.empty(); }
private:
	CVector<T> vec;
};

} // namespace my