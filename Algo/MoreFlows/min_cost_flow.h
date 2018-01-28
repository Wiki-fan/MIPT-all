#pragma once
#include <vector>
#include <cstddef>
#include <deque>

// from emaxx
const int INF = 1000 * 1000 * 1000;

class MinCostFlow {
public:
    MinCostFlow(int n_):n(n_), g(n_) {}

    void insertEdge(int from, int to, int capacity, int cost) {
        Edge r1 = {to, capacity, cost, 0, g[to].size()};
        Edge r2 = {from, 0, -cost, 0, g[from].size()};
        g[from].push_back(r1);
        g[to].push_back(r2);
    }

    int getMaxCost(int desiredFlow) {
        int flow = 0, cost = 0;
        while (flow < desiredFlow) {
            std::vector<int> id(n, 0);
            std::vector<int> collectedCost(n, 0);
            std::deque<int> deque(n);
            std::vector<int> previous(n);
            std::vector<size_t> p_rib(n);
            deque.push_back(s);
            collectedCost[s] = 0;
            while (!deque.empty()) {
                int v = deque.front();
                id[v] = 2;
                deque.pop_front();
                for (size_t i = 0; i < g[v].size(); ++i) {
                    Edge& r = g[v][i];
                    if (r.flow < r.capacity && collectedCost[v] + r.cost > collectedCost[r.b]) {
                        collectedCost[r.b] = collectedCost[v] + r.cost;
                        if (id[r.b] == 0) {
                            deque.push_back(r.b);
                        } else if (id[r.b] == 2) {
                            deque.push_front(r.b);
                        }
                        id[r.b] = 1;
                        previous[r.b] = v;
                        p_rib[r.b] = i;
                    }
                }
            }
            if (collectedCost[t] == 0) break;
            int addflow = desiredFlow - flow;
            for (int v = t; v != s; v = previous[v]) {
                int pv = previous[v];
                size_t pr = p_rib[v];
                addflow = std::min(addflow, g[pv][pr].capacity - g[pv][pr].flow);
            }
            for (int v = t; v != s; v = previous[v]) {
                int pv = previous[v];
                size_t pr = p_rib[v], r = g[pv][pr].back;
                g[pv][pr].flow += addflow;
                g[v][r].flow -= addflow;
                cost += g[pv][pr].cost * addflow;
            }
            flow += addflow;
        }

        return cost;
    }

    int getMinCost(int desiredFlow) {
        int flow = 0, cost = 0;
        while (flow < desiredFlow) {
            std::vector<int> id(n, 0);
            std::vector<int> collectedCost(n, INF);
            std::deque<int> deque(n);
            std::vector<int> previous(n);
            std::vector<size_t> p_rib(n);
            deque.push_back(s);
            collectedCost[s] = 0;
            while (!deque.empty()) {
                int v = deque.front();
                id[v] = 2;
                deque.pop_front();
                for (size_t i = 0; i < g[v].size(); ++i) {
                    Edge& r = g[v][i];
                    if (r.flow < r.capacity && collectedCost[v] + r.cost < collectedCost[r.b]) {
                        collectedCost[r.b] = collectedCost[v] + r.cost;
                        if (id[r.b] == 0) {
                            deque.push_back(r.b);
                        } else if (id[r.b] == 2) {
                            deque.push_front(r.b);
                        }
                        id[r.b] = 1;
                        previous[r.b] = v;
                        p_rib[r.b] = i;
                    }
                }
            }
            if (collectedCost[t] == 0) break;
            int addflow = desiredFlow - flow;
            for (int v = t; v != s; v = previous[v]) {
                int pv = previous[v];
                size_t pr = p_rib[v];
                addflow = std::min(addflow, g[pv][pr].capacity - g[pv][pr].flow);
            }
            for (int v = t; v != s; v = previous[v]) {
                int pv = previous[v];
                size_t pr = p_rib[v], r = g[pv][pr].back;
                g[pv][pr].flow += addflow;
                g[v][r].flow -= addflow;
                cost += g[pv][pr].cost * addflow;
            }
            flow += addflow;
        }

        return cost;
    }
private:
    struct Edge {
        int b, capacity, cost, flow;
        size_t back;
    };

    int n, m;
    int s = 0, t = 1;
    std::vector<std::vector<Edge> > g;

};