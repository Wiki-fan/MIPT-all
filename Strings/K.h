#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "SuffixArray.h"

class K {
public:
    void solve() {
        int n;
        std::cin >> n;

        std::string s;
        std::cin >> s;

        std::vector<int> sufArray(n);
        for (int i = 0; i < n; ++i) {
            std::cin >> sufArray[i];
            --sufArray[i];
        }

        SuffixArray sa;
        std::vector<int> lcp = sa.getLCP(s, sufArray);
        for (int i = 0; i < lcp.size() - 1; ++i) {
            std::cout << lcp[i] << ' ';
        }
    }
private:

};
