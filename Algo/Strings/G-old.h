#pragma once

#include <iostream>
#include <set>
#include <string>

// Когда считали, что advance в set работает за O(logN)
class G {
public:
    void solve() {
        read();
    }

private:
    void read() {
        int n;
        std::set<std::string> set;
        std::cin >> n;

        for (int i = 0; i < n; ++i) {
            char c;
            std::cin >> c;
            std::cin.putback(c);
            if (isalpha(c)) {
                // Если строка, добавляем
                std::string s;
                std::getline(std::cin, s);
                set.insert(s);
            } else {
                // Если число, ищем и выводим
                int k;
                std::cin >> k;
                auto iter = set.begin();
                std::advance(iter, k - 1);
                std::cout << *iter << std::endl;
            }
        }
    }
};
