#pragma once

#include <vector>
#include <string>
#include <cassert>
#include <iostream>

// Даже не претендовало на то, чтобы работать.
class J {
public:
    const static int ROOT = 0;
    const static int UNINITIALIZED;

    struct Node {
        std::vector<int> next, go;
        int suffLink, up, parent;
        char charToParent;
        int strNum; // Номер строки, которая кончается в этой вершине, UNINITIALIZED, если никакая не кончается.
        std::vector<int> leafPatternNumber;
        int corresponding;

        Node(int alphabetSize, int parent) : suffLink(UNINITIALIZED), parent(parent) {
            next.resize(alphabetSize, UNINITIALIZED);
            go.resize(alphabetSize, UNINITIALIZED);
        }

        // Является ли концом какой-либо строки.
        bool isEnd() { return strNum != UNINITIALIZED; }
    };

    J(char minChar, char maxChar) : minChar(minChar), maxChar(maxChar) {
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

    void insertEdge(int from, char c, int to) {
        nodes[from].next[c] = to;
    }

    void resize(int n) {
        nodes.resize(n);
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

    // уже нормализован
    void move(char c) {
        //c = normalizeChar(c);
        vcur = go(vcur, c);
    }

    char normalizeChar(char c) {
        assert(minChar <= c && c <= maxChar);
        return c - minChar;
    }

    void dfs(int v, J& ak) {
        bool wentFurther = false;
        for (int c = 0; c < alphabetSize; ++c) {
            if (nodes[v].next[c] != UNINITIALIZED) {
                // Зашли в рекурсию - отработали - вернули значение обратно
                nodes[v].corresponding = ak.vcur;
                ak.move(c);
                wentFurther = true;
                dfs(nodes[v].next[c], ak);
                ak.vcur = nodes[v].corresponding;
            }
        }
        if (wentFurther = false) {

        }
    }

    void my(int v, J& ak) {
        int currentInAK = ak.vcur;
        for (int i = 0; i < alphabetSize; ++i) {
            if ()
        }
    }
    std::vector<Node> nodes;
    int alphabetSize;
    char minChar, maxChar;
    int stringsCount = 0;

    // Состояние автомата
    int vcur;
};


// Построить бор из строк. Потом обходом по данному бору получать текст и скармливать его Ахо-Корасику.
// Для каждой развилки исходного бора хранить указатель на неё.
class J {
public:
    J trie, ak;

    J() : trie('a', 'z'), ak('a', 'z') {}

    void solve() {
        int n;
        std::cin >> n;
        trie.resize(n);
        for (int i = 0; i < n; ++i) {
            int edgesCount;
            std::cin >> edgesCount;

            for (int j = 0; i < edgesCount; ++i) {
                int to;
                char c;
                std::cin >> to >> c;
                trie.insertEdge(n, c, to);
            }
        }

        int m;
        std::cin >> m;
        ak.resize(m);
        for (int i = 0; i < m; ++i) {
            std::string s;
            std::cin >> s;
            ak.addString(s);
        }

        ak.vcur = ak.ROOT;
        trie.dfs(trie.ROOT, ak);
    }

private:


};