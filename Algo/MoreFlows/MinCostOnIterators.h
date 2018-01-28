#pragma once

#include <vector>
#include <queue>
#include <limits>
#include "CostNetwork.h"

template<typename vtype = int, typename etype = int, typename FlowType = int, typename CostType = int, typename EST = int>
class MinCostOnIterators {
public:
    CostNetwork<vtype, etype, FlowType, CostType, EST>* net;
    
    const FlowType INF = std::numeric_limits<vtype>::max();


    // First is flow that can actually be achieved, second is its cost.
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
        std::queue<vtype> queue;
        std::vector<vtype> parent(net->getVertexCount(), -1);
        std::vector<vtype> parent_edge(net->getVertexCount(), -1); // Edge by which we go to the parent

        queue.push(net->getSource());
        queue.push(net->getSource());
        distance[net->getSource()] = 0;
        used[net->getSource()] = true;
        while (!queue.empty()) {
            vtype vertex = queue.front();
            queue.pop();
            used[vertex] = false;
            for (auto it = net->begin(vertex); it != net->end(); ++it) {
                if (it.getResidualCapacity() <= 0)
                    continue;

                if (distance[it.getFinish()] > distance[vertex] + it.getCost()) {
                    distance[it.getFinish()] = distance[vertex] + it.getCost();
                    parent[it.getFinish()] = vertex;
                    parent_edge[it.getFinish()] = it.getEdgeNumber(); // = i TODO

                    if (!used[it.getFinish()]) {
                        queue.push(it.getFinish());
                        used[it.getFinish()] = true;
                    }
                }
            }
        }

        if (distance[net->getTarget()] == INF)
            return std::make_pair(-1, -1);

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
