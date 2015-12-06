#pragma once
#include "stdafx.h"
#include "MeldableHeap.h"

/*
//На std::forward_list
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
			auto tmp = layer2;
			layer2->parent = &( *layer1 );
			layer1->nodes.push_front( *layer2 );
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
	head.erase_after( beforeTop ); // выстрел в ногу? на что теперь указывает tmp?
	// Сливаем кучи.
	Meld( tmp );

	return top;
}*/

// На указателях.
template<typename T, class Compare>
class CBinomialHeap : public IMeldableHeap<T, Compare> {

	struct CNode {
	public:
		CNode() : parent( 0 ), child( 0 ), sibling( 0 ) { }
		explicit CNode( const T& _key ) : key( _key ), parent( 0 ), child( 0 ),
		                                  sibling( 0 ), degree( 0 ) { }
		~CNode() { delete sibling; delete child; }
		T key;
		CNode* parent;
		CNode* child;
		CNode* sibling;
		mysize degree;
	};

public:
	// Конструктор по умолчанию, создаёт пустую кучу.
	CBinomialHeap() : head( 0 ) { }
	// Конструктор, создающий кучу с единственным элементом с ключом key.
	explicit CBinomialHeap( const T& key ) : head( new CNode( key )) { }
	~CBinomialHeap() { delete head; }
	void Add( const T& key ) override;
	T ExtractTop() override;
	bool isEmpty() override { return head == 0; }
	//IMeldableHeap<T, Compare>* Meld( CBinomialHeap& other );
	virtual IMeldableHeap<T, Compare>* Meld( IMeldableHeap<T, Compare>& other ) override;


	CNode* head;
private:
	CNode* subMeld( CNode* heap1, CNode* heap2 );
	CNode* link( CNode* heap1, CNode* heap2 );
};

template<typename T, class Compare>
IMeldableHeap<T, Compare>* CBinomialHeap<T, Compare>::Meld( IMeldableHeap<T, Compare>& other )
{
	CBinomialHeap& bh = dynamic_cast<CBinomialHeap&> (other);
	head = subMeld( head, bh.head );
	bh.head = 0;
	return this;
}

template<typename T, class Compare>
typename CBinomialHeap<T, Compare>::CNode* CBinomialHeap<T, Compare>::link( CNode* y, CNode* z )
{
	y->parent = z;
	y->sibling = z->child;
	z->child = y;
	++(z->degree);
}

template<typename T, class Compare>
typename CBinomialHeap<T, Compare>::CNode* CBinomialHeap<T, Compare>::subMeld( CNode* heap1, CNode* heap2 )
{
	if (heap1 == heap2) {
		return heap1;
	}
	if( heap1 == 0 ) {
		return heap2;
	}
	if( heap2 == 0 ) {
		return heap1;
	}
	CNode* res = new CNode;
	CNode* iRes = res, * iHeap1 = heap1, * iHeap2 = heap2;
	while( iHeap1 != 0 && iHeap2 != 0 ) {
		if( iHeap1->degree > iHeap2->degree ) {
			iRes->sibling = iHeap1;
			iRes = iHeap1;
			iHeap1 = iHeap1->sibling;
		} else {
			iRes->sibling = iHeap2;
			iRes = iHeap2;
			iHeap2 = iHeap2->sibling;
		}
	}

	while( iHeap2 != 0 ) {
		iRes->sibling = iHeap2;
		iRes = iHeap2;
		iHeap2 = iHeap2->sibling;
	}
	while( iHeap1 != 0 ) {
		iRes->sibling = iHeap1;
		iRes = iHeap1;
		iHeap1 = iHeap1->sibling;
	}

	CNode* prevx = 0;
	CNode* x = res->sibling;
	res->sibling = 0;
	delete res;
	res = x;
	CNode* nextx = x->sibling;
	while (nextx != 0) {
		// Либо степень разная, либо это первые два дерева в цепочке из трёх одинаковой степени. Просто передвигаем указатель.
		if (x->degree != nextx->degree || (nextx->sibling != 0 && nextx->sibling->degree == x->degree)) {
			prevx = x;
			x = nextx;
		} else {
			//
			if( Compare()(x->key, nextx->key) || x->key == nextx->key) {
				x->sibling = nextx->sibling;
				link( nextx, x );
			} else {
				if( prevx == 0 ) {
					res = nextx;
				} else {
					prevx->sibling = nextx;
				}
				link(x, nextx);
				x = nextx;
			}
		}
		nextx = x->sibling;
	}

	/*iRes = res;
	while( iRes->sibling != 0 ) {
		if( iRes->degree == iRes->sibling->degree ) {

			iRes->degree += 1;
			iRes->sibling->parent = iRes;
			CNode* tmp = iRes->sibling;
			iRes->sibling = iRes->sibling->sibling;
			tmp->sibling = iRes->child;
			iRes->child = tmp;
			continue;
		}
		else {
			iRes = iRes->sibling;
		}
	}*/
	return res;
}

template<typename T, class Compare>
void CBinomialHeap<T, Compare>::Add( const T& key )
{
	CNode* tmp = new CNode( key );
	head = subMeld( head, tmp );
}

