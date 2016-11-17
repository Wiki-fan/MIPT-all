#pragma once

// from acm from stanford
#include <cmath>
#include <vector>
#include <iostream>
#include <limits>

const long long int INF = std::numeric_limits<long long int>::max() / 4;

struct Edge
{
    int64_t cap, flow, cost;
};
class MinCostMaxFlow {
public:
    MinCostMaxFlow(int N) :
            N(N), edges(N, std::vector<Edge>(N)),
            found(N), dist(N), pi(N), width(N), dad(N) {}

    void AddEdge(int from, int to, int64_t cap, int64_t cost) {
        edges[from][to].cap = cap;
        edges[from][to].cost = cost;
    }

    std::pair<int64_t, int64_t> GetMaxFlow(int s, int t) {
        int64_t totflow = 0, totcost = 0;
        while (int64_t amt = Dijkstra(s, t)) {
            totflow += amt;
            for (int x = t; x != s; x = dad[x].first) {
                if (dad[x].second == 1) {
                    edges[dad[x].first][x].flow += amt;
                    totcost += amt * edges[dad[x].first][x].cost;
                } else {
                    edges[x][dad[x].first].flow -= amt;
                    totcost -= amt * edges[x][dad[x].first].cost;
                }
            }
        }
        return std::make_pair(totflow, totcost);
    }
private:
    int N;
    std::vector<std::vector<Edge>> edges;
    std::vector<int> found;
    std::vector<long long int> dist, pi, width;
    std::vector<std::pair<int,int>> dad;

    void Relax(int s, int k, int64_t cap2, int64_t cost, int dir) {
        int64_t val = dist[s] + pi[s] - pi[k] + cost;
        if (cap2 && val < dist[k]) {
            dist[k] = val;
            dad[k] = std::make_pair(s, dir);
            width[k] = std::min<uint64_t>(cap2, width[s]);
        }
    }

    int64_t Dijkstra(int s, int t) {
        fill(found.begin(), found.end(), false);
        fill(dist.begin(), dist.end(), INF);
        fill(width.begin(), width.end(), 0);
        dist[s] = 0;
        width[s] = INF;

        while (s != -1) {
            int best = -1;
            found[s] = true;
            for (int k = 0; k < N; k++) {
                if (found[k]) continue;
                Relax(s, k, edges[s][k].cap - edges[s][k].flow, edges[s][k].cost, 1);
                Relax(s, k, edges[k][s].flow, -edges[k][s].cost, -1);
                if (best == -1 || dist[k] < dist[best]) best = k;
            }
            s = best;
        }

        for (int k = 0; k < N; k++)
            pi[k] = std::min(pi[k] + dist[k], INF);
        return width[t];
    }

};

// BEGIN CUT
// The following code solves UVA problem #10594: Data Flow

/*int main() {
    int N, M;

    while (scanf("%d%d", &N, &M) == 2) {
        std::vector<std::vector<long long>> v(M, std::vector<long long int>(3));
        for (int i = 0; i < M; i++)
            scanf("%int64_td%int64_td%int64_td", &v[i][0], &v[i][1], &v[i][2]);
        int64_t D, K;
        scanf("%int64_td%int64_td", &D, &K);

        MinCostMaxFlow mcmf(N+1);
        for (int i = 0; i < M; i++) {
            mcmf.AddEdge(int(v[i][0]), int(v[i][1]), K, v[i][2]);
            mcmf.AddEdge(int(v[i][1]), int(v[i][0]), K, v[i][2]);
        }
        mcmf.AddEdge(0, 1, D, 0);

        pair<int64_t, int64_t> res = mcmf.GetMaxFlow(0, N);

        if (res.first == D) {
            printf("%int64_td\n", res.second);
        } else {
            printf("Impossible.\n");
        }
    }

    return 0;
}
*/