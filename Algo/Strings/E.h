#pragma once

#include <iostream>
#include <vector>
#include <string>

class E {
public:
    void solve() {
        int n;
        std::cin >> n;
        std::vector<int> prefix(n);
        for (int i = 0; i < n; ++i) {
            std::cin >> prefix[i];
        }

        std::vector<int> z = std::move(getZfromPrefix(prefix));
        for (int i = 0; i < n; ++i) {
            std::cout << z[i] << ' ';
        }

    }
private:
    std::vector<int> getZfromPrefix(const std::vector<int>& prefix) {
        int n = prefix.size();

        std::vector<int> z(n);
        for (int i = 0; i < n; ++i) {
            if (prefix[i] > 0) {
                z[i - prefix[i] + 1] = prefix[i];
            }
        }
        z[0] = n;
        int i = 1;
        while (i < n) {
            int t = i;
            if (z[i] > 0) {
                for (int j = 0; j < z[i]; ++j) {
                    if (z[i + j] > z[j]) {
                        break;
                    }
                    z[i + j] = std::min(z[j], z[i] - j);
                    t = i + j;
                }
            }
            i = t + 1;
        }
        return z;
    }

};
