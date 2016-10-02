#pragma once
#include "dinic.h"
#include "network.h"
#include <iostream>

template<typename vtype, typename etype>
class MaximalFlow {
public:
    void find() {
        while (true) {
            Network<vtype, etype, int> net;
            uint n;
            vtype s, t;
            std::cin >> n;
            if (n == 0)
                break;
            int cnt;
            std::cin >> s >> t >> cnt;

            net.init(n, s - 1, t - 1);
            for (int j = 0; j < cnt; ++j) {
                vtype u, v;
                int c;
                std::cin >> u >> v >> c;
                net.insertUndirectedEdge(u - 1, v - 1, c);
            }

            Dinic<vtype, etype, int> dinic;
            std::cout << dinic.findMaxFlow(&net) << std::endl;
        }
    }
};
