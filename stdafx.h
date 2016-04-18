#pragma once

#define _CRT_SECURE_NO_WARNINGS
// Для вывода ошибок чтения файлов в CFile в stdout.
// #define STDOUT_CFILE_DEBUG 
// Для вывода подробных massert'ов.
// #define NO_MY_DEBUG
#define MY_DOUBLE_HASHING

#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <stdarg.h>
#include <limits.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>
#include <iterator>
#include <functional>
#include <memory>


#include "Other/Smth.h"

typedef long long int mysize;
typedef long long int type;
// Подставляемая функция assert'а, выводящая файл, номер строки и (если есть) сообщение об ошибке, 
// задерживающая окно консоли и после выходящая с кодом 1.
void mfassert( char* file, unsigned int line, bool check, char* format = "", ... );
// Подставляемая функция assert'а, выводящая "error" и выходящая с кодом 1.
void mfrassert( bool check, ... );

// Мой кастомный assert. 
// ifndef NO_MY_DEBUG пишет файл, строку и сообщение об ошибке (если таковое имеется). 
// Иначе просто выводит "error" и выходит с кодом 1.
#ifndef NO_MY_DEBUG
#define massert( check, ... ) mfassert( __FILE__, __LINE__, check, ##__VA_ARGS__)
#else
#define massert( check, ... ) ( mfrassert(check) )
#endif
