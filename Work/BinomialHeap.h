#pragma once
#include "stdafx.h"
#include "MeldableHeap.h"

template<typename T, class Compare>
class CBinomialHeap : public IMeldableHeap<T, Compare> {

public:
	CBinomialHeap() = default;
	CBinomialHeap( const T& key ) : head( new CLayer( key )) { }
	~CBinomialHeap();
	void Add( const T& key ) override;
	T ExtractTop() override;
	bool isEmpty() override { return head->nodes.empty(); }
	IMeldableHeap<T, Compare>* Meld( CBinomialHeap& other );

private:

	struct CLayer {
		CLayer( const T& _key ) : nodes( { _key } ), parent( 0 ), degree( 0 ) { }
		~CLayer();
		std::forward_list<CLayer> nodes;
		CLayer* parent;
		int degree;
		T value;
	};

	//CBinomialHeap( CNode* _head ) : head( _head ) { }
	CLayer* head; // Корневые элементы.

};

template<typename T, class Compare>
CBinomialHeap<T, Compare>::~CBinomialHeap()
{
	delete head;
}

template<typename T, class Compare>
IMeldableHeap<T, Compare>* CBinomialHeap<T, Compare>::Meld( CBinomialHeap& other )
{
	if( other.isEmpty()) {
		return this;
	}

	// Создание новой кучи как результата слияния корневых списков данных куч.
	head->nodes = head->nodes;
	head->parent = 0;
	head->nodes.merge( other.head->nodes, Compare());

	if( isEmpty()) {
		return this;
	}

	// Сливание рядом стоящих куч одной степени.
	CBinomialHeap::CLayer* cur = head;
	auto layer1 = head->nodes.begin();
	auto layer2 = head->nodes.begin();
	++layer2;
	for( ; layer2 != head->nodes.end(); ++layer1, ++layer2 ) {
		if( layer1->degree == layer2->degree ) {
			layer1->parent = layer2->parent;
			( layer1->nodes ).insert_after( layer1->nodes.end(), layer2->nodes.begin(), layer2->nodes.end());
			head->nodes.erase_after( layer2 );
		}
	}

	return this;
}

template<typename T, class Compare>
void CBinomialHeap<T, Compare>::Add( const T& key )
{
	CBinomialHeap tmp( key );
	Meld( tmp );
}

template<typename T, class Compare>
T CBinomialHeap<T, Compare>::ExtractTop()
{
	massert( !isEmpty());
	// Ищем максимум.
	T top = head->nodes.front().value;
	auto topIter = head->nodes.begin();
	auto iter = head->nodes.begin();
	++iter;
	for( ; iter != head->nodes.end(); ++iter ) {
		if( Compare()( iter->value, top )) {
			top = iter->value;
			topIter = iter;
		}
	}

	// Создаём временную пустую кучу.
	CBinomialHeap tmp;
	// Цепляем к ней детей удаляемой вершины.
	tmp.head = &( *topIter );
	// Выставляем им правильного родителя.
	std::for_each( tmp.head->nodes.begin(), tmp.head->nodes.end(),
	               [ tmp ]( CLayer& layer ) { layer.parent = tmp.head; } );
	// Удаляем вершину.
	head->nodes.erase_after( topIter );
	// Сливаем кучи.
	Meld( tmp );

	return top;
}

/*struct CNode {
public:
	CNode( const T& _key ) : key( _key ), parent( 0 ), child( 0 ),
							 sibling( 0 ), degree( 1 ) { }
	T key;
	CNode* parent;
	CNode* child;
	CNode* sibling;
	mysize degree;
};*/

/*template<typename T, class Compare>
CBinomialHeap<T, Compare>::~CBinomialHeap()
{
	// TODO: деструктор
}

template<typename T, class Compare>
IMeldableHeap<T, Compare>* Meld( CBinomialHeap<T, Compare>& heap1, CBinomialHeap<T, Compare>& heap2 )
{
	if( head == 0 ) {
		return &heap2;
	}
	if( heap2.head == 0 ) {
		return &heap1;
	}
	CBinomialHeap<type, std::greater<type>>::CNode* res = 0;
	CBinomialHeap<type, std::greater<type>>::CNode* iRes = res, * iHeap1 = head, * iHeap2 = heap2.head;
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

template<typename T, class Compare>
void CBinomialHeap<T, Compare>::Add( const T& key )
{
	T hss;
	CBinomialHeap tmp( key );
	head = Meld( *this, tmp );
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

*/
