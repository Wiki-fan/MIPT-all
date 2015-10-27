﻿#pragma once
#include "stdafx.h"

// Параметр хеш-функции.
const int HashParameter = 91;

class CHashTable {

public:
	CHashTable() : keysCount( 0 ) {}
	virtual ~CHashTable() {}

	// Добавление ключа. 
	// Если число элементов становится больше размера таблицы, то таблица расширяется.
	virtual bool Add( const std::string& key ) = 0;
	// Удаление ключа.
	// Уменьшение таблицы не заложено.
	virtual bool Remove( const std::string& key ) = 0;
	// Проверка наличия ключа.
	virtual bool Has( const std::string& key ) const = 0;

protected:
	size_t keysCount; // Количество ключей.

	// Перехеширование с увеличением размера таблицы.
	virtual void rehash() = 0;
	// Вычисление хэша от ключа key.
	virtual int myHash( const std::string& key ) const = 0;
};
