#pragma once

class StringFunctions {
    std::vector<int> computeZFunction(const std::string& s) {
        int n = s.length();
        std::vector<int> zf(n);
        int left = 0, right = 0;
        for (int i = 0; i < n; ++i) {
            zf[i] = std::max(0, std::min(right - i, zf[i - left]));
            while (i + zf[i] < n && s[zf[i]] == s[i + zf[i]]) {
                ++zf[i];
            }
            if (i + zf[i] >= right) {
                left = i;
                right = i + zf[i];
            }
        }
        return zf;
    }

    std::vector<int> computePrefixFunctionNaive(const std::string& s) {
        int n = s.length();
        std::vector<int> pf(n);
        for (int i = 0; i < n; ++i) {
            for (int k = 0; k <= i; ++k) {
                if (s.substr(0, k) == s.substr(i - k + 1, k)) {
                    pf[i] = k;
                }
            }
        }
        return pf;
    }
};