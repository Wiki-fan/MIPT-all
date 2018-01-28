#pragma once

#include <vector>
#include <iostream>
#include <limits>
#include "Hungary.h"
#include "BaseTask.h"

class H : public BaseTask {
public:
    void solve() override {
        read();
        printAnswer();
    }
private:
    int n, m;
    std::vector<std::vector<int>> a;
    //std::vector<int> columnPotential, pairs;
    void read() override {
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

    void printAnswer() override {
        // вывод стоимости
        // восстановление ответа
        Hungary h;
        std::cout << h.find(&a);
        for (int i = 1; i <= m; ++i) {
            std::cout << h.getPairs()[i] << ' '  << i << std::endl;
        }
        /*std::vector<int> ans(n + 1);*/
        /*for (int i = 1; i <= m; ++i)
            ans[pairs[i]] = i;*/

    }
};
