#include "stdafx.h"
#include "Stack.h"

enum Associativity {
	left,right
};

Associativity associativity( char c )
{
	switch( c ) {
		case '+':
		case '*':
		case '/':
		case '-':
			return Associativity::left;
		case '^':
			return Associativity::right;
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
		} else if( associativity( c ) == Associativity::left ) {
			massert( unary_flag != 1, "missing operand for %c", c );
			while( !oper.empty() && oper.top() != '(' && priority( c ) <= priority( oper.top() ) ) {
				os << oper.pop();
			}
			oper.push( c );
		} else if( associativity( c ) == Associativity::right ) {
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
			double a2 = st.pop();
			double a1 = st.pop();
			double res = pow( a1, a2 );
			st.push( res );
		}
	}
	return st.pop();
}
