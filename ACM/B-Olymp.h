#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include "BaseTask.h"
#include "DinicOnAdjacencyLists.h"

class B : public BaseTask, public DinicOnAdjacencyLists<> {
public:

    void solve() override {
        read();

        if (bfs2()) {
            std::cout << -1 << std::endl;
        } else {
            int maxFlow = findMaxFlow(source, target);
            if (maxFlow > 0) {
                printAnswer();
            } else {
                std::cout << 0 << std::endl;
            }

        }
    }

private:

    enum Tile {
        WALL, PLACE, NOTHING
    };

    size_t dim;
    int n, m;
    std::vector<Tile> map;

    int getIndexByPart(int v) { return (map[v] == Tile::PLACE ? v + n * m : v); }
    int getCap(int a, int b) { return (a == b ? INF / 2 : 1); }
    int readCoord() {
        int a, b;
        std::cin >> a >> b;
        return (a - 1) * n + b - 1;
    }

    bool bfs2() {
        std::queue<int> queue;
        begins.assign(vertexCount, 0);
        distance.assign(vertexCount, INF);
        distance[source] = 0;
        queue.push(source);
        while (!queue.empty()) {
            int u = queue.front();
            queue.pop();
            for (int i = 0; i < g[u].size(); i++) {
                if (((g[u][i].capacity - g[u][i].flow) > 0) && (distance[g[u][i].to] == INF) && (map[g[u][i].to] == Tile::NOTHING)) {
                    distance[g[u][i].to] = distance[u] + 1;
                    queue.push(g[u][i].to);
                }
            }
        }
        return distance[target] != INF;
    }

    void read() override {
        int k, l;
        std::cin >> m >> n;
        dim = n * m;
        init(n * m * 2);
        map.resize(vertexCount, Tile::NOTHING);

        std::cin >> k >> l;
        for (int i = 0; i < k; i++) {
            map[readCoord()] = Tile::WALL;
        }
        for (int i = 0; i < l; i++) {
            map[readCoord()] = Tile::PLACE;
        }
        source = readCoord();
        target = readCoord();

        for (int i = 0; i < dim; i++) {
            map[i + dim] = map[i];
        }
        for (int i = 0; i < dim; i++) {
            if (map[i] == Tile::PLACE) {
                insertEdge(i, i + dim, 1);
            }
        }

        int p = -1;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                p++;
                if ((map[p] != Tile::WALL)) {
                    auto lmb = [this](int op, int np) {
                        if (map[np] != Tile::WALL)
                            insertEdge(getIndexByPart(op), np, getCap(map[op], map[np]));
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
        std::queue<int> queue;
        std::vector<bool> used(vertexCount, 0);
        std::vector<bool> answer(vertexCount, 0);
        queue.push(source);

        while (!queue.empty()) {
            int u = queue.front();
            queue.pop();
            used[u] = 1;
            for (int i = 0; i < g[u].size(); i++) {
                if ((g[u][i].capacity > 0) && (distance[g[u][i].to] == INF)) {
                    if (map[g[u][i].to] == Tile::NOTHING) {
                        answer[u % dim] = 1;
                    } else {
                        answer[(g[u][i].to) % dim] = 1;
                    }
                } else {
                    if ((g[u][i].capacity > 0) && (!used[g[u][i].to])) {
                        queue.push(g[u][i].to);
                        used[g[u][i].to] = 1;
                    }
                }
            }
        }

        int res = 0;
        for (int i = 0; i < dim; i++) {
            res += answer[i];
        }
        std::cout << res << std::endl;
        for (int i = 0; i < dim; i++) {
            if (answer[i]) {
                int a = i / n + 1;
                int b = i % n + 1;
                std::cout << a << ' ' << b << std::endl;
            }
        }
    }

};
