#pragma once
#include "HashTable.h"

class COpenAddressHashTable : public CHashTable
{
public:
	COpenAddressHashTable();
	~COpenAddressHashTable();

	virtual bool Has( const std::string& key ) const;
	virtual bool Add( const std::string& key );
	virtual bool Remove( const std::string& key );
private:
	struct CTableNode
	{
		std::string *Key;
		bool Deleted;
		CTableNode(const std::string& key) : Key(new std::string(key)), Deleted( false ) {}
		~CTableNode() { delete Key; }
		void deleteItem() { delete Key; }
	};
	std::vector<CTableNode*> table;

	virtual int myHash( const std::string& key ) const;
	virtual void rehash();
};

