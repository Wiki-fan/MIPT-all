#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "SuffixAutomaton.h"

// Кажется, работает
class L : public SuffixAutomaton<> {
public:

    L() : SuffixAutomaton('a', 'z') {}

    int normalize(char c) override {
        return tolower(c) - minC;
    }

    void solve() {
        std::string cmd, s;
        while (std::cin >> cmd >> s) {
            if (cmd[0] == 'A') {
                addString(s);
            } else {
                std::cout << (checkOccurence(s) ? "YES" : "NO") << '\n';
            }
        }
    }
};
