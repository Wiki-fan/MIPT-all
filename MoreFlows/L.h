#pragma once
#include <iostream>
#include "../Flow/Network.h"
#include "../Flow/Dinic.h"

template<typename vtype = int, typename etype = int, typename FlowType = int>
class L {
public:
    void solve() {
        read();
        {
            Dinic<> dinic;
            auto ans = dinic.findMaxFlow(&net);
            findWays();
            printResult();
        }
    }

private:
    Network<vtype, etype, FlowType> net;
    std::vector<std::vector<int>> a;
    std::vector<int> marked;
    std::vector<bool> visited;
    vtype n;

    void read() {
        std::cin >> n;
        net.init(2*n+2, 0, 1);
        a.resize(n, std::vector<int>(n));
        for (vtype i = 0; i < n; ++i)
            for (vtype j = 0; j < n; ++j)
                std::cin >> a[i][j];
        // транзитивное замыкание
        for (vtype k = 0; k < n; ++k)
            for (vtype i = 0; i < n; ++i)
                for (vtype j = 0; j < n; ++j)
                    if (a[i][k] && a[k][j])
                        a[i][j] = 1;
        // к стоку и истоку
        for (vtype i = 0; i < n; ++i) {
            net.insertDirectedEdge(net.getSource(), i + 2, 1);
            net.insertDirectedEdge(i + n + 2, net.getTarget(), 1);
        }
        //сами рёбра
        for (vtype i = 0; i < n; ++i)
            for (vtype j = 0; j < n; ++j)
                if (i != j && a[i][j])
                    net.insertDirectedEdge(i + 2, j + n + 2, 1);
    }

    void dfs(vtype v) {
        if (!visited[v]) {
            visited[v] = true;
            for (auto it = net.begin(v); it != net.end(); ++it) {
                if (!it.isDeleted())
                    dfs(it.getFinish());
            }
        }
    }

    void findWays() {
        visited.resize(net.getVertexCount(), false);
        marked.resize(net.getVertexCount(), false);
        visited[net.getSource()] = true;
        visited[net.getTarget()] = true;

        for (vtype i = 0; i < n; ++i) {
            for (auto e = net.begin(i + 2); e != net.end(); ++e)
                if (e.getFlow() == 1) {
                    marked[i + 2] = true;
                    e.markAsDeleted();
                } else {
                    e.markReverseAsDeleted();
                }
        }

        for (vtype i = 0; i < n; i++)
            if ((marked[i + 2] == false) && (visited[i + 2] == false))
                dfs(i + 2);
    }

    void printResult() {
        std::vector<int> ans;
        for (vtype i = 0; i<n; ++i)
            if (visited[i+2] && !visited[n+i+2])
                ans.push_back(i);
        std::cout << ans.size()<<std::endl;

        for (int a = 0; a<ans.size(); ++a)
            std::cout << ans[a]+1 <<' ';
    }


};
