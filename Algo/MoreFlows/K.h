#pragma once
#include <iostream>
#include "MinCostOnDijkstra.h"
#include "BaseTask.h"
#include "MinCostOnIterators.h"

template<typename vtype = int, typename FlowType = int, typename CostType = int>
class K : public BaseTask {
public:
    CostNetwork<vtype, FlowType, CostType> net;
    MinCostOnIterators<vtype, FlowType, CostType> mc;

    void solve() {
        read();
        printAnswer();
    }
private:
    vtype n, m, e;
    const static CostType INF = 1000 * 1000 * 1000;

    void read() {
        std::vector<CostType> wn, wm;
        std::cin >> n >> m >> e;
        net.init(n + m + 2, n + m, n + m + 1);
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
            --u, --v;
            net.insertDirectedEdge(u, v + n, 1, -(wn[u] + wm[v]));
        }
        for (int i = 0; i < n; ++i) {
            net.insertDirectedEdge(net.getSource(), i, 1, 0);
        }
        for (int i = 0; i < m; ++i) {
            net.insertDirectedEdge(i + n, net.getTarget(), 1, 0);
        }
    }

    void printAnswer() {
        do {
            auto ans = mc.findMinCostFlow(&net, m + n);
            //std::cout << ans.first << ' ' << ans.second << std::endl;
            if (ans.first < 0) break;
        } while (true);

        CostType cost = 0;
        std::vector<vtype> answer;
        for (vtype v = 0; v<n; ++v) {
            for (auto it = net.begin(v); it != net.end(); ++it) {
                if (it.getResidualCapacity() == 0 && it.getFinish() != net.getSource()) {

                    answer.push_back(it.getEdgeNumber()/2);
                    cost -= it.getCost();
                }
            }
        }

        std::cout << cost <<std::endl;
        std::cout << answer.size() <<std::endl;

        for (vtype i = 0; i<answer.size(); ++i) {
            std::cout <<answer[i]+1 <<' ';
        }
    }

};
