#pragma once
#include "stdafx.h"
#include "MeldableHeap.h"

template<typename T, class Compare>
class CBinomialHeap : public IMeldableHeap<T, Compare> {

public:
	// Конструктор по умолчанию, создаёт пустую кучу.
	CBinomialHeap() : head() { }
	// Конструктор, создающий кучу с единственным элементом с ключом key.
	CBinomialHeap( const T& key ) : head( { CLayer( key ) } ) { }
	~CBinomialHeap();
	void Add( const T& key ) override;
	T ExtractTop() override;
	bool isEmpty() override { return head.empty(); }
	IMeldableHeap<T, Compare>* Meld( CBinomialHeap& other );

private:

	struct CLayer {
		CLayer() { }
		CLayer( const T& _key ) : nodes(), key( _key ), parent( 0 ), degree( 0 ) { }
		~CLayer() = default;
		bool operator>( const CLayer& other ) const { return Compare()( key, other.key ); }

		std::forward_list<CLayer> nodes; // Список детей.
		CLayer* parent; // Родитель.
		int degree; // Высота.
		T key; // Ключ.
	};

	//CLayer* head; // Корневые элементы.
	std::forward_list<CLayer> head; // Список корневых элементов.

};

template<typename T, class Compare>
CBinomialHeap<T, Compare>::~CBinomialHeap()
{
	// По сути, всё должно удалиться само.
	//delete head; 
}

template<typename T, class Compare>
IMeldableHeap<T, Compare>* CBinomialHeap<T, Compare>::Meld( CBinomialHeap& other )
{
	if( other.isEmpty()) {
		return this;
	}

	// Упорядоченное слияние корневых списков данных куч.
	head.merge( other.head, std::greater<CLayer>());

	// Слияние рядом стоящих куч одной степени.
	auto layer1 = head.begin();
	auto layer2 = head.begin();
	++layer2; // Костыль.
	for( ; layer2 != head.end(); ++layer1, ++layer2 ) {
		if( layer1->degree == layer2->degree ) {
			//Цепляем layer2 к layer1.
			/*layer1->parent = layer2->parent;
			layer1->nodes.insert_after( layer1->nodes.end(), layer2->nodes.begin(), layer2->nodes.end());
			head.nodes.erase_after( layer2 ); // Удаляем пустой*/
			auto tmp = layer2;
			layer2->parent = &( *layer1 );
			layer1->nodes.insert_after( layer1->nodes.begin(), *layer2 );
			head.erase_after( layer1 ); // Удаляем layer2.
		}
	}

	return this;
}

template<typename T, class Compare>
void CBinomialHeap<T, Compare>::Add( const T& key )
{
	CBinomialHeap<T, Compare> tmp( key );
	Meld( tmp );
}

template<typename T, class Compare>
T CBinomialHeap<T, Compare>::ExtractTop()
{
	massert( !isEmpty());
	// Ищем максимум.
	T top = head.front().key;
	auto iter1 = head.before_begin();
	auto beforeTop = iter1;
	auto iter2 = head.begin();
	for( ; iter2 != head.end(); ++iter2 ) {
		if( Compare()( iter2->key, top )) {
			top = iter2->key;
			beforeTop = iter1;
		}
	}

	// Создаём временную пустую кучу.
	CBinomialHeap tmp;
	// Цепляем к ней детей удаляемой вершины.
	tmp.head = iter1->nodes;
	// Выставляем им правильного родителя.
	std::for_each( tmp.head.begin(), tmp.head.end(),
	               [ &tmp ]( CLayer& layer ) mutable { layer.parent = 0; } );
	// Удаляем вершину.
	head.erase_after( iter1 ); // выстрел в ногу? на что теперь указывает tmp?
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
	T topKey = head.key;
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
