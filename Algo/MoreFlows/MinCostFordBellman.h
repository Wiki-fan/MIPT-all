#pragma once

#include <vector>
#include <queue>
#include <limits>

template<typename vtype = int, typename FlowType = int, typename CostType = int>
class MinCostFB {
public:
    struct Edge {
        vtype to, back;
        FlowType flow, capacity;
        CostType cost;
        int number;
        Edge(vtype to_, FlowType capacity_, CostType cost_, FlowType flow_, int num, vtype back_) {
            to = to_;
            capacity = capacity_;
            cost = cost_;
            flow = flow_;
            number = num;
            back = back_;
        }
    };

public:
    std::vector<std::vector<Edge>> g;
    const FlowType INF = std::numeric_limits<vtype>::max();

    void init(vtype n) {
        g.resize(n);
        vertex_count = n;
    }

    void insertEdge(vtype from_, vtype to_, FlowType capacity_, CostType cost_, int num) {
        vtype a = g[to_].size();
        vtype b = g[from_].size();
        Edge e1 = {to_, capacity_, cost_, 0, num, a};
        Edge e2 = {from_, 0, -cost_, 0, num, b};
        g[from_].push_back(e1);
        g[to_].push_back(e2);
    }

    // First is flow that can actually be achieved, second is its cost.
    std::pair<FlowType, CostType> findMinCostMaxFlow(FlowType desiredFlow, vtype source, vtype target) {
        cost = 0;
        flow = 0;
        while (flow < desiredFlow) {
            std::vector<vtype> used(vertex_count, 0);
            std::vector<FlowType> distance(vertex_count, INF);
            std::queue<vtype> queue;
            std::vector<vtype> parent(vertex_count, -1);
            std::vector<vtype> parent_edge(vertex_count, -1); // Edge by which we go to the parent

            queue.push(source);
            queue.push(source);
            distance[source] = 0;
            used[source] = true;
            while (!queue.empty()) {
                vtype vertex = queue.front();
                queue.pop();
                used[vertex] = false;
                for (vtype i = 0; i < g[vertex].size(); ++i) {
                    if (g[vertex][i].capacity <= g[vertex][i].flow)
                        continue;

                    if (distance[g[vertex][i].to] > distance[vertex] + g[vertex][i].cost) {
                        distance[g[vertex][i].to] = distance[vertex] + g[vertex][i].cost;
                        parent[g[vertex][i].to] = vertex;
                        parent_edge[g[vertex][i].to] = i;

                        if (!used[g[vertex][i].to]) {
                            queue.push(g[vertex][i].to);
                            used[g[vertex][i].to] = true;
                        }
                    }
                }
            }

            if (distance[target] == INF)
                return std::make_pair(-1, 0);

            FlowType delta = INF;
            for (vtype v = target; v != source; v = parent[v]) {
                vtype pv = parent[v];
                vtype pe = parent_edge[v];
                delta = std::min(delta, g[pv][pe].capacity - g[pv][pe].flow);
            }

            for (vtype v = target; v != source; v = parent[v]) {
                vtype pv = parent[v];
                vtype pe = parent_edge[v];
                vtype r = g[pv][pe].back;
                g[pv][pe].flow += delta;
                g[v][r].flow -= delta;
                cost += g[pv][pe].cost * delta;
            }
            flow += delta;
        }
        return std::make_pair(flow, cost);
    }

private:

    CostType cost;
    FlowType flow;
    vtype vertex_count;
};
