#pragma once

#include <vector>
#include <iostream>
#include <limits>
#include "Hungary.h"

class H {
public:
    void solve() {
        read();
        Hungary h;
        h.find(&a);
        printAnswer();
    }
private:
    int n, m;
    std::vector<std::vector<int>> a;
    std::vector<int> rawP, colP, pairs, way;
    const static int INF;
    void read() {
        std::cin >> n;
        m = n;
        a.resize(n + 1);
        for (int i = 0; i < n + 1; ++i)
            a[i].resize(m + 1);
        for (int i = 1; i <= n; ++i) {
            for (int j = 1; j <= m; ++j)
                std::cin >> a[i][j];
        }
    }

    void printAnswer() {
        // вывод стоимости
        int cost = -colP[0];
        std::cout << cost << std::endl;
        // восстановление ответа
        /*std::vector<int> ans(n + 1);*/
        /*for (int i = 1; i <= m; ++i)
            ans[pairs[i]] = i;*/
        for (int i = 1; i <= m; ++i) {
            std::cout << pairs[i] << ' '  << i << std::endl;
        }
    }
};

const int H::INF{std::numeric_limits<int>::max()};