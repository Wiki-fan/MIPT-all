#pragma once
#include "Network.h"
#include <climits>
#include <queue>
#include <memory>
#include <algorithm>
#include <iostream>

template<typename vtype, typename etype, typename FlowType>
class EdmondsKarp {
public:
    size_t findMaxFlow(Network<vtype, etype, FlowType>* net_) {
        size_t maxFlow = 0;
        net = net_;
        pred.resize(net->getVertexCount());
        while (bfs()) {
            FlowType df = std::numeric_limits<FlowType>::max();
            for (auto it = net->getEdgeByNum(pred[net->getTarget()]);
                 it != net->end(); it = net->getEdgeByNum(pred[it.getStart()])) {
                df = std::min(df, it.getResidualCapacity());
                //std::cout << it.getEdgeNumber() <<std::endl;
            }
            for (auto it = net->getEdgeByNum(pred[net->getTarget()]);
                 it != net->end(); it = net->getEdgeByNum(pred[it.getStart()])) {
                it.pushFlow(df);
            }

            maxFlow += df;
        }
        return maxFlow;
    }
private:
    std::vector<etype> pred;
    Network<vtype, etype, FlowType>* net;

    bool bfs() {
        std::queue<vtype> q;
        q.push(net->getSource());
        pred.assign(net->getVertexCount(), -1);
        while (!q.empty()) {
            vtype u = q.front();
            q.pop();
            for (auto it = net->begin(u); it != net->end(); ++it) {
                if (pred[it.getFinish()] == -1
                        && it.getFinish() != net->getSource()
                        && it.getResidualCapacity() > 0) {
                    pred[it.getFinish()] = it.getEdgeNumber();
                    q.push(it.getFinish());
                }
            }
        }
        return pred[net->getTarget()] != -1;
    }
};
