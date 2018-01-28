#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include "BaseTask.h"
#include "MinCostOnIterators.h"

template<typename vtype = int, typename FlowType = int, typename CostType = int>
class G : public BaseTask {
public:
    void solve() override {
        read();
        printAnswer();
    }

private:
    FlowType flow = 0;
    vtype n, m, k;
    MinCostOnIterators<> fb;
    CostNetwork<> net;

    void read() override {
        std::cin >> n >> m >> k;
        net.init(n, 0, n-1);

        vtype from, to;
        CostType cost;
        for (vtype i = 0; i < m; ++i) {
            std::cin >> from >> to >> cost;
            net.insertDirectedEdge(from - 1, to - 1, 1, cost, i);
            net.insertDirectedEdge(to - 1, from - 1, 1, cost, i);
        }
    }

    void printAnswer() override
    {
        auto ans = fb.findMinCostFlow(&net, k);
        FlowType maxFlow = ans.first;
        CostType cost = ans.second;
        if (maxFlow < k) {
            std::cout << "-1";
        } else {
            std::cout.setf(std::cout.fixed);
            //std::cout.precision(6);
            std::cout << double(cost) / k << std::endl;

            printOnePath();
        }
    }

    void printOnePath() {
        std::vector<bool> roadIsForbidden(m, false);
        for (vtype i = 0; i < k; ++i) {

            std::vector<int> towns;
            std::vector<bool> used(n, false);
            used[net.getSource()] = true;
            auto it = net.begin(net.getSource());
            while (it.getFlow() == 0 || roadIsForbidden[it.getValue()] == true)
                ++it;
            towns.push_back(it.getValue());
            roadIsForbidden[it.getValue()] = true;
            vtype next = it.getFinish();
            while (next != net.getTarget()) {
                vtype e = next;
                for (auto it = net.begin(e); it != net.end(); ++it) {
                    if (roadIsForbidden[it.getValue()] == false && it.getFlow() > 0 && used[it.getFinish()] == false) {
                        towns.push_back(it.getValue());
                        roadIsForbidden[it.getValue()] = true;
                        used[it.getFinish()] = true;
                        next = it.getFinish();
                        break;
                    }
                }
            }
            std::cout << towns.size() << ' ';
            for (vtype i = 0; i < towns.size(); ++i) {
                std::cout << towns[i] + 1 << ' ';
            }
            std::cout << std::endl;
        }
    }

};

