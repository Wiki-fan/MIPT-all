#pragma once

#define _CRT_SECURE_NO_WARNINGS
// ��� ������ ������ ������ ������ � CFile � stdout.
// #define STDOUT_CFILE_DEBUG 
#include <conio.h>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstring>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>
#include <stdarg.h>

typedef long long int mysize;
typedef long long type;
// ������������� ������� assert'�, ��������� ����, ����� ������ � (���� ����) ��������� �� ������, ������������� ���� ������� � ����� ��������� � ����� 1.
void mfassert( char* file, unsigned int line, bool check, char *format = "", ... );
// ������������� ������� assert'�, ��������� "error" � ��������� � ����� 1.
void mfrassert( bool check , ...);

// ��� ��������� assert. 
// ifdef NO_MY_DEBUG ����� ����, ������ � ��������� �� ������ (���� ������� �������). 
// ����� ������ ������� "error" � ������� � ����� 1.
#ifndef NO_MY_DEBUG
#define massert( check, ... ) ( mfassert( __FILE__, __LINE__, check, __VA_ARGS__) )
#else
#define massert( check, ... ) ( mfrassert(check) )
#endif
