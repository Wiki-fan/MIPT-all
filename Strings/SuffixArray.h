#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

class SuffixArray {
public:
    // Алгоритм Касаи
    std::vector<int> getLCP(const std::string& s, const std::vector<int>& suffArray) {
        int n = s.length();

        // Массив, обратный массиву suffArray
        std::vector<int> pos(n);
        for (int i = 0; i < n; ++i) {
            pos[suffArray[i]] = i;
        }

        std::vector<int> lcp(n);
        int k = 0;
        for (int i = 0; i < n; ++i) {
            if (k > 0) {
                --k;
            }
            if (pos[i] == n - 1) {
                lcp[n - 1] = -1;
                k = 0;
            } else {
                int j = suffArray[pos[i] + 1];
                while (std::max(i + k, j + k) < n && s[i + k] == s[j + k]) {
                    ++k;
                }
                lcp[pos[i]] = k;
            }
        }

        /*for (auto& i: lcp) {
            ++i;
        }*/
        return std::move(lcp);
    }

    using ui32 = int;
    std::vector<ui32> sortCycleShifts(const std::string& s) {
        ui32 n = s.length();
        std::vector<ui32> a(n), na(n), classes(n), newClasses(n), h;
        ui32 kolc = 256;

        h.assign(kolc, 0);
        for (ui32 i = 0; i < n; ++i) {
            classes[i] = s[i];
            h[s[i]]++;
        }
        for (ui32 i = 1; i < h.size(); ++i) {
            h[i] += h[i - 1];
        }
        for (int i = n - 1; i >= 0; --i) {
            na[--h[classes[i]]] = i;
        }
        a.swap(na);

        for (ui32 l = 1; l <= n; l += l) {
            h.assign(kolc, 0);
            for (ui32 i = 0; i < n; ++i) {
                h[classes[i]]++;
            }
            for (ui32 i = 1; i < kolc; ++i) {
                h[i] += h[i - 1];
            }
            for (int i = n - 1; i >= 0; --i) {
                int v = (a[i] - l + n) % n;
                na[--h[classes[v]]] = v;
            }

            a.swap(na);
            kolc = 1;
            newClasses[a[0]] = 0;
            for (ui32 i = 1; i < n; ++i) {
                if ((classes[a[i]] == classes[a[i - 1]]) && (classes[(a[i] + l) % n] == classes[(a[i - 1] + l) % n])) {
                    newClasses[a[i]] = kolc - 1;
                } else {
                    newClasses[a[i]] = kolc++;
                }
            }
            classes.swap(newClasses);
        }
        return a;
    }

    std::vector<ui32> buildFromLecture(const std::string& s) {
        std::vector<ui32> ans = sortCycleShifts(s + '\0');
        ans.erase(ans.begin());
        return ans;
    }

    std::vector<int> buildNaive(const std::string& s) {
        int n = s.length();
        std::vector<int> suffArray(n);
        for (int i = 0; i < n; ++i) {
            suffArray[i] = i;
        }
        auto compareStringShiftsByModulo = [&n, &s](int j1, int j2)->int {
            for (int i = 0; i < n; ++i) {
                if (s[(j1 + i) % n] < s[(j2 + i) % n]) {
                    return true;
                } else if (s[(j1 + i) % n] > s[(j2 + i) % n]) {
                    return false;
                }
            }
            return false;
        };
        std::sort(suffArray.begin(), suffArray.end(), compareStringShiftsByModulo);

        return suffArray;
    }
};