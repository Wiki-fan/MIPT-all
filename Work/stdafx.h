#pragma once

#define _CRT_SECURE_NO_WARNINGS
// Для вывода ошибок чтения файлов в CFile в stdout.
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
// Подставляемая функция assert'а, выводящая файл, номер строки и (если есть) сообщение об ошибке, задерживающая окно консоли и после выходящая с кодом 1.
void mfassert( char* file, unsigned int line, bool check, char *format = "", ... );
// Подставляемая функция assert'а, выводящая "error" и выходящая с кодом 1.
void mfrassert( bool check , ...);

// Мой кастомный assert. 
// ifdef NO_MY_DEBUG пишет файл, строку и сообщение об ошибке (если таковое имеется). 
// Иначе просто выводит "error" и выходит с кодом 1.
#ifndef NO_MY_DEBUG
#define massert( check, ... ) ( mfassert( __FILE__, __LINE__, check, __VA_ARGS__) )
#else
#define massert( check, ... ) ( mfrassert(check) )
#endif
