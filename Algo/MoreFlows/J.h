#pragma once
#include <iostream>
#include <algorithm>
#include "MinCostOnIterators.h"
#include "BaseTask.h"

template<typename vtype = int, typename FlowType = int, typename CostType = int>
class J :public BaseTask {
public:
    void solve() override {
        read();
        // std::cout << fb.findMinCostFlow(k, source, target).first<< std::endl;
        fb.findMinCostFlow(&net, k);
        printAnswer();
    }
private:
    struct Task { vtype start, finish; CostType cost; };
    struct TaskNum { vtype start, index; };
    vtype n, k;
    MinCostOnIterators<vtype, FlowType, CostType> fb;
    CostNetwork<> net;

    void read() override {
        std::cin >> n >> k;
        net.init(n * 2 + 2, 0, 1);
        std::vector<Task> tasks(net.getVertexCount());
        std::vector<TaskNum> vec(n);

        int s, t, c;
        for (int i = 0; i < n; ++i) {
            std::cin >> s >> t >> c;

            tasks[i] = {s, s+t, c};
            vec[i] = {s, i};
        }


        std::sort(vec.begin(), vec.end(), [](TaskNum& a, TaskNum& b){ return a.start < b.start; });
        for (vtype i = 0; i < n - 1; ++i) {
            net.insertDirectedEdge(vec[i].index+2, vec[i+1].index+2, 2*k, 0);
            for (vtype j = i+1; j < n; ++j)
                if (tasks[vec[i].index].finish <= tasks[vec[j].index].start) {
                    net.insertDirectedEdge(n + vec[i].index+2, vec[j].index+2, 2*k, 0);
                    break;
                }
        }
        net.insertDirectedEdge(vec[n-1].index+2, net.getTarget(), 2*k, 0);
        for (vtype i = 0; i < n; i++)
            net.insertDirectedEdge(i + 2, i + n + 2, 1, -tasks[i].cost);
        for (vtype i = 0; i < n; i++)
            net.insertDirectedEdge(net.getSource(), i + 2, 2*k, 0);
        for (vtype i = 0; i < n; i++)
            net.insertDirectedEdge(n + i + 2, net.getTarget(), 2*k, 0);
    }

    void printAnswer() override {
        for (vtype i = 0; i < n; ++i) {
            int fl = 0;
            for (auto it = net.begin(i+n+2); it != net.end(); ++it)
                if (it.getFlow() == 1) {
                    fl = 1;
                    break;
                }
            std::cout << fl << ' ';
        }
    }
};