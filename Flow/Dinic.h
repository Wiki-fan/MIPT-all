#pragma once
#include "Network.h"
#include <climits>
#include <queue>
#include <memory>

template<typename vtype, typename etype, typename FlowType>
class Dinic {
public:
    size_t findMaxFlow(Network<vtype, etype, FlowType>* net_) {
        net = net_;
        d.resize(net->getVertexCount());
        return performFind();
    }
protected:
    std::vector<FlowType> d; // Shortest paths from s to vertex.

    Network<vtype, etype, FlowType>* net;

    size_t performFind() {
        size_t maxFlow = 0;
        while (bfs())
            while (int flow = dfs(net->getSource(), INT_MAX))
                maxFlow += flow;
        return maxFlow;
    }

    bool bfs() {
        std::queue<vtype> q;
        d.assign(d.size(), INT_MAX);
        net->resetDeleted();
        d[net->getSource()] = 0;
        q.push(net->getSource());
        while (!q.empty()) {
            vtype u = q.front();
            q.pop();
            for (auto it = net->begin(u); it != net->end(); ++it) {
                if (it.getResidualCapacity() > 0 && d[it.getFinish()] == INT_MAX) {
                    d[it.getFinish()] = d[u] + 1;
                    q.push(it.getFinish());
                }
            }
        }
        return d[net->getTarget()] != INT_MAX;
    }

    // Blocking flow search.
    FlowType dfs(vtype v, FlowType flow) {
        if (flow == 0 || v == net->getTarget())
            return flow;
        for (auto it = net->begin(v); it != net->end(); ++it) {
            if (d[it.getFinish()] != d[it.getStart()] + 1)
                continue;
            int pushed = dfs(it.getFinish(), std::min(flow, it.getResidualCapacity()));
            if (pushed != 0) {
                it.pushFlow(pushed);
                return pushed;
            }
            it.remove();
        }
        return 0;
    }
};
