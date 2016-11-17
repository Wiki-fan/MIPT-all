#pragma once
#include <string>
#include <iostream>
#include "../Flow/Network.h"
#include "../Flow/Dinic.h"
#include "BaseTask.h"

class A : public BaseTask {
public:
    Network<> net;

    void solve() override {
        read();
        printAnswer();
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
                return -1;
        }
        return ans;
    }

    void read() override {
        int n, m;
        std::cin >> n >> m;
        net.init(n * m + 2, 0, 1);

        std::vector<std::vector<char> > formula(n);
        for (auto&& row:formula) {
            row.resize(m);
        }

        char d;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                do {
                    d = getchar();
                } while (getValency(d) == -1);
                formula[i][j] = d;
            }

        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)

                if (formula[i][j] != '.') {
                    if ((i + j) % 2) {
                        net.insertUndirectedEdge(net.getSource(), i * m + j + 2, getValency(formula[i][j]));
                        auto lmb = [i, j, formula, m, this](int ni, int nj) {
                            if ((formula[ni][nj] != '.')) net.insertDirectedEdge(i * m + j + 2, ni * m + nj + 2, 1);
                        };
                        if (i != 0) lmb(i - 1, j);
                        if (i != n - 1) lmb(i + 1, j);
                        if (j != m - 1) lmb(i, j + 1);
                        if (j != 0) lmb(i, j - 1);
                        whiteValency += getValency(formula[i][j]);
                    } else {
                        net.insertUndirectedEdge(i * m + j + 2, net.getTarget(), getValency(formula[i][j]));
                        blackValency += getValency(formula[i][j]);
                    }
                }
    }

    void printAnswer() override {
        if ((whiteValency == blackValency) && (whiteValency != 0)) {
            Dinic<> dinic;
            auto max_flow = dinic.findMaxFlow(&net);
            if (max_flow == whiteValency)
                std::cout << "Valid";
            else
                std::cout << "Invalid";
        } else {
            std::cout << "Invalid";
        }
    }
};
