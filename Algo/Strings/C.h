#pragma once
#include <iostream>
#include <string>
#include "StringHashes.h"
#include <climits>

class C {
public:
    void solve() {
        std::string s;
        s.resize(100001);
        scanf("%s", &s[0]);
        int n;
        StringHashes hashes1(71, UINT_MAX, s);
        scanf("%d", &n);
        for (int i = 0; i < n; ++i) {
            printAnswer(hashes1, s);
        }
    }
private:
    void printAnswer(const StringHashes& hashes1, const std::string& s) {
        int l1, r1, l2, r2;
        scanf("%d %d %d %d", &l1, &r1, &l2, &r2);
        if (r1 - l1 != r2 - l2) {
            printf("No\n");
            return;
        }
        if (hashes1.calcHash(l1 - 1, r1 - 1) == hashes1.calcHash(l2 - 1, r2 - 1)) {
            /*for (int j = 0; j<r1-l1; ++j) {
                if (s[l1+j] != s[l2+j]) {
                    printf("No\n");
                    return;
                }
            }*/
            printf("Yes\n");
        } else {
            printf("No\n");
        }
    }
};
