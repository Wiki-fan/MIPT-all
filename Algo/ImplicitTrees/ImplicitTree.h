#pragma once
#include "../stdafx.h"

class IImplicitTree {
public:
	IImplicitTree() { }
	virtual ~IImplicitTree() { }

	// Вставка элемента со значением k на позицию i.
	virtual void Insert( int i, int k ) = 0;

	// Присвоение элементу с индексом i значения k.
	virtual void Assign( int i, int k ) = 0;

	// Сумма на полуинтервале [l, r).
	virtual int SumSubSegment( int l, int r ) = 0;

	// Лексикографически следующая перестановка на полуинтервале [l, r).
	virtual bool NextPermutation( int r, int l ) = 0;
};