#pragma once
#include "Network.h"
#include <queue>

template<typename vtype, typename etype, typename FlowType>
class dinic {
public:
    size_t findMaxFlow(Network<vtype, etype, FlowType>* net_) {
        net = net_;
        distance.resize(net->getVertexCount());
        return performFind();
    }
protected:
    const FlowType Infinity = std::numeric_limits<FlowType>::max();

    std::vector<FlowType> distance; // Shortest paths from s to vertex.

    Network<vtype, etype, FlowType>* net;

    size_t performFind() {
        size_t maxFlow = 0;
        while (bfs())
            while (int flow = dfs(net->getSource(), Infinity))
                maxFlow += flow;
        return maxFlow;
    }

    bool bfs() {
        std::queue<vtype> q;
        distance.assign(distance.size(), Infinity);
        net->resetBegins();
        distance[net->getSource()] = 0;
        q.push(net->getSource());
        while (!q.empty()) {
            vtype u = q.front();
            q.pop();
            for (auto it = net->begin(u); it != net->end(); ++it) {
                if (it.getResidualCapacity() > 0 && distance[it.getFinish()] == Infinity) {
                    distance[it.getFinish()] = distance[u] + 1;
                    q.push(it.getFinish());
                }
            }
        }
        return distance[net->getTarget()] != Infinity;
    }

    // Blocking flow search.
    FlowType dfs(vtype v, FlowType flow) {
        if (flow == 0 || v == net->getTarget())
            return flow;
        for (auto it = net->begin(v); it != net->end(); ++it) {
            if (distance[it.getFinish()] != distance[it.getStart()] + 1)
                continue;
            int pushed = dfs(it.getFinish(), std::min(flow, it.getResidualCapacity()));
            if (pushed != 0) {
                it.pushFlow(pushed);
                return pushed;
            }
            it.shiftBeginToNext();
        }
        return 0;
    }
};
