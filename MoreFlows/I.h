#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include "MinCostOnDijkstra.h"
#include "CostNetwork.h"
#include "BaseTask.h"

template<typename vtype = int, typename FlowType = int, typename CostType = int>
class I : public BaseTask {
public:
    void solve() override {
        read();
        printAnswer();
    }

private:
    vtype n, k;
    MinCostOnDijkstra<vtype, FlowType, CostType> fb;
    CostNetwork<vtype, vtype, FlowType, CostType> net, subNet;

    void read() override {
        std::cin >> n >> k;
        net.init(2 * n + 2, 0, 1);

        int a;
        for (vtype i = 0; i < n; ++i)
            for (vtype j = 0; j < n; ++j) {
                std::cin >> a;
                net.insertDirectedEdge(i + 2, n + j + 2, 1, a);
            }

        for (vtype i = 0; i < n; ++i)
            net.insertDirectedEdge(net.getSource(), i + 2, k, 0);

        for (vtype i = 0; i < n; ++i)
            net.insertDirectedEdge(n + i + 2, net.getTarget(), k, 0);
    }

    void printAnswer() override {
        auto ans = fb.findMinCostFlow(&net, k * n);
        std::cout << ans.second << std::endl;
        createSubNetwork();
        for (vtype i = 0; i < k; ++i) {
            findBipartite();
        }
    }

    void createSubNetwork() {
        subNet.init(net.getVertexCount(), net.getSource(), net.getTarget());

        for (vtype i = 0; i < n; ++i)
            for (auto it = net.begin(i+2); it != net.end(); ++it)
                if (it.getFlow() == 1)
                    subNet.insertDirectedEdge(it.getStart(), it.getFinish(), 1, 0);
        for (vtype i = 0; i < n; ++i)
            subNet.insertDirectedEdge(subNet.getSource(), i + 2, 1, 0);
        for (vtype i = 0; i < n; ++i)
            subNet.insertDirectedEdge(n + i + 2, subNet.getTarget(), 1, 0);

    }

    void findBipartite() {
        FlowType flow = fb.findMinCostFlow(&subNet, n).first;
        vtype w;
        for (vtype i = 0; i < n; ++i) {
            for (auto it = subNet.begin(i+2); it != subNet.end(); ++it) {
                if (it.getFlow() == 1 && it.getFinish() != subNet.getSource()) {
                    w = it.getFinish() - n - 1;
                    std::cout << w << ' ';
                    it.setCapacity(0);
                    subNet.getEdgeByNum(it.getBackEdge()).setCapacity(0);

                    break;
                }
            }
        }
        std::cout << std::endl;

        subNet.resetFlow();
        subNet.resetBegins();
    }
};
