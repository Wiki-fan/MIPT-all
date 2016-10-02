#pragma once
#include <string>
#include <iostream>
#include <cassert>
#include "network.h"
#include "dinic.h"

class A {
public:

    Network<uint, int, int> net;
    void solve() {
        read();
        if ((whiteValency == blackValency) && (whiteValency != 0)) {
            Dinic<uint, int, int> dinic;
            auto max_flow = dinic.findMaxFlow(&net);
            if ((max_flow == whiteValency) && (max_flow == blackValency))
                std::cout << "Valid";
            else {
                std::cout << "Invalid";
            }
        } else {
            std::cout << "Invalid";
        }
    }
private:

    int whiteValency = 0, blackValency = 0;
    int getValency(char elem) {
        int ans;
        switch (elem) {
            case 'H':
                ans = 1;
                break;
            case 'O':
                ans = 2;
                break;
            case 'N':
                ans = 3;
                break;
            case 'C':
                ans = 4;
                break;
            case '.':
                return 0;
            default:
                assert(false);
        }
        return ans;
    }

    void read() {
        int n, m;
        std::cin >> n >> m;
        net.init(n * m + 2, 0, 1);
        std::vector<std::vector<char> > H(n);
        for (auto&& row:H) {
            row.resize(m);
        }

        char d;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                do d = getchar();
                while ((d != '.') && (d != 'O') && (d != 'H') && (d != 'C') && (d != 'N'));
                H[i][j] = d;
            }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)

                if (H[i][j] != '.') {
                    if ((i + j) % 2) {
                        net.insertUndirectedEdge(net.getSource(), i * m + j + 2, getValency(H[i][j]));
                        auto lmb = [i, j, H,m,  this](int ni, int nj) {
                            if ((H[ni][nj] != '.')) net.insertDirectedEdge(i * m + j + 2, ni * m + nj + 2, 1);
                        };
                        if (i != 0) lmb(i - 1, j);
                        if (i != n - 1) lmb(i + 1, j);
                        if (j != m - 1) lmb(i, j + 1);
                        if (j != 0) lmb(i, j - 1);
                        whiteValency += getValency(H[i][j]);
                    } else {
                        net.insertUndirectedEdge(i * m + j + 2, net.getTarget(), getValency(H[i][j]));
                        blackValency += getValency(H[i][j]);
                    }
                }
    }
};