#pragma once
#include <iostream>
#include <algorithm>
#include "MinCostFordBellman.h"
#include "BaseTask.h"

template<typename vtype = int, typename FlowType = int, typename CostType = int>
class J :public BaseTask {
public:
    void solve() override {
        read();
        // std::cout << fb.findMinCostMaxFlow(k, source, target).first<< std::endl;
        fb.findMinCostMaxFlow(k, source, target);
        printAnswer();
    }
private:
    struct Task { vtype start, finish; CostType cost; };
    struct TaskNum { vtype start, index; };
    vtype source = 0, target = 1;
    vtype n, k;
    MinCostFB<vtype, FlowType, CostType> fb;

    void read() override {
        std::cin >> n >> k;
        vtype n_vertices = n * 2 + 2;
        std::vector<Task> tasks(n_vertices);
        std::vector<TaskNum> vec(n);

        int s, t, c;
        for (int i = 0; i < n; ++i) {
            std::cin >> s >> t >> c;

            tasks[i] = {s, s+t, c};
            vec[i] = {s, i};
        }

        fb.init(n_vertices);
        std::sort(vec.begin(), vec.end(), [](TaskNum& a, TaskNum& b){ return a.start < b.start; });
        for (vtype i = 0; i < n - 1; ++i) {
            fb.insertEdge(vec[i].index+2, vec[i+1].index+2, 2*k, 0, 0);
            for (vtype j = i+1; j < n; ++j)
                if (tasks[vec[i].index].finish <= tasks[vec[j].index].start) {
                    fb.insertEdge(n + vec[i].index+2, vec[j].index+2, 2*k, 0, 0);
                    break;
                }
        }
        fb.insertEdge(vec[n-1].index+2, target, 2*k, 0,0);
        for (vtype i = 0; i < n; i++)
            fb.insertEdge(i + 2, i + n + 2, 1, -tasks[i].cost, 0);
        for (vtype i = 0; i < n; i++)
            fb.insertEdge(source, i + 2, 2*k, 0, 0);
        for (vtype i = 0; i < n; i++)
            fb.insertEdge(n + i + 2, target, 2*k, 0, 0);
    }

    void printAnswer() override {
        for (vtype i = 0; i < n; ++i) {
            int fl = 0;
            for (vtype j = 0; j < fb.g[i+n+2].size(); ++j)
                if (fb.g[i+n+2][j].flow == 1) {
                    fl = 1;
                    break;
                }
            std::cout << fl << ' ';
        }
    }
};
