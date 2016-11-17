#pragma once
#include <vector>
#include <limits>
#include <cstddef>
#include "Graph.h"

using uint = unsigned int;

template<typename vtype, typename etype, typename FlowType, typename VST, typename EST>
class Network :public Graph<vtype, etype, VST, EST> {
private:

    struct NetworkEdge;
    struct NetworkVertex;
public:
    class NetworkEdgeIterator : public GraphEdgeIterator {
    public:
        NetworkEdgeIterator() = default;

        NetworkEdgeIterator(Network& graph_, vtype v_, bool first) :GraphEdgeIterator(graph_, v_, first) {}

        NetworkEdgeIterator(Network& graph_, etype num):GraphEdgeIterator(graph_, num) {}

        using GraphEdgeIterator::hasNext;

        using GraphEdgeIterator::operator==;
        using GraphEdgeIterator::operator!=;

        // Set start pointer to the next Edge or to NullEdge if no other left.
        inline void shiftBeginToNext() {
            graph->vertices[v].first = edge->next;
        }

        using GraphEdgeIterator::operator++;

        inline bool isDeleted() { return edge->deleted; }
        inline void markAsDeleted() { edge->deleted = true; }
        inline void markReverseAsDeleted() { graph->edges[BackEdge(e)].deleted = true; }
        inline void unmarkAsDeleted() { edge->deleted = false; }

        inline FlowType getFlow() { return edge->flow; }
        inline int getCapacity() { return edge->capacity; }
        inline vtype getStart() { return edge->from; }
        inline vtype getFinish() { return edge->to; }
        inline etype getEdgeNumber() { return e; }
        inline FlowType getResidualCapacity() { return getCapacity() - getFlow(); }
        inline void pushFlow(FlowType f) {
            edge->flow += f;
            graph->edges[BackEdge(e)].flow -= f;
        }
        inline void pushCapacity(FlowType c) {
            edge->capacity += c;
        }

    private:
        vtype v; // NetworkVertex that iterator belongs to.
        etype e;
        Network* graph;
        NetworkEdge* edge;

        etype BackEdge(etype edgeNum) {
            return edgeNum ^ 1;
        }

        bool checkBackward(etype edge) { return (edge & 1) == 1; }

        friend class Network;
    };

    Network() = default;

    Network(vtype n, vtype s, vtype t) {
        init(n, s, t);
    }

    void init(vtype n_, vtype s_, vtype t_) {
        s = s_;
        t = t_;
        vertices.resize(n_);
        edges.clear();
    }

    inline vtype getSource() { return s; }
    inline vtype getTarget() { return t; }
    inline vtype getVertexCount() { return vertices.size(); }
    inline etype getEdgesCount() { return edges.size(); }
    inline void setSource(vtype s_) { s = s_; }
    inline void setTarget(vtype t_) { t = t_; }
    inline void insertUndirectedEdge(vtype u, vtype v, FlowType c) {
        insertEdgeLocal(u, v, c, 0);
        insertEdgeLocal(v, u, c, 0);
    }
    inline void insertDirectedEdge(vtype u, vtype v, FlowType c) {
        insertEdgeLocal(u, v, c, 0);
        insertEdgeLocal(v, u, 0, 0);
    }
    inline void insertRemainEdge(vtype u, vtype v, FlowType c) {
        insertEdgeLocal(u, v, c, 0);
        insertEdgeLocal(v, u, -c, 0);
    }

    void resetBegins() {
        for (auto&& v: vertices) {
            v.first = v.backup_first;
        }
    }

    void resetFlow() {
        for (auto&& e: edges) {
            e.f = 0;
        }
    }

    // Returns iterator of vertex v edges.
    NetworkEdgeIterator begin(vtype v) { return NetworkEdgeIterator(*this, v, true); }
    NetworkEdgeIterator end() { return NetworkEdgeIterator(*this, 0, false); }
    NetworkEdgeIterator getEdgeByNum(etype num) { return num != -1 ? NetworkEdgeIterator(*this, num) : end(); }
private:
    static const etype NullEdge = std::numeric_limits<etype>::max();
    vtype s, t;
    std::vector<NetworkEdge> edges;
    std::vector<NetworkVertex> vertices; // First and last (according to time of addition) edges of vertex.

    struct FlowData
    {
        FlowType capacity, flow;
        EST data;
    };

    struct NetworkEdge :public GraphEdge {
        bool deleted = false;
        vtype from, to;

        etype next = NullEdge;
        NetworkEdge(vtype u, vtype v, FlowType c, FlowType f, EST data) : GraphEdge(u, v, data), capacity(c), flow(f) {}
    };

    struct NetworkVertex {
        etype first = NullEdge;
        etype last = NullEdge;
        etype backup_first = NullEdge;
    };

    void insertEdgeLocal(vtype from, vtype to, FlowType capacity, FlowType flow) {
        edges.push_back(NetworkEdge(from, to, capacity, flow));
        // Set first if necessary.
        if (vertices[from].first == NullEdge) {
            vertices[from].backup_first = vertices[from].first = edges.size() - 1;
        } else {
            etype tmp = vertices[from].last;
            // Set previous and next.
            edges[tmp].next = edges.size() - 1;
        }
        // Set last.
        vertices[from].last = edges.size() - 1;
    }


};