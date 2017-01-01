#pragma once

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

class AhoCorasick {
protected:
    const static int ROOT = 0;
    const static int UNINITIALIZED;

    struct Node {
        std::vector<int> next, go;
        int suffLink, up, parent;
        char charToParent;
        int strNum; // Номер строки, которая кончается в этой вершине, UNINITIALIZED, если никакая не кончается.
        std::vector<int> leafPatternNumber;

        Node(int alphabetSize, int parent) : suffLink(UNINITIALIZED), parent(parent) {
            next.resize(alphabetSize, UNINITIALIZED);
            go.resize(alphabetSize, UNINITIALIZED);
        }

        // Является ли концом какой-либо строки.
        bool isEnd() { return strNum != UNINITIALIZED; }
    };

public:
    AhoCorasick(char minChar, char maxChar) : minChar(minChar), maxChar(maxChar) {
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
            }
            v = nodes[v].next[c];
        }
        nodes[v].strNum = stringsCount++;
    }

    void addNode(Node node) {
        nodes.push_back(node);
    }

    bool processText(std::string& t) {
        //int depth = 0;
        int vcur = ROOT;
        for (int i = 0; i < t.length(); ++i) {
            char c = normalizeChar(t[i]);
            vcur = go(vcur, c);
            for (int v = vcur; v != ROOT; v = nodes[v].suffLink) {
                if (nodes[v].isEnd()) {
                    std::cout << nodes[v].strNum << ' '/*<< i-depth+1*/;
                }
            }
        }
    }

public:

    // Ссылки выставляются лениво
    int getSuffLink(int v) {
        if (nodes[v].suffLink == UNINITIALIZED) {
            // Корень и его непосредственные дети ссылаются на корень
            if (v == ROOT || nodes[v].parent == ROOT) {
                nodes[v].suffLink = ROOT;
            } else {
                // Пытаемся выставить ссылку такой же, как и у родителя
                nodes[v].suffLink = go(getSuffLink(nodes[v].parent), nodes[v].charToParent);
            }
            return nodes[v].suffLink;
        }
    }

    int go(int v, char c) {
        if (nodes[v].go[c] == UNINITIALIZED) {
            if (nodes[v].next[c] != UNINITIALIZED) {
                // Если можем перейти из вершины по букве, идём
                nodes[v].go[c] = nodes[v].next[c];
            } else if (v == ROOT) {
                // У корня по определению все ссылки, по которым не можем пройти вниз, направим в него же
                nodes[v].go[c] = ROOT;
            } else {
                nodes[v].go[c] = go(getSuffLink(v), c);
            }
        }
        return nodes[v].go[c];
    }

    char normalizeChar(char c) {
        assert(minChar <= c && c <= maxChar);
        return c - minChar;
    }

    std::vector<Node> nodes;
    int alphabetSize;
    char minChar, maxChar;
    int stringsCount = 0;
};
