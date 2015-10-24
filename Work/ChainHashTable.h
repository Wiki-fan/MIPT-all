#pragma once
#include "stdafx.h"
#include "HashTable.h"

// ���-�������, ����������� �������� ������� �������.
class CChainHashTable : public CHashTable
{
public:
	CChainHashTable();
	~CChainHashTable();
	// �������� ������� �����.
	virtual bool Has( const std::string& key ) const;
	// ���������� �����. 
	// ���� ����� ��������� ���������� ������ ������� �������, �� ������� �����������.
	virtual bool Add( const std::string& key );
	// �������� �����. ���������� ������� �� ��������.
	// ���������� ������� �� ��������.
	virtual bool Remove( const std::string& key );
private:
	// ������� �������.
	struct CListNode
	{
		std::string Key;
		CListNode* Next;
		CListNode( const std::string& key ) : Key( key ), Next( 0 ) {}
		~CListNode() { delete Next; }
	};
	std::vector<CListNode*> table;
	virtual int myHash( const std::string& key ) const;
	virtual void rehash();
};