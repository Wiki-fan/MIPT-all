#pragma once
#include <iostream>
#include "network.h"
#include "dinic.h"

template<typename vtype, typename etype>
class Birthday {
public:
    void solve() {
        read();
        findMaximalFlow();
        findWays();
        printResult();
    }

private:
    uint bCount, gCount;
    static const vtype s = 0, t = 1;
    std::vector<bool> visited;
    std::vector<bool> marked;
    Network<uint, int, int> net;

    // Assuming i counts from 1.
    inline vtype getMaleIndex(vtype i) { return 2 + i - 1; }
    inline vtype getFemaleIndex(vtype i) { return 2 + bCount + i - 1; }

    void dfs(vtype v) {
        if (!visited[v]) {
            visited[v] = true;
            for (auto it = net.begin(v); it != net.end(); ++it) {
                if (!it.isDeleted())
                    dfs(it.getFinish());
            }
        }
    }


    void read() {
        std::cin >> bCount >> gCount;
        net.init(bCount + gCount + 2, s, t);
        for (uint j = 1; j <= bCount; ++j) {
            vtype tmp;
            std::vector<bool> girls(gCount, false);
            while (true) {
                std::cin >> tmp;
                if (tmp == 0) {
                    // Insert inversed edges.
                    for (uint r = 1; r <= gCount; ++r) {
                        if (girls[r - 1] == false)
                            net.insertDirectedEdge(getMaleIndex(j), getFemaleIndex(r), 1);
                    }
                    break;
                }
                girls[tmp - 1] = true;
            }
        }
        // Connect boys and girls with source and target.
        for (vtype j = getMaleIndex(1); j <= getMaleIndex(bCount); ++j)
            net.insertDirectedEdge(s, j, 1);
        for (vtype j = getFemaleIndex(1); j <= getFemaleIndex(gCount); ++j)
            net.insertDirectedEdge(j, t, 1);
    }

    void findMaximalFlow() {
        Dinic<uint, int, int> dinic;
        dinic.findMaxFlow(&net);
    }

    void findWays() {
        visited.resize(net.getVertexCount(), false);
        marked.resize(net.getVertexCount(), false);
        visited[s] = true;
        visited[t] = true;

        for (vtype i = getMaleIndex(0); i <= getMaleIndex(bCount); ++i) {
            for (auto e = net.begin(i); e != net.end(); ++e)
                if (e.getFlow() == 1) {
                    marked[i] = true;
                    e.markAsDeleted();
                } else {
                    e.markReverseAsDeleted();
                }
        }

        for (vtype i = getMaleIndex(1); i <= getMaleIndex(bCount); i++)
            if ((marked[i] == false) && (visited[i] == false))
                dfs(i);
    }

    void printResult() {
        std::vector<vtype> boys;
        std::vector<vtype> girls;
        for (vtype i = getMaleIndex(1); i <= getMaleIndex(bCount); i++) {
            if (visited[i]) {
                boys.push_back(i - 1);
            }
        }
        for (vtype i = getFemaleIndex(1); i <= getFemaleIndex(gCount); i++) {
            if (!visited[i]) {
                girls.push_back(i - bCount - 1);
            }
        }
        std::cout << boys.size() + girls.size() << std::endl;
        std::cout << boys.size() << " " << girls.size() << std::endl;
        for (size_t i = 0; i < boys.size(); i++)
            std::cout << boys[i] << " ";

        if (boys.size() != 0) std::cout << std::endl;
        for (size_t i = 0; i < girls.size(); i++)
            std::cout << girls[i] << " ";

        if (girls.size() != 0) std::cout << std::endl;
        std::cout << std::endl;
    }
};
