#pragma once


#include <vector>
#include <iostream>
#include <limits>

class H
{
public:
    void solve()
    {
        read();
        find();
        printAnswer();
    }
private:
    int n, m;
    std::vector<std::vector<int>> a;
    std::vector<int> u, v, p, way;
    const static int INF ;
    void read()
    {
        std::cin >> n;
        m = n;
        a.resize(n+1);
        for (int i = 0; i<n+1; ++i)
            a[i].resize(m+1);
        for (int i = 0; i<n; ++i) {
            for (int j = 0; j < m; ++j)
                std::cin >> a[i][j];
        }
    }
    void find()
    {
        u.resize(n + 1);
        v.resize(m + 1);
        p.resize(m + 1);
        way.resize(m + 1);
        for (int i = 1; i <= n; ++i) {
            p[0] = i;
            int j0 = 0;
            std::vector<int> minv(m + 1, INF);
            std::vector<char> used(m + 1, false);
            do {
                used[j0] = true;
                int i0 = p[j0], delta = INF, j1;
                for (int j = 1; j <= m; ++j)
                    if (!used[j]) {
                        int cur = a[i0][j] - u[i0] - v[j];
                        if (cur < minv[j]) {
                            minv[j] = cur;
                        way[j] = j0;}
                        if (minv[j] < delta) {
                            delta = minv[j];
                            j1 = j;
                        }
                    }
                for (int j = 0; j <= m; ++j)
                    if (used[j])
                        u[p[j]] += delta, v[j] -= delta;
                    else
                        minv[j] -= delta;
                j0 = j1;
            } while (p[j0] != 0);

            do {
                int j1 = way[j0];
                p[j0] = p[j1];
                j0 = j1;
            } while (j0 != 0);
        }
    }

    void printAnswer()
    {
        // вывод стоимости
        int cost = -v[0];
        std::cout << cost <<std::endl;
        // восстановление ответа
        std::vector<int> ans (n+1);
        for (int i=1; i<=m; ++i)
            ans[p[i]] = i;
        for (int i=1; i<=m; ++i) {
            std::cout <<i <<' ' <<ans[i] <<std::endl;
        }
    }
};
const int H::INF{ std::numeric_limits<int>::max()};