#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include "MinCostFordBellman.h"
#include "BaseTask.h"

template<typename vtype = int, typename FlowType = int, typename CostType = int>
class I : public BaseTask {
public:
    void solve() override {
        read();
        auto ans = g.findMinCostMaxFlow(k * n, source, target);
        std::cout << ans.second << std::endl;
        createSubNetwork();
        for (vtype i = 0; i < k; ++i) {
            findBipartite();
        }
    }

private:
    vtype vertexCount;
    vtype source = 0;
    vtype target = 1;
    vtype n, k;
    MinCostFB<vtype, FlowType, CostType> g, h;

    void read() override {
        std::cin >> n >> k;
        vertexCount = 2 * n + 2;
        g.init(vertexCount);

        int a;
        for (vtype i = 0; i < n; ++i)
            for (vtype j = 0; j < n; ++j) {
                std::cin >> a;
                g.insertEdge(i + 2, n + j + 2, 1, a, 0);
            }

        for (vtype i = 0; i < n; ++i)
            g.insertEdge(source, i + 2, k, 0, 0);

        for (vtype i = 0; i < n; ++i)
            g.insertEdge(n + i + 2, target, k, 0, 0);
    }

    void createSubNetwork() {
        h.init(vertexCount);

        for (vtype i = 0; i < n; ++i)
            for (vtype j = 0; j < g.g[i + 2].size(); ++j)
                if (g.g[i + 2][j].flow == 1)
                    h.insertEdge(i + 2, g.g[i + 2][j].to, 1, 0, 0);
        for (vtype i = 0; i < n; ++i)
            h.insertEdge(source, i + 2, 1, 0, 0);
        for (vtype i = 0; i < n; ++i)
            h.insertEdge(n + i + 2, target, 1, 0, 0);

    }

    void findBipartite() {
        FlowType fl = h.findMinCostMaxFlow(n, source, target).first;
        vtype w;
        for (vtype i = 0; i < n; ++i) {
            for (vtype j = 0; j < h.g[i + 2].size(); ++j) {
                if ((h.g[i + 2][j].flow == 1) && (h.g[i + 2][j].to != source)) {
                    w = h.g[i + 2][j].to - n - 1;
                    std::cout << w << ' ';
                    h.g[i + 2][j].capacity = 0;
                    h.g[h.g[i + 2][j].to][h.g[i + 2][j].back].capacity = 0;

                    break;
                }
            }
        }
        std::cout << std::endl;

        for (vtype i = 0; i < vertexCount; ++i) {
            for (vtype j = 0; j < h.g[i].size(); ++j) {
                h.g[i][j].flow = 0;
            }
        }
    }
};
