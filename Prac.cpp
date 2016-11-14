#include <iostream>
#include "Prac.h"

// Тесты из условия
// ab+c.aba.*.bac.+.+* a 4 NO
// acb..bab.c.*.ab.ba.+.+*a. b 2 YES
// Мои тесты
// aa.a+b. a 2 YES
// aa.b.c.aa.a.+ a 3 YES
// 1aa.+b. a 2 YES
// a*b. a 123456 YES
// aa.a. a 4 NO
// a++ a 42 ParseException
int main()
{
    Prac<> prac;
    std::string s;
    int k;
    char x;
    std::cin >>s >>x>>k;
    if (prac.solve(s, x, k)) {
        std::cout << "YES" << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }

    return 0;
}