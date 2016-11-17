#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include "BaseTask.h"
#include "DinicOnAdjacencyLists.h"

class E : public BaseTask, public DinicOnAdjacencyLists<> {
public:
    void solve() {
        read();
        lastLiked.resize(boysCount + girlsCount + 2);
        int q;
        std::cin >> q;
        for (int i = 0; i < q; ++i) {
            int boysInGroup;
            std::cin >> boysInGroup;
            lastLiked.assign(boysCount + girlsCount + 2, INF);
            for (int j = 0; j < boysInGroup; ++j) {
                int boy;
                std::cin >> boy;
                --boy;
                for (int z = 0; z < girls[boy].size(); ++z) {
                    if (lastLiked[girls[boy][z]] > boy + girlsCount + 2) {
                        lastLiked[girls[boy][z]] = boy + girlsCount + 2;
                    }
                }
            }
            int likeCount = 0;
            for (int j = 0; j < g[source].size(); ++j) {
                if (lastLiked[g[source][j].to] < INF) {
                    g[source][j].capacity = 1;
                    ++likeCount;
                } else {
                    g[source][j].capacity = 0;
                }
            }
            size_t flow = findMaxFlow(source, target);
            if (flow == likeCount) {
                std::cout << "Yes" << std::endl;
            } else {
                std::cout << "No" << std::endl;
            }
            // reset flow
            for (int j = 0; j < vertexCount; ++j)
                for (int k = 0; k < g[j].size(); ++k)
                    g[j][k].flow = 0;
        }
    }

    bool bfs() override {
        std::queue<int> queue;
        begins.assign(vertexCount, 0);
        distance.assign(vertexCount, INF);
        distance[source] = 0;
        queue.push(source);

        while (!queue.empty()) {
            int u = queue.front();
            queue.pop();
            for (int i = 0; i < g[u].size(); ++i)
                if (((g[u][i].capacity - g[u][i].flow) > 0) && (distance[g[u][i].to] == INF) && (g[u][i].to < lastLiked[u])) {
                    distance[g[u][i].to] = distance[u] + 1;
                    queue.push(g[u][i].to);
                }
        }

        return distance[target] != INF;
    }

private:
    int boysCount, girlsCount;
    std::vector<std::vector<int>> girls;
    std::vector<int> lastLiked;

    void read() {
        std::cin >> boysCount >> girlsCount;
        init( boysCount + girlsCount + 2);
        girls.resize(boysCount);
        source = 0;
        target = 1;

        for (int i = 0; i < boysCount; ++i) {
            int a, b;
            std::cin >> a;
            girls[i].resize(a);
            for (int j = 0; j < a; ++j) {
                std::cin >> b;
                insertEdge(b + 1, girlsCount + i + 2, 1);
                girls[i][j] = b + 1;
            }
        }
        for (int i = 0; i < girlsCount; ++i)
            insertEdge(source, i + 2, 1);
        for (int i = 0; i < boysCount; ++i)
            insertEdge(girlsCount + i + 2, target, 1);
    }

};
