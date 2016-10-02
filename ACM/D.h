#pragma once
#include <cmath>
#include <iostream>
#include "dinic.h"
#include "network.h"

class D {
public:
    void solve() {
        read();
        Dinic<uint, int, int> dinic;
        std::cout << (net.getVertexCount() - 2)/2 - dinic.findMaxFlow(&net);
    }
private:
    Network<uint, int, int> net;

    int distance(int x1, int y1, int x2, int y2) {
        return (std::abs(x1 - x2) + std::abs(y1 - y2));
    }

    void read() {
        int n;
        std::cin >> n;
        net.init(2 * n + 2, 0, 1);
        std::vector<int> x_start(net.getVertexCount(), 0);
        std::vector<int> y_start(net.getVertexCount(), 0);
        std::vector<int> x_finish(net.getVertexCount(), 0);
        std::vector<int> y_finish(net.getVertexCount(), 0);
        std::vector<int> time_start(net.getVertexCount(), 0);
        std::vector<int> time_finish(net.getVertexCount(), 0);

        int th, ts, tm;
        for (int i = 2; i < n + 2; i++) {
            std::cin >> th;
            getchar();
            std::cin >> ts;
            tm = 60*th + ts;
            std::cin >> x_start[i] >> y_start[i] >> x_finish[i] >> y_finish[i];

            time_finish[i] = tm + distance(x_start[i], y_start[i], x_finish[i], y_finish[i]);
            time_start[i] = tm;
            for (int j = 2; j < i; j++) {
                if (time_finish[j] + distance(x_start[i], y_start[i], x_finish[j], y_finish[j]) < time_start[i]) {
                    net.insertDirectedEdge(j, n + i, 1);
                }
            }
        }
        for (int i = 0; i < n; i++) {
            net.insertDirectedEdge(net.getSource(), i + 2, 1);
        }
        for (int i = 0; i < n; i++) {
            net.insertDirectedEdge(n + i + 2, net.getTarget(), 1);
        }
    }
};