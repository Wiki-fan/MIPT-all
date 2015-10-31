/*#define _CRT_SECURE_NO_WARNINGS
// Для вывода ошибок чтения файлов в CFile в stdout.
// #define STDOUT_CFILE_DEBUG
// Для вывода подробных massert'ов.
#define NO_MY_DEBUG

#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstring>
#include <cassert>
#include <stdarg.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

typedef long long int mysize;
typedef long long int type;
// Подставляемая функция assert'а, выводящая файл, номер строки и (если есть) сообщение об ошибке,
// задерживающая окно консоли и после выходящая с кодом 1.
void mfassert( char* file, unsigned int line, bool check, char *format = "", ... );
// Подставляемая функция assert'а, выводящая "error" и выходящая с кодом 1.
void mfrassert( bool check, ... );

// Мой кастомный assert.
// ifndef NO_MY_DEBUG пишет файл, строку и сообщение об ошибке (если таковое имеется).
// Иначе просто выводит "error" и выходит с кодом 1.
#ifndef NO_MY_DEBUG
#define massert( check, ... ) ( mfassert( __FILE__, __LINE__, check, __VA_ARGS__) )
#else
#define massert( check, ... ) ( mfrassert(check) )
#endif
void mfassert( char* file, unsigned int line, bool check, char *format, ... )
{
	if( !check ) {
		printf( "ASSERT: file %s, line %d\n\t", file, line );
		va_list args;
		va_start( args, format );
		vprintf( format, args );
		va_end( args );
		exit( 1 );
	}
}
void mfrassert( bool check, ... )
{
	if( !check ) {
		printf( "error" );
		exit( 1 );
	}
}

namespace my
{

template <typename T>
void swap( T& a, T& b )
{
	if( &a != &b ) { // Проверка на самообмен.
		T temp = a;
		a = b;
		b = temp;
	}
}

template<typename T>
T max( const T& val1, const T& val2 )
{
	return val1 >= val2 ? val1 : val2;
}

} // namespace my

namespace my
{

// Динамический вектор.
template<typename T>
class CVector
{
public:
	// Дефолтный конструктор.
	CVector();
	// Создаёт вектор размера _size
	CVector( mysize _size );
	// Создаёт вектор размера _size и копирует туда _size элементов _items.
	CVector( mysize _size, const T* items );
	// Копирующий конструктор.
	CVector( const CVector& other );
	// Деструктор.
	virtual ~CVector();

	// Элемент на позиции n.
	const T& operator[]( mysize n ) const;
	T& operator[]( mysize n );

	CVector& operator=( const CVector& other );

	// Добавление элемента в конец вектора.
	virtual void push_back( const T& item );
	// Удаление элемента из конца вектора. В отличие от std::vector возвращает удаляемый элемент.
	virtual T pop_back();
	// Перевыделяет память так, чтобы не было лишней неиспользуемой памяти.
	void shrink_to_fit();

	// Проверка пустоты вектора.
	bool empty() const { return (c_ount == 0); }
	// Указатель на начало.
	T* begin() const { return items; }
	// Указатель на элемент, следующий за последним элементом вектора.
	T* end() const { return &(items[size]); }
	// Последний элемент.
	T& back() const
	{
		massert( !empty() );
		return items[c_ount - 1];
	}
	// Первый элемент.
	T& front() const
	{
		massert( !empty() );
		return items[0];
	}

	// Возвращает размер вектора.
	const mysize size() const { return c_ount; }
	// Возвращает количество памяти, отведённое в данный момент под элементы.
	const mysize max_size() const { return maxCount; }

private:
	mysize c_ount; // Размер (количество хранящихся элементов).
	mysize maxCount; // Максимальный размер, a.k.a. размер буфера items.
	T* items; // Указатель на элементы.

			  // Копирование элементов вектора из буфера from в буфер to.
	static void copy( mysize _size, const T *from, T *to );
	// Должна как-нибудь перевыделить память. Пока что не нужна.
	// void realloc( mysize& c_ount, mysize otherCount, T* items, T* otherItems );
};

} // namespace my

  // Размер буфера по умолчанию, когда используется дефолтный конструктор.
const mysize DefaultInitialVectorSize = 8;

namespace my
{

template<typename T>
CVector<T>::CVector() : c_ount( 0 ), maxCount( DefaultInitialVectorSize ), items( new T[maxCount] )
{
}

template<typename T>
CVector<T>::CVector( mysize _count ) :
	c_ount( _count ),
	maxCount( c_ount ),
	items( new T[c_ount] )
{
	massert( c_ount > 0 );
}

template <typename T>
CVector<T>::CVector( mysize _count, const T* _items ) :
	c_ount( _count ),
	maxCount( c_ount ),
	items( new T[c_ount] )
{
	copy( c_ount, _items, items );
}

template <typename T>
CVector<T>::CVector( const CVector& other )
{
	if( maxCount < other.c_ount ) {
		delete[] items;
		maxCount = other.maxCount;
		items = new T[maxCount];
	}
	c_ount = other.c_ount;
	copy( c_ount, other.items, items );
}

template <typename T>
CVector<T>::~CVector()
{
	delete[] items;
}

template<typename T>
CVector<T>& my::CVector<T>::operator=( const CVector& other )
{
	if( &other == this ) {
		return *this;
	} else {
		if( maxCount < other.c_ount ) {
			delete[] items;
			maxCount = other.maxCount;
			items = new T[maxCount];
		}
		c_ount = other.c_ount;
		copy( c_ount, other.items, items );
	}
}

template <typename T>
const T& CVector<T>::operator[] ( mysize n ) const
{
	massert( n >= 0 && n < c_ount );
	return items[n];
}

template <typename T>
T& CVector<T>::operator[] ( mysize n )
{
	massert( n >= 0 && n < c_ount );
	return items[n];
}

template<typename T>
void CVector<T>::push_back( const T& item )
{
	massert( c_ount <= maxCount );
	if( c_ount == maxCount ) {
		maxCount = 2 * maxCount;
		T* new_items = new T[maxCount];

		copy( c_ount, items, new_items );

		new_items[c_ount++] = item;
		delete[] items;
		items = new_items;
	} else {
		items[c_ount++] = item;
	}
}

template<typename T>
T CVector<T>::pop_back()
{
	massert( c_ount > 0 );
	if( c_ount <= maxCount / 2 ) {
		shrink_to_fit();
	}

	return items[--c_ount];
}

template<typename T>
void CVector<T>::copy( mysize _count, const T *from, T *to )
{
	for( int i = 0; i < _count; ++i ) {
		to[i] = from[i];
	}
}

template<typename T>
void CVector<T>::shrink_to_fit()
{
	maxCount = max<mysize>( c_ount + 1, DefaultInitialVectorSize );// На всякий случай, если будет много push_back'ов и pop_back'ов на этом месте.
	T* new_items = new T[maxCount];

	copy( c_ount, items, new_items );

	delete[] items;
	items = new_items;
}

} // namespace my


namespace my
{

// Стек LIFO, организованный на динамическом векторе. Подходит в основном для калькулятора из Calc.cpp.
template<typename T>
class CStack
{
public:
	CStack();
	void push( T elem );
	T pop();
	T& top();
	bool empty() { return vec.empty(); }
private:
	CVector<T> vec;
};

} // namespace my
namespace my
{

template<typename T>
CStack<T>::CStack()
{

}

template<typename T>
void CStack<T>::push( T elem )
{
	vec.push_back( elem );
}

template<typename T>
T CStack<T>::pop()
{
	massert( !empty() );
	return vec.pop_back();
}

template<typename T>
T& CStack<T>::top()
{
	massert( !empty() );
	return vec.back();
}

}

char associativity( char c )
{
	switch( c ) {
	case '+':
	case '*':
	case '/':
	case '-':
		return 'l';
	case '^':
		return 'r';
	default:
		massert( false );
	}
}

int priority( char c )
{
	switch( c ) {
	case '(':
	case ')':
		return 0;
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '^':
		return 3;
	default:
		massert( false );
	}
}

bool ToReversePolish( std::istream& is, std::ostream& os )
{
	my::CStack<char> oper;
	char c;
	int unary_flag = 1; // Хитрый способ. Если 1 — минус или плюс должен быть унарным. Если 0 — нет.
	double a = 0;
	while( is >> c ) {
		if( isdigit( c ) ) {
			is.putback( c );
			is >> a;
			os << a << ' ';
		} else if( (c == '-' || c == '+') && unary_flag == 1 ) { // Минус или плюс унарный в начале строки или после '('.
			is >> a;
			os << "0 " << a << ' ' << c;
		} else if( c == '(' ) {
			oper.push( '(' );
			unary_flag = 2; // В конце цикла будет декрементирован. Будет 1, т.е. если дальше минус, то он унарный.
		} else if( c == ')' ) {
			massert( !oper.empty(), "closing parethence before opening one" );
			while( oper.top() != '(' ) {
				massert( !oper.empty(), "closing parethence before opening one" );
				os << oper.pop();
			}
			oper.pop(); // Удаляем '(' из стека.
		} else if( associativity( c ) == 'l' ) {
			massert( unary_flag != 1, "missing operand for %c", c );
			while( !oper.empty() && oper.top() != '(' && priority( c ) <= priority( oper.top() ) ) {
				os << oper.pop();
			}
			oper.push( c );
		} else if( associativity( c ) == 'r' ) {
			massert( unary_flag != 1, "missing operand for %c", c );
			while( !oper.empty() && oper.top() != '(' && priority( c ) < priority( oper.top() ) ) {
				os << oper.pop();
			}
			oper.push( c );
		}
		if( unary_flag > 0 )
			--unary_flag;
	}
	while( !oper.empty() ) {
		os << oper.pop();
	}
	return true;
}

double CalculateReversePolish( std::istream& is )
{
	my::CStack<double> st;
	char c;
	double a;
	while( is >> c ) {
		if( isdigit( c ) ) {
			is.putback( c );
			is >> a;
			st.push( a );
		} else if( c == '+' ) {
			double a1 = st.pop();
			double a2 = st.pop();
			st.push( a1 + a2 );
		} else if( c == '-' ) {
			double a2 = st.pop();
			double a1 = st.pop();
			st.push( a1 - a2 );
		} else if( c == '*' ) {
			double a1 = st.pop();
			double a2 = st.pop();
			st.push( a1*a2 );
		} else if( c == '/' ) {
			double a2 = st.pop();
			double a1 = st.pop();
			massert( a2 != 0, "division by zero" );
			st.push( a1 / a2 );
		} else if( c == '^' ) {
			double b = st.pop();
			double a = st.pop();
			double res = pow( a, b );
			st.push( res );
		}
	}
	return st.pop();
}

int main()
{
	std::stringstream ss;
	ToReversePolish( std::cin, ss );
	double res = CalculateReversePolish( ss );
	std::cout << res;


	return 0;
}
*/