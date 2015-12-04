#pragma once
#include "stdafx.h"

// Класс тестирующий список CSingleLinkedList
class CSingleLinkedListTester {
public:
	CSingleLinkedListTester() { }

	// Тестируем метод Reverse, true если удачно
	// Заполняет список случайными числами, запоминает их, делает reverse и проверяет что числа те же в обратом порядке
	bool TestReverse();

	// Тестируем добавление и просмотр некоторого количества элементов
	// true если удачно
	// Заполняет список случайными числами, запоминает их и проверяет, что числа те же
	bool TestAddAndList();

private:
	const int N = 101;
};

void TestSingleLinkedList();