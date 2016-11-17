#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include "BaseTask.h"
#include "../Flow/Network.h"
#include "../Flow/Dinic.h"

template<typename vtype = int, typename etype = int, typename FlowType = int>
class B : public BaseTask, public Dinic<vtype, etype, FlowType> {
public:

    void solve() override {
        read();

        if (bfs2()) {
            std::cout << -1 << std::endl;
        } else {
            int maxFlow = findMaxFlow(&net);
            if (maxFlow > 0) {
                printAnswer();
            } else {
                std::cout << 0 << std::endl;
            }
        }
    }

private:

    using Dinic<vtype, etype, FlowType>::distance;
    using Dinic<vtype, etype, FlowType>::dfs;
    using Dinic<vtype, etype, FlowType>::bfs;
    using Dinic<vtype, etype, FlowType>::performFind;
    using Dinic<vtype, etype, FlowType>::findMaxFlow;
    using Dinic<vtype, etype, FlowType>::INF;

    enum Tile {
        WALL, PLACE, NOTHING
    };

    vtype dim;
    vtype n, m;
    std::vector<Tile> map;
    Network<vtype, etype, int> net;

    vtype getIndexByPart(vtype v) { return (map[v] == Tile::PLACE ? v + n * m : v); }
    vtype getCap(vtype a, vtype b) { return (a == b ? INF / 2 : 1); }
    vtype readCoord() {
        vtype a, b;
        std::cin >> a >> b;
        return (a - 1) * n + b - 1;
    }

    bool bfs2() {
        std::queue<vtype> q;
        distance.resize(net.getVertexCount(), INF);
        net.resetBegins();
        distance[net.getSource()] = 0;
        q.push(net.getSource());

        while (!q.empty()) {
            vtype u = q.front();
            q.pop();
            for (auto it = net.begin(u); it != net.end(); ++it) {
                if (it.getResidualCapacity() > 0 && distance[it.getFinish()] == INF && map[it.getFinish()] == Tile::NOTHING) {
                    distance[it.getFinish()] = distance[u] + 1;
                    q.push(it.getFinish());
                }
            }
        }

        return distance[net.getTarget()] != INF;
    }

    void read() override {
        vtype k, l;
        std::cin >> m >> n;
        dim = n * m;
        net.init(n * m * 2, -1, -1);
        map.resize(net.getVertexCount(), Tile::NOTHING);

        std::cin >> k >> l;
        for (vtype i = 0; i < k; ++i) {
            map[readCoord()] = Tile::WALL;
        }
        for (vtype i = 0; i < l; ++i) {
            map[readCoord()] = Tile::PLACE;
        }

        net.setSource(readCoord());
        net.setTarget(readCoord());

        for (vtype i = 0; i < dim; ++i) {
            map[i + dim] = map[i];
        }
        for (vtype i = 0; i < dim; ++i) {
            if (map[i] == Tile::PLACE) {
                net.insertDirectedEdge(i, i + dim, 1);
            }
        }

        vtype p = -1;
        for (vtype i = 0; i < m; ++i) {
            for (vtype j = 0; j < n; ++j) {
                ++p;
                if ((map[p] != Tile::WALL)) {
                    auto lmb = [this](int op, int np) {
                        if (map[np] != Tile::WALL)
                            net.insertDirectedEdge(getIndexByPart(op), np, getCap(map[op], map[np]));
                    };
                    if (i != 0) lmb(p, p - n);
                    if (i != m - 1) lmb(p, p + n);
                    if (j != n - 1) lmb(p, p + 1);
                    if (j != 0) lmb(p, p - 1);
                }
            }
        }
    }

    void printAnswer() override {
        std::queue<vtype> q;
        std::vector<bool> used(net.getVertexCount(), 0);
        std::vector<bool> answer(net.getVertexCount(), 0);
        q.push(net.getSource());
        while (!q.empty()) {
            vtype u = q.front();
            q.pop();
            used[u] = true;
            for (auto it = net.begin(u); it != net.end(); ++it) {
                if (it.getCapacity() > 0 && distance[it.getFinish()] == INF) {
                    if (map[it.getFinish()] == Tile::NOTHING) {
                        answer[u % dim] = 1;
                    } else {
                        answer[it.getFinish() % dim] = 1;
                    }
                } else {
                    if (it.getCapacity() > 0 && (!used[it.getFinish()])) {
                        q.push(it.getFinish());
                        used[it.getFinish()] = true;
                    }
                }
            }
        }

        int res = 0;
        for (vtype i = 0; i < dim; ++i) {
            res += answer[i];
        }

        std::cout << res << std::endl;
        for (vtype i = 0; i < dim; ++i) {
            if (answer[i]) {
                int a = i / n + 1;
                int b = i % n + 1;
                std::cout << a << ' ' << b << std::endl;
            }
        }
    }

};