template<typename T, class Compare>
T CBinomialHeap<T, Compare>::ExtractTop()
{
	// Ищем в корневом списке вершину с наибольшим ключом.
	T topKey = head->key;
	CNode* top = head, * topPrev = 0;
	CNode* i = head, * iPrev = 0;
	while( i != 0 ) {
		if( Compare()( i->key, topKey )) {
			topKey = i->key;
			top = i;
			topPrev = iPrev;
		}
		iPrev = i;
		i = i->sibling;
	}

	massert( top != 0 );
	// Удаление этой вершины.
	if( topPrev == 0 ) {
		head = top->sibling;
	} else {
		topPrev->sibling = top->sibling;
	}

	i = top->child;
	//CNode* ret = i->child;
	// Реверсим список детей (если дети есть).
	if( i != 0 ) {
		CNode* temp;
		CNode* ret = nullptr;
		while( i ) {
			temp = i->sibling;
			i->sibling = ret;
			ret = i;
			i = temp;
		}
		head = subMeld( head, ret );
	}


	return topKey;
}


/*#include "SingleLinkedList.h"

template<typename T, class Compare>
class CBinomialHeap : public IMeldableHeap<T, Compare> {

public:
	// Конструктор по умолчанию, создаёт пустую кучу.
	CBinomialHeap() { }
	// Конструктор, создающий кучу с единственным элементом с ключом key.
	explicit CBinomialHeap( const T& key ) : head( CLayer( key )) { }
	~CBinomialHeap();

	void Add( const T& key ) override;
	T ExtractTop() override;
	bool isEmpty() override { return head.isEmpty(); }
	IMeldableHeap<T, Compare>* Meld( CBinomialHeap& other );

private:

	struct CLayer {
		CLayer() : parent( 0 ), degree( 0 ) { }
		explicit CLayer( const T& _key ) : key( _key ), parent( 0 ), degree( 1 ) { }
		CLayer( const CLayer& other )
				: nodes( other.nodes ), parent( other.parent ), degree( other.degree ), key( other.key ) { }
		~CLayer() { std::cout << "Delete binomial " << key << std::endl; }
		//= default;
		bool operator<( const CLayer& other ) const { return degree >
					other.degree; }

		CSingleLinkedList<CLayer> nodes; // Список детей.
		CLayer* parent; // Родитель.
		int degree; // Высота.
		T key; // Ключ.
	};

	CSingleLinkedList<CLayer>* subMeld( CSingleLinkedList<CLayer>* x, CSingleLinkedList<CLayer>* y );

	//CLayer* head; // Корневые элементы.
	CSingleLinkedList<CLayer> head; // Список корневых элементов.

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
	subMeld( &head, &other.head );
}

template<typename T, class Compare>
CSingleLinkedList<typename CBinomialHeap<T, Compare>::CLayer>* CBinomialHeap<T, Compare>::subMeld( CSingleLinkedList<CLayer>* x, CSingleLinkedList<CLayer>* y )
{
	if( x == y ) {
		return x;
	}
	//if( y->isEmpty()) {
	//	return x;
	//}
	//if (x->isEmpty()) {
	//	return y;
	//}

	// Упорядоченное слияние корневых списков данных куч.
	x->Merge( *y );

	// Слияние рядом стоящих куч одной степени.
	auto iter1 = x->First();
	if( iter1->Next() != 0 ) {
		auto iter2 = x->First()->Next();
		for( ; iter2 != 0; ) {
			if( iter1->Value.degree == iter2->Value.degree ) {
				if( Compare()( iter2->Value.key, iter1->Value.key )) {
					// Цепляем iter2  к iter1.
					iter2->Value.parent = &iter1->Value;
					iter1->Value.nodes.AddFirst( iter2->Value );
					iter1->Value.degree += 1;
					x->DeleteAfter( iter1 );
					iter2 = iter1->Next();
				} else {
					// Цепляем iter1  к iter2.
					iter1->Value.parent = &iter2->Value;
					iter2->Value.nodes.AddFirst( iter1->Value );
					iter2->Value.degree += 1;
					x->DeleteAfter( iter2 );
					iter1 = iter2->Next();
				}

			} else {
				iter1 = iter1->Next();
				iter2 = iter2->Next();
			}
		}
	}

	return x;
}

template<typename T, class Compare>
void CBinomialHeap<T, Compare>::Add( const T& key )
{
	//CBinomialHeap<T, Compare> tmp( key );
	//Meld( tmp );
	CSingleLinkedList<CLayer>* tmp = new CSingleLinkedList<CLayer>;
	tmp->AddFirst( new CLayer( key ));
	head = *subMeld( &head, tmp );
}

template<typename T, class Compare>
T CBinomialHeap<T, Compare>::ExtractTop()
{
	massert( !isEmpty());
	// Ищем максимум.
	T top = head.First()->Value.key;
	auto iter1 = head.First();
	auto beforeTop = iter1;
	auto iter2 = head.First()->Next();
	for( ; iter2 != 0; iter1 = iter1->Next(), iter2 = iter2->Next()) {
		if( Compare()( iter2->Value.key, top )) {
			top = iter2->Value.key;
			beforeTop = iter1;
		}
	}

	// Создаём временную пустую кучу.
	CSingleLinkedList<CLayer> tmp;
	// Цепляем к ней детей удаляемой вершины.
	tmp = iter1->Value.nodes;
	// Выставляем им правильного родителя (нулевого, так как они вылезают в корневой список).
	for( auto iter = tmp.First(); iter != 0; iter = iter->Next()) {
		iter->Value.parent = 0;
	}
	// Удаляем вершину.
	head.DeleteAfter( beforeTop );

	// Сливаем кучи.
	head.Merge( tmp );

	return top;
}*/
