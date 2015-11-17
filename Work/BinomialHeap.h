#pragma once
#include "stdafx.h"
#include "MeldableHeap.h"

template<typename T, class Compare>
class CBinomialHeap : public IMeldableHeap<T, Compare> {

public:
	CBinomialHeap();
	CBinomialHeap( const T& key ) : head( new CNode( key ))
	{
	}
	~CBinomialHeap();
	void Add( const T& key );
	T ExtractTop();
	friend IMeldableHeap<T, Compare>* Meld( CBinomialHeap& heap1, CBinomialHeap& heap2 );

private:
	struct CNode {
	public:
		CNode( const T& _key ) : key( _key ), parent( 0 ), child( 0 ),
		                         sibling( 0 ), degree( 1 ) { }
		T key;
		CNode* parent;
		CNode* child;
		CNode* sibling;
		mysize degree;
	};

	CBinomialHeap( CNode* _head ) : head( _head ) { }
	CNode* head; // Корневые элементы.

};

template<typename T, class Compare>
CBinomialHeap<T, Compare>::~CBinomialHeap()
{
	// TODO: деструктор
}

template<typename T, class Compare>
void CBinomialHeap<T, Compare>::Add( const T& key )
{
	CBinomialHeap tmp( key );
	head = Meld( *this, tmp ).head;
}

template<typename T, class Compare>
T CBinomialHeap<T, Compare>::ExtractTop()
{
	T topKey = head->key;
	CNode* top = 0, * topPrev = 0;
	CNode* iCur = head, * iPrev = 0; // Будет одно лишнее сравнение, ну и ладно.
	while( iCur != 0 ) {
		if( Compare( iCur->key, topKey )) {
			topKey = iCur->key;
			top = iCur;
			topPrev = iPrev;
		}
		iPrev = iCur;
		iCur = iCur->sibling;
	}

	// Удаление корня.
	if( topPrev == 0 ) {
		head = top->sibling;
	} else {
		topPrev->sibling = top->sibling;
	}

	CBinomialHeap ret;
	ret.head = iCur->child;
	iCur = iCur->child;

	while( iCur != 0 ) {
		iCur->parent = 0;
		iCur = iCur->sibling;
	}
	this = Meld( *this, ret );

	return topKey;
}

template<typename T, class Compare>
IMeldableHeap<T, Compare>* Meld( CBinomialHeap<T, Compare>& heap1, CBinomialHeap<T, Compare>& heap2 )
{
	if( heap1.head == 0 ) {
		return &heap2;
	}
	if( heap2.head == 0 ) {
		return &heap1;
	}
	CBinomialHeap<type, std::greater<type>>::CNode* res = 0;
	CBinomialHeap<type, std::greater<type>>::CNode* iRes = res, * iHeap1 = heap1.head, * iHeap2 = heap2.head;
	while( iHeap1 != 0 && iHeap2 != 0 ) {
		if( iHeap1->degree < iHeap2->degree ) {
			iRes->sibling = iHeap1;
			iRes = iHeap1; // TODO: надо ли менять детей/родителей?
			iHeap1 = iHeap1->sibling;
		} else {
			iRes->sibling = iHeap2;
			iRes = iHeap2; // TODO: надо ли менять детей/родителей?
			iHeap2 = iHeap2->sibling;
		}
	}
	if( iHeap1 == 0 ) {
		while( iHeap2 != 0 ) {
			iRes->sibling = iHeap2;
			iHeap2 = iHeap2->sibling;
		}
	} else {
		while( iHeap1 != 0 ) {
			iRes->sibling = iHeap1;
			iHeap1 = iHeap1->sibling;
		}
	}
	iRes = res;
	while( iRes->sibling != 0 ) {
		if( iRes->degree == iRes->sibling->degree ) {
			iRes->parent = iRes->sibling;
			CBinomialHeap<type, std::greater<type>>::CNode* tmp = iRes->sibling;
			iRes->sibling = iRes->sibling->child;
			iRes = tmp;
			continue;
		}
		iRes = iRes->sibling;
	}

	return new CBinomialHeap<type, std::greater<type>>( res );
}
