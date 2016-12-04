#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class B {
public:
    void solve() {
        std::string s;
        std::cin >> s;
        int n = s.length();
        std::reverse(s.begin(), s.end());
        // inline
        std::vector<int> zf(n);
        int left = 0, right = 0;
        for (int i = 0; i < n; ++i) {
            zf[i] = std::max(0, std::min(right - i, zf[i - left]));
            while (i + zf[i] < n && s[zf[i]] == s[i + zf[i]]) {
                ++zf[i];
            }
            if (i + zf[i] >= right) {
                left = i;
                right = i + zf[i];
            }
        }

        /*for (int i = 0; i < n; ++i) {
            std::cout << zf[i] <<'\n';
        }*/

        int pos = 1;
        // Инициализация так, чтобы вся дробная часть вывелась как период
        int ansLen = n;
        int start = 2;
        while (pos < n - 2) {
            // Проматываем нолики. Предполагаем, что нолики дают период
            while (pos < n - 2 && zf[pos] == 0) {
                ++pos;
            }
            if (pos != n - 2) {
                int curLen = n - zf[pos];
                if (ansLen > curLen) {
                    ansLen = curLen;
                    start = zf[pos] + pos;
                }
                ++pos;
            }
        }

        for (int i = n - 1; i >= start; --i) {
            std::cout << s[i];
        }
        std::cout << '(';
        for (int i = start - 1; i >= n - ansLen; --i) {
            std::cout << s[i];
        }
        std::cout << ")\n";
    }
};
