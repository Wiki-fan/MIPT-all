#pragma once

#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include "MyException.h"

// Бор
class G {
public:
    void solve() {
        init('a', 'z');
        read();
    }
private:
    void init(char minChar_, char maxChar_) {
        minChar = minChar_;
        maxChar = maxChar_;
        alphabetSize = maxChar - minChar + 1;
        nodes.emplace_back(alphabetSize, UNINITIALIZED);
    }

    void addString(const std::string& s) {
        int v = ROOT;
        for (int i = 0; i < s.length(); ++i) {
            char c = normalizeChar(s[i]);
            if (nodes[v].next[c] == UNINITIALIZED) {
                nodes.emplace_back(alphabetSize, v);
                nodes.back().charToParent = c;
                nodes[v].next[c] = nodes.size() - 1;

                int prevLeft = 0;
                for (int vLeft = c - 1; vLeft >= 0; --vLeft) {
                    if (nodes[v].next[vLeft] != UNINITIALIZED) {
                        prevLeft = nodes[nodes[v].next[vLeft]].leftCount;
                        break;
                    }
                }
                nodes.back().leftCount = prevLeft;
            }

            for (int j = c; j < alphabetSize; ++j) {
                if (nodes[v].next[j] != UNINITIALIZED) {
                    ++nodes[nodes[v].next[j]].leftCount;
                }
            }

            v = nodes[v].next[c];
        }
        nodes[v].strNum = stringsCount++;
    }

    std::string getNthString(int n) {
        int v = ROOT;
        std::string s;
        while (n != 0) {
            for (int i = 0; i < alphabetSize; ++i) {
                if (nodes[v].next[i] != UNINITIALIZED && nodes[nodes[v].next[i]].leftCount >= n) {
                    if (nodes[nodes[v].next[i]].isEnd()) {
                        --n;
                    }

                    int prevLeft = 0;
                    for (int vLeft = i - 1; vLeft >= 0; --vLeft) {
                        if (nodes[v].next[vLeft] != UNINITIALIZED) {
                            prevLeft = nodes[nodes[v].next[vLeft]].leftCount;
                            break;
                        }
                    }
                    n -= prevLeft;
                    v = nodes[v].next[i];
                    s.push_back(denormalizeIndex(nodes[v].charToParent));

                    break;
                }
            }
        }
        //s.push_back(denormalizeIndex(nodes[nodes[v].next[i]].charToParent));
        return s;
    }

    char normalizeChar(char c) {
        assert(minChar <= c && c <= maxChar);
        return c - minChar;
    }

    char denormalizeIndex(char i) {
        return i + minChar;
    }

    struct Node {
        int leftCount = 0; // Количество концов левее.
        std::vector<int> next, go;
        int suffLink, parent;
        char charToParent;
        int strNum; // Номер строки, которая кончается в этой вершине, UNINITIALIZED, если никакая не кончается.

        Node(int alphabetSize, int parent) : suffLink(UNINITIALIZED), parent(parent), strNum(UNINITIALIZED) {
            next.resize(alphabetSize, UNINITIALIZED);
            go.resize(alphabetSize, UNINITIALIZED);
        }

        // Является ли концом какой-либо строки.
        bool isEnd() { return strNum != UNINITIALIZED; }
    };

    std::vector<Node> nodes;
    int alphabetSize;
    char minChar, maxChar;
    int stringsCount = 0;
    const static int ROOT = 0;
    const static int UNINITIALIZED;

    void read() {
        int n;
        std::cin >> n;

        for (int i = 0; i < n; ++i) {
            char c;
            std::cin >> c;
            std::cin.putback(c);
            if (isalpha(c)) {
                // Если строка, добавляем
                std::string s;
                std::getline(std::cin, s);
                addString(s);
            } else {
                // Если число, ищем и выводим
                int k;
                std::cin >> k;
                std::cout << getNthString(k) << std::endl;
            }
        }
    }
};
