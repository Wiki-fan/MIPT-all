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
        Dinic<vtype, etype, FlowType> dinic;
        flow = dinic.findMaxFlow(&net);
        bfs();
        printResult();
    }

private:
    enum class Marks {
        Left, Right
    };
    std::vector<Marks> marks;
    Network<vtype, etype, FlowType> net;
    std::string s, p;
    FlowType guaranteedDistance = 0, flow;

    vtype getL(vtype v) { return 2 + v; }
    vtype getR(vtype v) { return 2 + p.length() + v; }

    void insertOrIncrement(vtype u, vtype v) {
        auto it = net.begin(u);
        if (it != net.end()) {
            while (it.getFinish() != v && it != net.end()) ++it;
            if (it.getFinish() == v) it.pushCapacity(1);
        } else
            net.insertDirectedEdge(u, v, 1);

    }

    // Left is template p, right is string s. 0 are connected with source, 1 with target.
    void read() {
        std::cin >> s >> p;
        net.init(p.length() + s.length() + 2, 0, 1);
        std::vector<vtype> ltos(p.size(), 0), rtos(s.size(), 0), ltot(p.size(), 0), rtot(s.size(), 0);

        for (uint i = 0; i < s.length() - p.length() + 1; ++i)
            for (uint j = 0; j < p.length(); ++j) {
                ltos[j] += (p[j] == '0');
                rtos[i + j] += (s[i + j] == '0');
                ltot[j] += (p[j] == '1');
                rtot[i + j] += (s[i + j] == '1');
            }

        for (uint j = 0; j < s.length(); ++j) {
            if (s[j] == '?' && ltos[j] != 0)
                net.insertDirectedEdge(net.getSource(), getL(j), ltos[j]);
        }
        for (uint j = 0; j < s.length(); ++j) {
            if (s[j] == '?' && ltot[j] != 0)
                net.insertDirectedEdge(getL(j), net.getTarget(), ltot[j]);
        }
        for (uint j = 0; j < p.length(); ++j) {
            if (p[j] == '?' && rtos[j] != 0)
                net.insertDirectedEdge(net.getSource(), getR(j), rtos[j]);
        }
        for (uint j = 0; j < p.length(); ++j) {
            if (p[j] == '?' && rtot[j] != 0)
                net.insertDirectedEdge(getR(j), net.getTarget(), rtot[j]);
        }
        for (uint i = 0; i < s.length() - p.length() + 1; ++i)
            for (uint j = 0; j < p.length(); ++j) {
                if (s[i + j] == '?' && p[j] == '?')
                    net.insertUndirectedEdge(getR(i + j), getL(j), 1);
                else if ((s[i + j] == '0' && p[j] == '1') || (s[i + j] == '1' && p[j] == '0'))
                    ++guaranteedDistance;

            }
    }

    void bfs() {
        std::queue<vtype> q;
        marks.resize(net.getVertexCount(), Marks::Right);
        marks[net.getSource()] = Marks::Left;
        q.push(net.getSource());
        while (!q.empty()) {
            vtype u = q.front();
            q.pop();
            for (auto it = net.begin(u); it != net.end(); ++it)
                if (it.getResidualCapacity() > 0 && marks[it.getFinish()] != Marks::Left) {
                    marks[it.getFinish()] = Marks::Left;
                    q.push(it.getFinish());
                }
        }
    }

    void printString(std::string& s, vtype offset) {
        for (uint i = 0; i < s.length(); ++i) {
            if (s[i] != '?') std::cout << s[i];
            else std::cout << (marks[i + offset] == Marks::Left ? '0' : '1');
        }
    }

    void printResult() {
        std::cout << guaranteedDistance + flow << std::endl;
        printString(s, 2 + p.length());
        std::cout << std::endl;
        printString(p, 2);
        std::cout << std::endl;
    }

};
