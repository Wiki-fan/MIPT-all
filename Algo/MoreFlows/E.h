#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include "BaseTask.h"
#include "../Flow/Network.h"
#include "../Flow/Dinic.h"


template<typename vtype = int, typename etype = int, typename FlowType = int>
class E : public BaseTask, public Dinic<> {
public:
    void solve() {
        read();
        printAnswer();
    }

private:
    using Dinic<vtype, etype, FlowType>::distance;
    using Dinic<vtype, etype, FlowType>::dfs;
    using Dinic<vtype, etype, FlowType>::bfs;
    using Dinic<vtype, etype, FlowType>::performFind;
    using Dinic<vtype, etype, FlowType>::findMaxFlow;
    using Dinic<vtype, etype, FlowType>::INF;
    const int INF2 = 100000;

    Network<vtype, etype, int> net;
    int boysCount, girlsCount;
    std::vector<std::vector<int>> girls; // девочки, нравящиеся i-му мальчику
    std::vector<int> lastLiked;

    bool bfs() override {
        std::queue<vtype> q;
        distance.assign(distance.size(), INF);
        net.resetBegins();
        distance[net.getSource()] = 0;
        q.push(net.getSource());
        while (!q.empty()) {
            vtype u = q.front();
            q.pop();
            for (auto it = net.begin(u); it != net.end(); ++it) {
                if (it.getResidualCapacity() > 0 && distance[it.getFinish()] == INF && it.getFinish() < lastLiked[u]) {
                    distance[it.getFinish()] = distance[u] + 1;
                    q.push(it.getFinish());
                }
            }
        }
        return distance[net.getTarget()] != INF;
    }

    void read() override {
        std::cin >> boysCount >> girlsCount;
        net.init(boysCount + girlsCount + 2, boysCount + girlsCount, boysCount + girlsCount+1); // девочки в начале, мальчики в конце
        girls.resize(boysCount);

        for (int i = 0; i < boysCount; ++i) {
            int numGirlsLiked;
            std::cin >> numGirlsLiked;
            girls[i].resize(numGirlsLiked);
            for (int j = 0; j < numGirlsLiked; ++j) {
                int girl;
                std::cin >> girl;
                --girl;
                net.insertDirectedEdge(girl, girlsCount + i, 1);
                girls[i][j] = girl;
            }
        }
        for (int i = 0; i < girlsCount; ++i)
            net.insertDirectedEdge(net.getSource(), i, 1);
        for (int i = 0; i < boysCount; ++i)
            net.insertDirectedEdge(girlsCount + i, net.getTarget(), 1);
    }

    void printAnswer() {
        lastLiked.resize(boysCount + girlsCount + 2); // номер последнего мальчика, которому нравится i-я девочка
        int q;
        std::cin >> q;
        for (int i = 0; i < q; ++i) {
            int boysInGroup;
            std::cin >> boysInGroup;
            lastLiked.assign(boysCount + girlsCount + 2, INF2);

            for (int j = 0; j < boysInGroup; ++j) {
                int boy;
                std::cin >> boy;
                --boy;
                for (int k = 0; k < girls[boy].size(); ++k) {
                    if (lastLiked[girls[boy][k]] > boy + girlsCount ) {
                        lastLiked[girls[boy][k]] = boy + girlsCount ;
                    }
                }
            }

            int likeCount = 0;
            for (auto it = net.begin(net.getSource()); it != net.end(); ++it) {
                if (lastLiked[it.getFinish()] < INF2) {
                    it.setCapacity(1);
                    ++likeCount;
                } else {
                    it.setCapacity(0);
                }
            }

            // рёбра, которые не нужны, ибо текущий мальчик выберет
            for(int j = 0; j < girlsCount; ++j)
                for (auto it = net.begin(j); it != net.end(); ++it) {
                    if((it.getFinish() < lastLiked[j] ))
                        it.setCapacity(1);
                    else
                        it.setCapacity(0);
                }

            size_t flow = findMaxFlow(&net);
            if (flow == likeCount) {
                std::cout << "Yes" << std::endl;
            } else {
                std::cout << "No" << std::endl;
            }

            net.resetFlow();
        }
    }
};
