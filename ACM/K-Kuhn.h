#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include "BaseTask.h"

template<typename vtype = int, typename FlowType = int, typename CostType = int>
class K_Hungary : public BaseTask {
public:
    void solve() {
        read();
        find();
        printAnswer();
    }

private:
    const FlowType INF = std::numeric_limits<FlowType>::max();
    vtype n, m, e;
    std::vector<std::vector<int>> a;
    std::vector<bool> used;
    std::vector<vtype> matched, matched2;
    std::vector<CostType> wm;
    std::vector<std::pair< CostType, vtype>> wn;


    void read() {

        std::cin >> n >> m >> e;

        a.resize(n + 1, std::vector<CostType>(m + 1, 0));
        for (int i = 0; i < n; ++i) {
            CostType w;
            std::cin >> w;
            wn.push_back(std::make_pair(w,i));
        }
        std::sort(wn.begin(), wn.end());
        for (int i = 0; i < m; ++i) {
            CostType w;
            std::cin >> w;
            wm.push_back(w);
        }
        for (int i = 0; i < e; ++i) {
            vtype u, v;
            std::cin >> u >> v;
            a[u][v] = -(std::find_if(wn.begin(),
                                     wn.end(),
                                     [u](std::pair<CostType, vtype>& elem) {return elem.second == u - 1; })
                    ->first
                    + wm[v - 1]);
        }
        n = m = std::max(n, m);
    }
    CostType curMax;
    bool try_kuhn (int v, bool first) {
        if (used[v])  return false;
        used[v] = true;
        int curMax = 0;
        for (size_t i=0; i<a[v].size(); ++i) {
            int to = a[v][i];
            if (matched2[to] == -1 || try_kuhn (matched2[to], !first)) {
                matched2[to] = v;
                CostType myWeight = (first?wn[to]:wm[to]);
                if (myWeight > curMax) {
                    curMax = myWeight;
                    matched = matched2;
                }
                return true;
            }
        }
        return false;
    }

    void find() {
        for (int i = 0; i<n; ++i) {
            vtype v = wn[i].second;
            used.assign(n, false);
            curMax = 0;
            try_kuhn(v, false);
        }
    }

    void printAnswer() {
        for (int i = 0; i<matched.size(); ++i)
            std::cout <<
    }
};
