#pragma once

#include <vector>
#include <queue>
#include <limits>
#include "CostNetwork.h"

template<typename vtype = int, typename etype = int, typename FlowType = int, typename CostType = int, typename EST = int>
class MinCostOnDijkstra {
public:
    CostNetwork<vtype, etype, FlowType, CostType, EST>* net;

    const FlowType INF = std::numeric_limits<vtype>::max();


    // First is flow that can actually be achieved, second is its cost.
    std::pair<FlowType, CostType> findMinCostFlow(CostNetwork<vtype, FlowType, CostType>* net_, FlowType desiredFlow) {
        net = net_;
        CostType cost = 0;
        FlowType flow = 0;
        while (flow < desiredFlow) {
            auto ans = MinCostInnerLoop(false);
            if (ans.second <= 0 && ans.first <= 0)
                return std::make_pair(-1, -1);
            flow += ans.first;
            cost += ans.second;
        }

        return std::make_pair(flow, cost);
    }

    // First is flow that can actually be achieved, second is its cost.
    std::pair<FlowType, CostType> findMinCostMaxFlow(CostNetwork<vtype, FlowType, CostType>* net_) {
        net = net_;
        CostType cost = 0;
        FlowType flow = 0;
        while (true) {
            auto ans = MinCostInnerLoop(true);
            if (ans.second <= 0)
                break;
            flow += ans.first;
            cost += ans.second;
        }

        return std::make_pair(flow, cost);
    }

    //const std::vector<FlowType>& getDistance() { return distance; }

private:

    std::pair<FlowType, CostType> MinCostInnerLoop(bool anyFlow)
    {
        CostType cost = 0;
        FlowType flow = 0;
        std::vector<vtype> used(net->getVertexCount(), 0);
        std::vector<FlowType> distance(net->getVertexCount(), INF);
        std::vector<FlowType> potential(net->getVertexCount(), INF);
        std::vector<vtype> parent(net->getVertexCount(), -1);
        std::vector<vtype> parent_edge(net->getVertexCount(), -1); // Edge by which we go to the parent

        // count potentials
        bool upgraded;
        do {
            upgraded = false;
            for (vtype i = 0; i < net->getVertexCount(); ++i)
                for (auto it = net->begin(i); it != net->end(); ++it) {
                    if (it.getCapacity() == 0)
                        continue;
                    FlowType c = potential[i] + it.getCost();
                    vtype v = it.getFinish();
                    if (c < potential[v]) {
                        potential[v] = c;
                        upgraded = true;
                    }
                }
        } while (upgraded);

        // first is negative distance, second is vertex
        std::priority_queue <std::pair<int, vtype>> queue;
        queue.push(std::make_pair(0, net->getSource()));
        distance[net->getSource()] = 0;
        while (!queue.empty()) {
            int d = -queue.top().first;
            int u = queue.top().second;
            queue.pop();
            if (d > distance[u]) continue;

            for (auto it = net->begin (u); it != net->end(); ++it) {
                int td = distance[it.getStart()] + it.getCost() + potential[it.getStart()] - potential[it.getFinish()];
                if (it.getResidualCapacity() != 0 && td < distance[it.getFinish()]) {
                    queue.push(std::make_pair(-td, it.getFinish()));
                    distance[it.getFinish()] = td;
                    parent[it.getFinish()] = u;
                    parent_edge[it.getFinish()] = it.getEdgeNumber();
                }
            }
        }
        if (distance[net->getTarget()] == INF)
            return std::make_pair(-1, -1);

        for (vtype i = 0; i<net->getVertexCount(); ++i)
            potential[i] += distance[i];


        FlowType delta = INF;
        for (vtype v = net->getTarget(); v != net->getSource(); v = parent[v]) {
            vtype pe = parent_edge[v];
            delta = std::min(delta, net->getEdgeByNum(pe).getResidualCapacity());
            cost += net->getEdgeByNum(pe).getCost() * delta;
        }

        if (anyFlow && cost >= 0) {
            return std::make_pair(-1, -1);
        }

        for (vtype v = net->getTarget(); v != net->getSource(); v = parent[v]) {
            vtype pe = parent_edge[v];
            net->getEdgeByNum(pe).pushFlow(delta);

        }
        flow += delta;

        return std::make_pair(flow, cost);
    }

};
