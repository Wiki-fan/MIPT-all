#pragma once

#include <string>
#include <iostream>
#include <vector>

class D {
public:
    void solve() {
        std::string s;
        std::cin >> s;
        int n = s.length();

        int k = 0;
        std::vector<int> compactness(n), rborder(n), pf(n);

        // Подсчёт префикс-функции с дополнительным ифом в конце каждой итерации
        for (int i = 1; i < n; i++) {
            while (k > 0 && s[i] != s[k]) {
                k = pf[k - 1];
            }
            if (s[i] == s[k]) {
                k++;
            }
            pf[i] = k;

            if (k != 0 && rborder[compactness[k - 1]] + compactness[k - 1] + 1 >= i) {
                compactness[i] = compactness[k - 1];
                rborder[compactness[i]] = i;
            } else {
                compactness[i] = i;
                rborder[i] = i;
            }
        }

        for (int i = 0; i < n; i++) {
            std::cout << compactness[i] + 1 << " ";
        }
    }
};
