#pragma once
#include <iostream>
#include "Network.h"
#include "dinic.h"

template<typename vtype, typename etype, typename FlowType>
class VeryChangingFlow {
    Network<vtype, etype, FlowType> net;

public:
    void solve() {
        read();
    }

private:
    void read() {
        uint n, m;
        std::cin >> n >> m;
        net.init(n, 0, n - 1);
        for (uint i = 0; i < m; ++i) {
            uint x, y, c;
            std::cin >> x >> y >> c;
            net.insertDirectedEdge(x - 1, y - 1, c);
        }
        dinic<vtype, etype, FlowType> dinic;
        std::cout << dinic.findMaxFlow(&net) << std::endl;
        uint t;
        std::cin >> t;
        for (uint i = 0; i < t; ++i) {
            uint op, x, y;
            std::cin >> op >> x >> y;
            --x;
            --y;
            //net.insertDirectedEdge(x, y, 1);
            for (auto it = net.begin(x); it != net.end(); ++it) {
                if (it.getFinish() == y) {
                    if (op == 1)
                        it.pushCapacity(1);
                    else it.pushCapacity(-1);
                    break;
                }
            }
            dinic < vtype, etype, FlowType > dinic2;
            net.resetBegins();
            net.resetFlow();
            std::cout << dinic2.findMaxFlow(&net) << std::endl;
        }

    }


};

