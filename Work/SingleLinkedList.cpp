#include "SingleLinkedList.h"

CSingleLinkedList::CNode::CNode( int newVal, CSingleLinkedList::CNode* _next )
		: Value( newVal ), next( _next )
{
}

CSingleLinkedList::CSingleLinkedList() : first( nullptr )
{
}

CSingleLinkedList::~CSingleLinkedList()
{
	while( CNode* temp = first->Next()) {
		delete first;
		first = temp;
	}
	delete first;
}

void CSingleLinkedList::Reverse()
{
	CNode* temp;
	CNode* ret = nullptr;
	while( first ) {
		temp = first->Next();
		first->SetNext( ret );
		ret = first;
		first = temp;
	}
	first = ret;
}

CSingleLinkedList::CNode* CSingleLinkedList::First()
{
	return first;
}

void CSingleLinkedList::AddFirst( int newVal )
{
	CNode* temp = new CNode( newVal, first );
	first = temp;
}

CSingleLinkedList::CNode* CSingleLinkedList::CNode::Next()
{
	return next;
}

