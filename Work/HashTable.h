#pragma once
#include "stdafx.h"

class CHashTable
{
public:
	CHashTable() :keysCount(0) {}
	virtual ~CHashTable() {}

	// �������� ������� �����.
	virtual bool Has( const std::string& key ) const = 0;
	// ���������� �����. 
	// ���� ����� ��������� ���������� ������ ������� �������, �� ������� �����������.
	virtual bool Add( const std::string& key ) = 0;
	// �������� �����. ���������� ������� �� ��������.
	// ���������� ������� �� ��������.
	virtual bool Remove( const std::string& key ) = 0;

protected:

	size_t keysCount;
	virtual int myHash( const std::string& key ) const = 0;
	virtual void rehash() = 0;
};