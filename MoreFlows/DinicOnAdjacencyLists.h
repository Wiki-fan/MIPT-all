#pragma once
#include <vector>
#include <queue>
#include <limits>
#include <cstddef>

template<typename vtype = int, typename etype = int, typename FlowType = int>
class DinicOnAdjacencyLists {
public:



protected:
    std::vector<FlowType> distance;
    std::vector<vtype> begins;

    vtype vertexCount, source, target;
    const int INF = std::numeric_limits<FlowType>::max();

    virtual bool bfs() {
        std::queue<int> queue;
        begins.assign(vertexCount, 0);
        distance.assign(vertexCount, INF);
        distance[source] = 0;
        queue.push(source);

        while (!queue.empty()) {
            int u = queue.front();
            queue.pop();
            for (int i = 0; i < g[u].size(); ++i)
                if (((g[u][i].capacity - g[u][i].flow) > 0) && (distance[g[u][i].to] == INF)) {
                    distance[g[u][i].to] = distance[u] + 1;
                    queue.push(g[u][i].to);
                }
        }

        return distance[target] != INF;
    }

    FlowType dfs(vtype v, FlowType flow) {
        if (v == target || flow == 0)
            return flow;

        FlowType delta;
        for (int i = begins[v]; i < g[v].size(); ++i) {
            if ((distance[g[v][i].to] == distance[v] + 1)) {
                delta = dfs(g[v][i].to, std::min(flow, (g[v][i].capacity - g[v][i].flow)));
                if (delta != 0) {
                    g[v][i].flow += delta;
                    g[g[v][i].to][g[v][i].back].flow -= delta;
                    return delta;
                }
            }
            ++begins[v];
        }
        return 0;
    }

    size_t findMaxFlow(vtype source_, vtype target_) {
        size_t maxFlow = 0;
        source = source_;
        target = target_;

        while (bfs())
            while (int flow = dfs(source, INF))
                maxFlow += flow;
        return maxFlow;
    }

};
