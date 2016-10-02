#pragma once

#include <vector>
#include <limits>

class Hungary {
public:
    std::vector<std::vector<int>>* a;

    void find(std::vector<std::vector<int>>* g) {
        a = g;
        // Raw and column potenital.
        rawP.resize(n + 1);
        colP.resize(m + 1);
        pairs.resize(m + 1); // Matching.
        way.resize(m + 1); // Array of predecessors.
        for (int i = 1; i <= n; ++i) {
            pairs[0] = i;
            int freeCol = 0;
            std::vector<int> minv(m + 1, INF);
            std::vector<char> used(m + 1, false);
            do {
                used[freeCol] = true;
                int i0 = pairs[freeCol], delta = INF, colWithMin;
                for (int j = 1; j <= m; ++j)
                    if (!used[j]) {
                        int cur = (*a)[i0][j] - rawP[i0] - colP[j];
                        if (cur < minv[j]) {
                            minv[j] = cur;
                            way[j] = freeCol;
                        }
                        if (minv[j] < delta) {
                            delta = minv[j];
                            colWithMin = j;
                        }
                    }
                for (int j = 0; j <= m; ++j)
                    if (used[j])
                        rawP[pairs[j]] += delta, colP[j] -= delta;
                    else
                        minv[j] -= delta;
                freeCol = colWithMin;
            } while (pairs[freeCol] != 0);

            do {
                int colWithMin = way[freeCol];
                pairs[freeCol] = pairs[colWithMin];
                freeCol = colWithMin;
            } while (freeCol != 0);
        }
    }

private:
    int n, m;

    std::vector<int> rawP, colP, pairs, way;
    const static int INF;

    void recoverAnswer() {
        // вывод стоимости
        int cost = -colP[0];
        // восстановление ответа
        /*std::vector<int> ans(n + 1);*/
        /*for (int i = 1; i <= m; ++i)
            ans[pairs[i]] = i;*/
        /*for (int i = 1; i <= m; ++i) {
            std::cout << pairs[i] << ' '  << i << std::endl;
        }*/
    }
};

const int Hungary::INF{std::numeric_limits<int>::max()};