#pragma once
#include <vector>
#include <string>
#include <cassert>
#include <iostream>
#include <unordered_set>

// Кажется, оно не работает
class I {
protected:
    const static int ROOT = 0;
    const static int UNINITIALIZED;

    struct Node {
        std::vector<int> next, go;
        std::unordered_set<int> reverseSuffLink; // вершины, из которых суффиксные ссылки ведут в данную
        int suffLink, parent;
        char charToParent;
        bool isEndReachable = false;
        int strNum; // Номер строки, которая кончается в этой вершине, UNINITIALIZED, если никакая не кончается.

        Node(int alphabetSize, int parent) : suffLink(UNINITIALIZED), parent(parent), strNum(UNINITIALIZED) {
            next.resize(alphabetSize, UNINITIALIZED);
            go.resize(alphabetSize, UNINITIALIZED);
        }

        // Является ли концом какой-либо строки.
        bool isEnd() { return strNum != UNINITIALIZED; }
    };

public:
    I(char minChar, char maxChar) : minChar(minChar), maxChar(maxChar), alphabetSize(maxChar - minChar + 1) {
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
        nodes[v].isEndReachable = true;
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
                nodes[ROOT].reverseSuffLink.insert(v);
            } else {
                // Пытаемся выставить ссылку такой же, как и у родителя
                nodes[v].suffLink = go(getSuffLink(nodes[v].parent), nodes[v].charToParent);
                nodes[go(getSuffLink(nodes[v].parent), nodes[v].charToParent)].reverseSuffLink.insert(v);
            }
        }
        return nodes[v].suffLink;
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

    void DFS() {
        fl = false;
        visited.resize(nodes.size(), Color::WHITE);
        dfs(ROOT);
        if (!fl) {
            std::cout << "NIE";
        }
    }

    void dfs(int v) {
        if (fl) {
            return;
        }
        if (nodes[v].isEndReachable) {
            visited[v] = Color::BLACK;
            return;
        }
        visited[v] = Color::GREY;
        for (int c = 0; v < alphabetSize; ++c) {
            if (nodes[v].go[c] != UNINITIALIZED) {
                // Пропускаем те, из которых достижим конец
                if (nodes[nodes[v].go[c]].isEndReachable == false) {
                    if (visited[nodes[v].go[c]] == Color::BLACK) {
                        continue;
                    }
                    if (visited[nodes[v].go[c]] == Color::GREY) {
                        std::cout << "TAK";
                        fl = true;
                        return;
                    }
                    dfs(nodes[v].go[c]);
                }
            }
        }
    }

    void dfsReverseSuffixLink(int v) {
        for (auto u: nodes[v].reverseSuffLink) {
            nodes[u].isEndReachable = true;
            dfsReverseSuffixLink(u);
        }
    }

    void solve() {
        int n;
        std::cin >> n;
        for (int i = 0; i < n; ++i) {
            std::string s;
            std::cin >> s;
            addString(s);
        }

        for (int i = 0; i < nodes.size(); ++i) {
            getSuffLink(i);
            for (int c = 0; c < alphabetSize; ++c) {
                go(i, c);
            }
        }
        for (int i = 0; i < nodes.size(); ++i) {
            if (nodes[i].isEnd()) {
                dfsReverseSuffixLink(i);
            }
        }
        for (int i = 0; i < nodes.size(); ++i) {
            std::cout << i << ' ' << nodes[i].suffLink;
            for (int j = 0; j < alphabetSize; ++j) {
                std::cout << ' ' << nodes[i].next[j] << ':' << nodes[i].go[j];
            }
            std::cout << '\n';
        }

        DFS();
    }

    std::vector<Node> nodes;
    int alphabetSize;
    char minChar, maxChar;
    int stringsCount = 0;
    enum class Color {
        WHITE, GREY, BLACK
    };
    std::vector<Color> visited;
    bool fl;
};
