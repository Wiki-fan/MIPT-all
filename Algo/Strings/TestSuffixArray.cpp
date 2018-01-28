#include "SuffixArray.h"

#include <vector>
#include <iostream>

// http://www.spoj.com/problems/SARRAY/
// https://www.e-olymp.com/ru/problems/2894
int main() {
    std::string s;
    std::getline(std::cin, s);
    SuffixArray sa;
    std::vector<int> suffArr = sa.buildFromLecture(s);

    for (int i = 0; i < suffArr.size(); ++i) {
        std::cout << suffArr[i] << ' ';
    }
}