#pragma once
#include <string>
#include <iostream>
#include "Network.h"
#include "dinic.h"

template<typename vtype, typename etype, typename FlowType>
class HammingOptimizer {
public:
    void solve() {
        read();
        {
            dinic<vtype, etype, FlowType> dinic;
            flow = dinic.findMaxFlow(&net);
        }
        bfs();
        printResult();
    }

private:
    std::vector<bool> visited;
    Network<vtype, etype, FlowType> net;
    std::string string, pattern;
    FlowType guaranteedDistance = 0, flow;

    inline vtype getPatternIndex(vtype v) { return 2 + v; }
    inline vtype getStringIndex(vtype v) { return 2 + pattern.length() + v; }

    // 0's are connected with source, 1's - with target.
    void read() {
        std::cin >> string >> pattern;
        net.init(pattern.length() + string.length() + 2, 0, 1);

        std::vector<vtype> PatternToSource(pattern.size(), 0), StringToSource(string.size(), 0),
                PatternToTarget(pattern.size(), 0), StringToTarget(string.size(), 0);

        for (uint i = 0; i < string.length() - pattern.length() + 1; ++i)
            for (uint j = 0; j < pattern.length(); ++j) {
                if (string[i + j] == '?' && pattern[j] == '?')
                    net.insertUndirectedEdge(getStringIndex(i + j), getPatternIndex(j), 1);
                else {
                    net.insertUndirectedEdge(getStringIndex(i + j), getPatternIndex(j), 0);
                    if ((string[i + j] == '0' && pattern[j] == '1') || (string[i + j] == '1' && pattern[j] == '0'))
                        ++guaranteedDistance;
                }

                StringToSource[i + j] += (pattern[j] == '0' && string[i + j] == '?');
                PatternToSource[j] += (string[i + j] == '0' && pattern[j] == '?');
                StringToTarget[i + j] += (pattern[j] == '1' && string[i + j] == '?');
                PatternToTarget[j] += (string[i + j] == '1' && pattern[j] == '?');
            }

        for (uint j = 0; j < pattern.length(); ++j)
            if (PatternToSource[j] != 0)
                net.insertDirectedEdge(net.getSource(), getPatternIndex(j), PatternToSource[j]);

        for (uint j = 0; j < pattern.length(); ++j)
            if (PatternToTarget[j] != 0)
                net.insertDirectedEdge(getPatternIndex(j), net.getTarget(), PatternToTarget[j]);

        for (uint j = 0; j < string.length(); ++j)
            if (StringToSource[j] != 0)
                net.insertDirectedEdge(net.getSource(), getStringIndex(j), StringToSource[j]);

        for (uint j = 0; j < string.length(); ++j)
            if (StringToTarget[j] != 0)
                net.insertDirectedEdge(getStringIndex(j), net.getTarget(), StringToTarget[j]);
    }

    void bfs() {
        std::queue<vtype> q;
        visited.resize(net.getVertexCount(), false);
        visited[net.getSource()] = true;
        q.push(net.getSource());
        while (!q.empty()) {
            vtype u = q.front();
            q.pop();
            for (auto it = net.begin(u); it != net.end(); ++it)
                if (it.getResidualCapacity() > 0 && visited[it.getFinish()] == false) {
                    visited[it.getFinish()] = true;
                    q.push(it.getFinish());
                }
        }
    }

    void printString(std::string& s, vtype offset) {
        for (uint i = 0; i < s.length(); ++i) {
            if (s[i] == '?')
                s[i] = visited[i + offset] == 1 ? '0' : '1';
        }
        std::cout << s << std::endl;
    }

    void printResult() {
        std::cout << guaranteedDistance + flow << std::endl;
        printString(string, 2 + pattern.length());
        printString(pattern, 2);
    }

};

