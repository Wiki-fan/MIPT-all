#pragma once
#include <iostream>
#include "BaseTask.h"
#include "../Flow/Network.h"
#include "../Flow/Dinic.h"

template<typename vtype = int, typename etype = int, typename FlowType = int>
class C : public BaseTask {
public:

    void solve() override{
        read();
        printAnswer();
    }

private:
    const FlowType INF = 1000000000;

    int n;
    Dinic<vtype, etype, FlowType> fb;
    Network<vtype, etype, FlowType> net;

    const int maxUsefulness = 1001;

    void read() override
    {
        std::cin >>n;
        net.init(n+2, 0, 1);

        for (vtype i = 0; i<n; ++i) {
            int usefulness;
            std::cin >> usefulness;
            net.insertDirectedEdge(net.getSource(), i+2, -usefulness+maxUsefulness);
        };

        for (vtype i = 0; i<n; ++i) {
            vtype k;
            std::cin >> k;
            for (vtype j = 0; j < k; ++j) {
                vtype dep;
                std::cin >> dep;
                net.insertDirectedEdge(dep-1+2, i+2, INF);
            }
            net.insertDirectedEdge(i+2, net.getTarget(), maxUsefulness);
        }

    }

    void printAnswer() override {
        auto ans = fb.findMaxFlow(&net);
        int ret = -(ans-maxUsefulness*n);

        std::cout << ret;

    }

};
