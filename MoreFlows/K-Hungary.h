#pragma once
#include <vector>
#include <iostream>
#include <limits>
#include "BaseTask.h"

template<typename vtype = int, typename FlowType = int, typename CostType = int>
class K_Hungary : public BaseTask {
public:
    void solve() {
        read();
        find();
        printAnswer();
    }

private:
    const FlowType INF = std::numeric_limits<FlowType>::max();
    vtype n, m, e;
    std::vector<std::vector<int>> a;
    std::vector<int> rawP, colP, pairs, way;
    void read() {
        std::vector<CostType> wn, wm;
        std::cin >> n >> m >> e;

        a.resize(n + 1, std::vector<CostType>(m + 1, 0));
        /*for (vtype i = 0; i<n; ++i) {
            for (vtype j = 0; j<m; ++j) {
                a[i][0] = a[0][j] = INF;
            }
        }*/
        for (int i = 0; i < n; ++i) {
            CostType w;
            std::cin >> w;
            wn.push_back(w);
        }
        for (int i = 0; i < m; ++i) {
            CostType w;
            std::cin >> w;
            wm.push_back(w);
        }
        for (int i = 0; i < e; ++i) {
            vtype u, v;
            std::cin >> u >> v;
            a[u][v] = -(wn[u - 1] + wm[v - 1]);
        }
        n = m = std::max(n, m);
    }

    void find() {
        // Raw and column potenital.
        rawP.resize(n + 1);
        colP.resize(m + 1);
        pairs.resize(m + 1); // Matching.
        way.resize(m + 1); // Array of predecessors.
        for (int v = 1; v <= n; ++v) {
            pairs[0] = v;
            int freeCol = 0;
            std::vector<int> minv(m + 1, INF);
            std::vector<bool> used(m + 1, false);
            do {
                used[freeCol] = true;
                int i = pairs[freeCol];
                int delta = INF;
                int colWithMin;
                for (int j = 1; j <= m; ++j)
                    if (!used[j]) {
                        int cur = a[i][j] - rawP[i] - colP[j];
                        if (cur < minv[j]) {
                            minv[j] = cur;
                            way[j] = freeCol;
                        }
                        if (minv[j] < delta) {
                            delta = minv[j];
                            colWithMin = j;
                        }
                    }
                for (int j = 0; j <= m; ++j)
                    if (used[j]) {
                        rawP[pairs[j]] += delta;
                        colP[j] -= delta;
                    } else
                        minv[j] -= delta;
                freeCol = colWithMin;
            } while (pairs[freeCol] != 0);

            do {
                int colWithMin = way[freeCol];
                pairs[freeCol] = pairs[colWithMin];
                freeCol = colWithMin;
            } while (freeCol != 0);
        }
    }

    void printAnswer() {
        // вывод стоимости
        int cost = colP[0];
        std::cout << cost << std::endl;

        for (int i = 1; i <= m; ++i) {
            std::cout << pairs[i] << ' ' << i << std::endl;
        }
        /*std::vector<int> ans(n + 1);
        for (int i = 1; i <= m; ++i)
            ans[pairs[i]] = i;*/
        // восстановление ответа
        std::vector<int> ans(n + 1);
        for (int i = 1; i <= m; ++i)
            ans[pairs[i]] = i;
        for (int i = 1; i <= m; ++i) {
            std::cout << pairs[i] << ' ' << i << std::endl;
        }
    }
};

/*template<typename vtype = int, typename FlowType = int, typename CostType = int>
class K : public BaseTask
{
public:

    void solve()
    {
        read();
        printAnswer();
    }
private:
    vtype n, m, e;
    Hungary hungary;
    std::vector<std::vector<CostType>> a;
    const static CostType INF = 1000*1000*1000;

    void read()
    {
        std::vector<CostType> wn, wm;
        std::cin >>n >>m >>e;
        a.resize(n, std::vector<CostType>(m, 0));
        for (int i = 0; i<n; ++i) {
            CostType w;
            std::cin >> w;
            wn.push_back(w);
        }
        for (int i = 0; i<m; ++i) {
            CostType w;
            std::cin >> w;
            wm.push_back(w);
        }
        for (int i = 0; i<e; ++i) {
            vtype u, v;
            std::cin >>u >>v;
            a[u][n+v]= wn[u]+wm[v];
        }
    }

    void printAnswer()
    {
        std::cout << hungary.find(&a);
        // восстановление ответа
        std::vector<int> ans(n + 1);
        for (int i = 1; i <= m; ++i)
            ans[hungary.pairs[i]] = i;
        for (int i = 1; i <= m; ++i) {
            std::cout << hungary.pairs[i] << ' '  << i << std::endl;
        }
    }

};*/