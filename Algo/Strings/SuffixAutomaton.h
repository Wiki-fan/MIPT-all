#pragma once

#include <map>
#include <vector>
#include <iostream>
using uint = unsigned int;

// Как нормально передавать контейнер для детей как шаблон? И нужно ли это вообще?
template<typename vtype = uint, typename ChildStorageType = std::vector<vtype>>
class SuffixAutomaton {
protected:
    struct Node;
public:
    // Предполагается, что вершинами бора могут быть все буквы [minC, maxC].
    SuffixAutomaton(char minC, char maxC)
            : last(ROOT), alphabetSize(maxC - minC + 1), minC(minC), maxC(maxC) {
        st.push_back(Node(alphabetSize, 0)); // root
    }

    void addString(const std::string& s) {
        for (size_t i = 0; i < s.length(); i++) {
            addChar(normalize(s[i]));
        }
    }

    bool checkOccurence(const std::string& s) {
        bool hasOccurence = true;
        vtype v = ROOT;
        for (size_t i = 0; i < s.length(); i++) {
            if (st[v].child[normalize(s[i])] == UNINITIALIZED) {
                hasOccurence = false;
                break;
            } else {
                v = st[v].child[normalize(s[i])];
            }
        }
        return hasOccurence;
    }

protected:

    struct Node {
        int depth; // Глубина вершины в дереве
        vtype suffLink;
        ChildStorageType child;
        bool isEnd;

        Node(int alphabetSize, int len) : depth(len), suffLink(UNINITIALIZED), isEnd(false) {
            child.resize(alphabetSize, UNINITIALIZED);
        }

        Node(const Node& other) : depth(other.depth), suffLink(other.suffLink), child(other.child) {}
    };

    std::vector<Node> st;
    vtype last;

    const static vtype ROOT = 0;
    const static vtype UNINITIALIZED;

    const char minC, maxC;
    const int alphabetSize;

    inline virtual int normalize(char c) {
        return c - minC;
    }

    void addChar(char c) {
        vtype newLast = st.size();
        st.emplace_back(alphabetSize, st[last].depth + 1);

        vtype v = last;
        while (v != UNINITIALIZED && st[v].child[c] == UNINITIALIZED) {
            st[v].child[c] = newLast;
            v = st[v].suffLink;
        }

        if (v != UNINITIALIZED) {
            vtype q = st[v].child[c];
            if (st[q].depth == st[v].depth + 1) {
                st[newLast].suffLink = q;
            } else {
                vtype clone = st.size();
                st.push_back(Node(st[q]));
                st[clone].depth = st[v].depth + 1;

                while (v != UNINITIALIZED && st[v].child[c] == q) {
                    st[v].child[c] = clone;
                    v = st[v].suffLink;
                }
                st[q].suffLink = st[newLast].suffLink = clone;
            }
        } else {
            st[newLast].suffLink = ROOT;
        }
        last = newLast;
    }
};

// И почему я не могу прописать это внутри класса, хотя ROOT прописывается?
template<typename vtype, typename ChildStorageType>
const int SuffixAutomaton<vtype, ChildStorageType>::UNINITIALIZED = -1;
