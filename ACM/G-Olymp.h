#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include "MinCostFordBellman.h"
#include "BaseTask.h"

template<typename vtype = int, typename FlowType = int, typename CostType = int>
class G : public BaseTask {
public:
    void solve() override {
        read();
        printAnswer();
    }

private:
    FlowType flow = 0;
    vtype source;
    vtype target;
    vtype n, m, k;
    MinCostFB<> fb;

    void read() override {
        std::cin >> n >> m >> k;
        fb.init(n);
        source = 0;
        target = n - 1;

        vtype from, to;
        CostType cost;
        for (vtype i = 0; i < m; i++) {
            std::cin >> from >> to >> cost;
            fb.insertEdge(from - 1, to - 1, 1, cost, i);
            fb.insertEdge(to - 1, from - 1, 1, cost, i);
        }
    }

    void printAnswer() override
    {
        auto ans = fb.findMinCostMaxFlow(k, source, target);
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
        for (vtype i = 0; i < k; i++) {

            std::vector<int> towns;
            std::vector<bool> used(n, false);
            used[source] = true;
            vtype j = 0;
            while (fb.g[source][j].flow == 0 || roadIsForbidden[fb.g[source][j].number] == true)
                j++;
            towns.push_back(fb.g[source][j].number);
            roadIsForbidden[fb.g[source][j].number] = true;
            vtype next = fb.g[source][j].to;
            while (next != target) {
                vtype e = next;
                for (vtype h = 0; h < fb.g[e].size(); h++) {
                    if (roadIsForbidden[fb.g[e][h].number] == false && fb.g[e][h].flow > 0 && used[fb.g[e][h].to] == false) {
                        towns.push_back(fb.g[e][h].number);
                        roadIsForbidden[fb.g[e][h].number] = true;
                        used[fb.g[e][h].to] = true;
                        next = fb.g[e][h].to;
                        break;
                    }
                }
            }
            std::cout << towns.size() << ' ';
            for (vtype i = 0; i < towns.size(); i++) {
                std::cout << towns[i] + 1 << ' ';
            }
            std::cout << std::endl;
        }
    }

};

