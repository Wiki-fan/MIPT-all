#include "../stdafx.h"

int MinMaxHeapTest()
{
	my::CHeap<int> h1;
	h1.Push( 4 );
	h1.Push( 6 );
	h1.Push( 2 );
	std::cout << h1.Pop() << std::endl;
	std::cout << h1.Top() << std::endl;
	std::cout << h1.Pop() << std::endl;
	std::cout << h1.Pop() << std::endl;
	std::cout << h1.Pop() << std::endl;
	return 0;
}