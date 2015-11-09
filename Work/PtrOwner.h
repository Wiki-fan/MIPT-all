template<typename TYPE>
class CPtrOwner {
public:
	// Конструктор. Получает во владение переданный ему указатель.
	// Предполагаемое использование: CPtrOwner<CSomeInstance> instancePtr( new CSomeInstance() );
	explicit CPtrOwner( TYPE* _ptr = 0 );
	// Деструктор. Удаляет владеемый объект оператором delete
	~CPtrOwner();

	// Операция присваивания.
	const CPtrOwner<TYPE>& operator = ( TYPE* other );

	// Разрушает владеемый объект и зануляет указатель.
	// Можно вызывать даже для нулевого указателя на объект.
	void Release();

	// Отстраняет от владения объектом, не удаляя его.
	// Возвращает указатель на владеемый объект. Может вернуть 0.
	TYPE* Detach();

	// Метод явной проверки на равенство указателя нулю.
	// Возвращает true, если указатель равен нулю, и false в противном случае
	bool IsNull() const;

	// Возвращает указатель на владеемый объект. Может вернуть ноль.
	TYPE* Ptr();
	// Возвращает константный указатель на владеемый объект. Может вернуть ноль.
	const TYPE* Ptr() const;

	// Возвращает указатель на владеемый объект.
	// С помощью assert проверяет на неравенство указателя нулю.
	TYPE* SafePtr();
	// Возвращает константный указатель на владеемый объект.
	// С помощью assert проверяет на неравенство указателя нулю.
	const TYPE* SafePtr() const;

	// Операция неявного приведения типа к указателю на владеемый объект.
	// Может вернуть нулевой указатель.
	operator TYPE* ();
	operator const TYPE* () const;

	// Операция разыменования указателя.
	// Проверяет на неравенство указателя нулю с помощью assert
	TYPE& operator * ();
	const TYPE& operator * () const;

	// Операция доступа к члену.
	// Проверяет на неравенство указателя нулю с помощью assert
	TYPE* operator -> ();
	const TYPE* operator -> () const;

private:
	TYPE* ptr;
};

template <typename TYPE>
CPtrOwner<TYPE>::CPtrOwner( TYPE *_ptr )
	:ptr(_ptr)
{
}

template <typename TYPE>
CPtrOwner<TYPE>::~CPtrOwner()
{
	delete ptr;
}

template <typename TYPE>
const CPtrOwner<TYPE> & CPtrOwner<TYPE>::operator=( TYPE *other )
{
	delete ptr;
	ptr = other;
	other->ptr = 0;
	return ptr;
}

template <typename TYPE>
void CPtrOwner<TYPE>::Release()
{
	delete ptr;
	ptr = 0;
}

template <typename TYPE>
TYPE * CPtrOwner<TYPE>::Detach()
{
	TYPE *ret = ptr;
	ptr = 0;
	return ret;
}

template <typename TYPE>
bool CPtrOwner<TYPE>::IsNull() const
{
	return ptr == 0;
}

template <typename TYPE>
TYPE * CPtrOwner<TYPE>::Ptr()
{
	return ptr;
}

template <typename TYPE>
const TYPE * CPtrOwner<TYPE>::Ptr() const
{
	return ptr;
}

template <typename TYPE>
TYPE * CPtrOwner<TYPE>::SafePtr()
{
	assert( ptr != 0 );
	return ptr;
}

template <typename TYPE>
const TYPE * CPtrOwner<TYPE>::SafePtr() const
{
	assert( ptr != 0 );
	return ptr;
}

template <typename TYPE>
CPtrOwner<TYPE>::operator TYPE*()
{
	return ptr;
}

template <typename TYPE>
CPtrOwner<TYPE>::operator const TYPE*() const
{
	return ptr;
}

template <typename TYPE>
TYPE & CPtrOwner<TYPE>::operator*()
{
	return *ptr;
}

template <typename TYPE>
const TYPE & CPtrOwner<TYPE>::operator*() const
{
	return *ptr;
}

template <typename TYPE>
TYPE * CPtrOwner<TYPE>::operator->()
{
	return ptr;
}

template <typename TYPE>
const TYPE * CPtrOwner<TYPE>::operator->() const
{
	return ptr;
}
