template<typename TYPE>
class CPtrOwner {
public:
	// �����������. �������� �� �������� ���������� ��� ���������.
	// �������������� �������������: CPtrOwner<CSomeInstance> instancePtr( new CSomeInstance() );
	explicit CPtrOwner( TYPE* _ptr = 0 );
	// ����������. ������� ��������� ������ ���������� delete
	~CPtrOwner();

	// �������� ������������.
	const CPtrOwner<TYPE>& operator = ( TYPE* other );

	// ��������� ��������� ������ � �������� ���������.
	// ����� �������� ���� ��� �������� ��������� �� ������.
	void Release();

	// ���������� �� �������� ��������, �� ������ ���.
	// ���������� ��������� �� ��������� ������. ����� ������� 0.
	TYPE* Detach();

	// ����� ����� �������� �� ��������� ��������� ����.
	// ���������� true, ���� ��������� ����� ����, � false � ��������� ������
	bool IsNull() const;

	// ���������� ��������� �� ��������� ������. ����� ������� ����.
	TYPE* Ptr();
	// ���������� ����������� ��������� �� ��������� ������. ����� ������� ����.
	const TYPE* Ptr() const;

	// ���������� ��������� �� ��������� ������.
	// � ������� assert ��������� �� ����������� ��������� ����.
	TYPE* SafePtr();
	// ���������� ����������� ��������� �� ��������� ������.
	// � ������� assert ��������� �� ����������� ��������� ����.
	const TYPE* SafePtr() const;

	// �������� �������� ���������� ���� � ��������� �� ��������� ������.
	// ����� ������� ������� ���������.
	operator TYPE* ();
	operator const TYPE* () const;

	// �������� ������������� ���������.
	// ��������� �� ����������� ��������� ���� � ������� assert
	TYPE& operator * ();
	const TYPE& operator * () const;

	// �������� ������� � �����.
	// ��������� �� ����������� ��������� ���� � ������� assert
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
