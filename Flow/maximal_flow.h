#pragma once
//#include "../stdafx.h"
#include "Dinic.h"
#include "Network.h"
#include <iostream>

class MaximalFlow {
public:
    void find() {
        while (true) {
            Network<uint, int, int> net;
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

            Dinic<uint, int, int> dinic;
            std::cout << dinic.findMaxFlow(&net) << std::endl;
        }
    }
};
