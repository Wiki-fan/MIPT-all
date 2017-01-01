#pragma once

#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

class B {
public:
    void solve() {
        std::string s;
        s.resize(1000001);
        scanf("%s", &s[0]);
        int n = std::strlen(&s[0]);
        std::reverse(s.begin(), s.begin() + n);
        std::vector<int> zf(n);
        int left = 0, right = 0;
        for (int i = 1; i < n; ++i) {
            if (i <= right) {
                zf[i] = std::min(right - i + 1, zf[i - left]);
            }
            while (i + zf[i] < n && s[zf[i]] == s[i + zf[i]])
                ++zf[i];
            if (i + zf[i] - 1 > right) {
                left = i, right = i + zf[i] - 1;
            }
        }
        std::reverse(s.begin(), s.begin() + n);

        /*for (int i = 0; i < n; ++i) {
            std::cout << zf[i] << ' ' << pf[i] << '\n';
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
                    start = n - zf[pos] - pos;
                }
                ++pos;
            }
        }

        char buffer[1000001];
        char* it = buffer;
        for (int i = 0; i < start; ++i) {
            *it = s[i];
            ++it;
        }
        *it = '(';
        ++it;
        for (int i = start; i < ansLen; ++i) {
            *it = s[i];
            ++it;
        }
        *it = ')';
        ++it;
        *it = '\n';
        ++it;
        *it = '\0';
        puts(buffer);
    }
};
