#pragma once
#include <string>
#include <iostream>
#include "Network.h"
#include "Dinic.h"

template<typename vtype, typename etype, typename FlowType>
class HammingOptimizer {
public:
    void solve() {
        read();
        {
            Dinic<vtype, etype, FlowType> dinic;
            flow = dinic.findMaxFlow(&net);
        }
        bfs();
        printResult();
    }

private:
    std::vector<bool> visited;
    Network<vtype, etype, FlowType> net;
    std::string s, p;
    FlowType guaranteedDistance = 0, flow;
    vtype slen, plen;

    inline vtype getL(vtype v) { return 2 + v; }
    inline vtype getR(vtype v) { return 2 + plen + v; }

    // Left is template p, right is string s. 0 are connected with source, 1 with target.
    void read() {
        std::cin >> s >> p;
        plen = p.length();
        slen = s.length();
        net.init(plen + slen + 2, 0, 1);

        std::vector<vtype> ltos(p.size(), 0), rtos(s.size(), 0), ltot(p.size(), 0), rtot(s.size(), 0);

        for (uint i = 0; i < slen - plen + 1; ++i)
            for (uint j = 0; j < plen; ++j) {
                if (s[i + j] == '?' && p[j] == '?')
                    net.insertUndirectedEdge(getR(i + j), getL(j), 1);
                else {
                    net.insertUndirectedEdge(getR(i + j), getL(j), 0);
                    if ((s[i + j] == '0' && p[j] == '1') || (s[i + j] == '1' && p[j] == '0'))
                        ++guaranteedDistance;
                }

                rtos[i + j] += (p[j] == '0' && s[i + j] == '?');
                ltos[j] += (s[i + j] == '0' && p[j] == '?');
                rtot[i + j] += (p[j] == '1' && s[i + j] == '?');
                ltot[j] += (s[i + j] == '1' && p[j] == '?');
            }

        for (uint j = 0; j < plen; ++j)
            if (ltos[j] != 0)
                net.insertDirectedEdge(net.getSource(), getL(j), ltos[j]);

        for (uint j = 0; j < plen; ++j)
            if (ltot[j] != 0)
                net.insertDirectedEdge(getL(j), net.getTarget(), ltot[j]);

        for (uint j = 0; j < slen; ++j)
            if (rtos[j] != 0)
                net.insertDirectedEdge(net.getSource(), getR(j), rtos[j]);

        for (uint j = 0; j < slen; ++j)
            if (rtot[j] != 0)
                net.insertDirectedEdge(getR(j), net.getTarget(), rtot[j]);
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
                s[i] = visited[i + offset] == 1? '0':'1';
        }
        std::cout << s<< std::endl;
    }

    void printResult() {
        std::cout << guaranteedDistance + flow << std::endl;
        printString(s, 2 + plen);
        printString(p, 2);
    }

};
