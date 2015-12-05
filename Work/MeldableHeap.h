#pragma once
#include "stdafx.h"

// Интерфейс объединяемой кучи.
template<typename T, class Compare>
class IMeldableHeap {

public:
	IMeldableHeap() { };
	virtual ~IMeldableHeap() { };

	virtual void Add( const T& key ) = 0;
	virtual T ExtractTop() = 0;
	//virtual IMeldableHeap* Meld( IMeldableHeap& heap1, IMeldableHeap& heap2 ) = 0;
	//virtual IMeldableHeap* Meld( IMeldableHeap& other ) = 0;
	virtual bool isEmpty() = 0;

};
