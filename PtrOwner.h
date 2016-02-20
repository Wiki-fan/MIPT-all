#pragma once
#include "stdafx.h"

template<typename T>
class CPtrOwner {
public:
	// Конструктор. Получает во владение переданный ему указатель.
	// Предполагаемое использование: CPtrOwner<CSomeInstance> instancePtr( new CSomeInstance() );
	explicit CPtrOwner( T* _ptr = 0 );
	// Деструктор. Удаляет владеемый объект оператором delete
	~CPtrOwner();

	// Операция присваивания.
	const CPtrOwner<T>& operator=( T* other );

	// Разрушает владеемый объект и зануляет указатель.
	// Можно вызывать даже для нулевого указателя на объект.
	void Release();

	// Отстраняет от владения объектом, не удаляя его.
	// Возвращает указатель на владеемый объект. Может вернуть 0.
	T* Detach();

	// Метод явной проверки на равенство указателя нулю.
	// Возвращает true, если указатель равен нулю, и false в противном случае
	bool IsNull() const;

	// Возвращает указатель на владеемый объект. Может вернуть ноль.
	T* Ptr();
	// Возвращает константный указатель на владеемый объект. Может вернуть ноль.
	const T* Ptr() const;

	// Возвращает указатель на владеемый объект.
	// С помощью assert проверяет на неравенство указателя нулю.
	T* SafePtr();
	// Возвращает константный указатель на владеемый объект.
	// С помощью assert проверяет на неравенство указателя нулю.
	const T* SafePtr() const;

	// Операция неявного приведения типа к указателю на владеемый объект.
	// Может вернуть нулевой указатель.
	operator T*();
	operator const T*() const;

	// Операция разыменования указателя.
	// Проверяет на неравенство указателя нулю с помощью assert
	T& operator*();
	const T& operator*() const;

	// Операция доступа к члену.
	// Проверяет на неравенство указателя нулю с помощью assert
	T* operator->();
	const T* operator->() const;

private:
	T* ptr;
};

template<typename T>
CPtrOwner<T>::CPtrOwner( T* _ptr )
		: ptr( _ptr )
{
}

template<typename T>
CPtrOwner<T>::~CPtrOwner()
{
	delete ptr;
}

template<typename T>
const CPtrOwner<T>& CPtrOwner<T>::operator=( T* other )
{
	delete ptr;
	ptr = other;
	other->ptr = 0;
	return (const CPtrOwner<T>&) ptr;
}

template<typename T>
void CPtrOwner<T>::Release()
{
	delete ptr;
	ptr = 0;
}

template<typename T>
T* CPtrOwner<T>::Detach()
{
	T* ret = ptr;
	ptr = 0;
	return ret;
}

template<typename T>
bool CPtrOwner<T>::IsNull() const
{
	return ptr == 0;
}

template<typename T>
T* CPtrOwner<T>::Ptr()
{
	return ptr;
}

template<typename T>
const T* CPtrOwner<T>::Ptr() const
{
	return ptr;
}

template<typename T>
T* CPtrOwner<T>::SafePtr()
{
	assert( ptr != 0 );
	return ptr;
}

template<typename T>
const T* CPtrOwner<T>::SafePtr() const
{
	assert( ptr != 0 );
	return ptr;
}

template<typename T>
CPtrOwner<T>::operator T*()
{
	return ptr;
}

template<typename T>
CPtrOwner<T>::operator const T*() const
{
	return ptr;
}

template<typename T>
T& CPtrOwner<T>::operator*()
{
	assert( ptr != 0 );
	return *ptr;
}

template<typename T>
const T& CPtrOwner<T>::operator*() const
{
	assert( ptr != 0 );
	return *ptr;
}

template<typename T>
T* CPtrOwner<T>::operator->()
{
	assert( ptr != 0 );
	return ptr;
}

template<typename T>
const T* CPtrOwner<T>::operator->() const
{
	assert( ptr != 0 );
	return ptr;
}
