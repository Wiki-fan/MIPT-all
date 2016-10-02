#pragma once
#include <iostream>
#include "network.h"
#include "dinic.h"

template<typename vtype, typename etype, typename FlowType>
class ChangingFlow : public Dinic<vtype, etype, FlowType> {
    Network<vtype, etype, int> net;
public:
    void solve() {
        read();
    }

private:

    using Dinic<vtype, etype, FlowType>::distance;
    using Dinic<vtype, etype, FlowType>::dfs;
    using Dinic<vtype, etype, FlowType>::bfs;
    using Dinic<vtype, etype, FlowType>::performFind;
    using Dinic<vtype, etype, FlowType>::findMaxFlow;
    using Dinic<vtype, etype, FlowType>::Infinity;

    int dfsModify(int v, int flow) {

        if (flow == 0 || v == net.getTarget())
            return flow;
        for (auto it = net.begin(v); it != net.end(); ++it) {
            if (it.isDeleted()) {
                it.shiftBeginToNext();
            } else {

                if (distance[it.getFinish()] != distance[it.getStart()] + 1)
                    continue;
                int pushed = dfsModify(it.getFinish(), std::min(flow, it.getResidualCapacity()));
                if (pushed != 0) {
                    it.pushFlow(pushed);
                    return pushed;
                }
                it.shiftBeginToNext();
            }

        }
        return 0;
    }

    size_t increaseFlow(vtype u, vtype v) {
        net.insertDirectedEdge(u, v, 1);
        return performFind();
    }

    int decreaseFlow(int u, int v) {
        int smallFlow = 0;
        int max_flow = 0;
        net.setSource(u);
        net.setTarget(v);
        typename Network<vtype, etype, int>::NetworkEdgeIterator it;
        for (it = net.begin(u); it != net.end() && it.getFinish() != v; ++it)
            it.markAsDeleted();
        if (bfs()) {
            int flow = dfsModify(net.getSource(), Infinity);
            if (flow == 0) {
                it.pushCapacity(-1);
                it.pushFlow(-1);
                max_flow--;
                if (max_flow < 0) {
                    max_flow = 0;
                }
            }
        } else {
            it.pushCapacity(-1);
            it.pushFlow(-1);
            max_flow--;
            if (max_flow < 0) {
                max_flow = 0;
            }

        }
        it.unmarkAsDeleted();
        net.setSource(0);
        net.setTarget(net.getVertexCount() - 1);
        return max_flow;
    }

    void read() {
        uint n, m;
        std::cin >> n >> m;
        net.init(n, 0, n - 1);
        for (int i = 0; i < m; ++i) {
            uint x, y, c;
            std::cin >> x >> y >> c;
            net.insertRemainEdge(x - 1, y - 1, c);
        }
        std::cout << findMaxFlow(&net) << std::endl;
        uint t;
        std::cin >> t;
        for (int i = 0; i < t; ++i) {
            uint op, x, y;
            std::cin >> op >> x >> y;
            --x;
            --y;
            if (op == 1) {
                std::cout << increaseFlow(x, y) << std::endl;
            } else if (op == 2) {
                std::cout << decreaseFlow(x, y) << std::endl;
            }
        }

    }
};
