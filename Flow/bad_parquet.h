#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "network.h"
#include "dinic.h"

class BadParquet {
public:

    void solve() {
        read();

        if (2 * A > B) {
            Dinic<uint, int, int> dinic;
            size_t max_flow = dinic.findMaxFlow(&net);
            std::cout << ((missing_tiles_count - max_flow * 2) * A + max_flow * B);
        } else {
            std::cout << missing_tiles_count * A;
        }
    }

private:
    uint M, N, A, B;
    uint missing_tiles_count = 0;
    Network<uint, int, int> net;

    void read() {
        std::cin >> N >> M >> B >> A;
        std::string str;
        std::vector<std::string> floor;
        net.init(N * M + 2, 0, 1);
        // Read floor to matrix.
        for (int i = 0; i < N; i++) {
            std::cin >> str;
            floor.push_back(str);
        }
        // Make graph.
        for (uint i = 0; i < N; i++)
            for (uint j = 0; j < M; j++)
                if (floor[i][j] == '*') {
                    if ((i + j) % 2) {
                        net.insertDirectedEdge(net.getSource(), i * M + j + 2, 1);

                        auto lmb = [i, j, floor, this](int ni, int nj) {
                            if (floor[ni][nj] == '*') net.insertDirectedEdge(i * M + j + 2, ni * M + nj + 2, 1);
                        };
                        if (i != 0) lmb(i - 1, j);
                        if (i != N - 1) lmb(i + 1, j);
                        if (j != M - 1) lmb(i, j + 1);
                        if (j != 0) lmb(i, j - 1);
                    } else {
                        net.insertDirectedEdge(i * M + j + 2, net.getTarget(), 1);
                    }
                    ++missing_tiles_count;
                }
    }
};
