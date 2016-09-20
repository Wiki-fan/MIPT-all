#pragma once
//#include "../stdafx.h"
#include <vector>
#include <limits>

using uint = unsigned int;

// Remember: vertices are unsigned, edges are signed!
template<typename vtype, typename etype, typename FlowType>
class Network {
private:

    struct Edge;
    struct Vertex;
public:
    class NetworkEdgeIterator : public std::iterator<std::forward_iterator_tag, Edge> {
    public:
        NetworkEdgeIterator() = default;

        NetworkEdgeIterator(Network& graph_, vtype v_, bool first) {
            graph = &graph_;
            if (first) {
                v = v_;
                e = graph->vertices[v].first;
                edge = &graph->edges[e];
            } else {
                e = graph->NullEdge;
            }
        }

        NetworkEdgeIterator(Network& graph_, etype num) {
            graph = &graph_;
            e = num;
            edge = &graph->edges[e];
        }

        bool hasNext() { return edge->next != graph->NullEdge; }

        bool operator==(const NetworkEdgeIterator& other) const { return graph == other.graph && e == other.e; }
        bool operator!=(const NetworkEdgeIterator& other) const { return !(*this == other); }

        // Set start pointer to the next edge or to NullEdge if no other left.
        void remove() {
            /*if (graph->vertices[v].first == graph->vertices[v].last)
                graph->vertices[v].last = edge->next;*/
            graph->vertices[v].first = edge->next;
        }

        NetworkEdgeIterator& operator++() {
            if (!hasNext()) {
                e = graph->NullEdge;
            } else {
                e = edge->next;
                edge = &graph->edges[e];
            }
            return *this;
        }

        bool isDeleted() { return edge->deleted; }
        void markAsDeleted() { edge->deleted = true; }
        void markReverseAsDeleted() { graph->edges[BackEdge(e)].deleted = true; }
        void unmarkAsDeleted() { edge->deleted = false; }

        FlowType getFlow() { return edge->f; }
        int getCapacity() { return edge->c; }
        vtype getStart() { return edge->u; }
        vtype getFinish() { return edge->v; }
        etype getEdgeNumber() { return e; }
        FlowType getResidualCapacity() { return getCapacity() - getFlow(); }
        void pushFlow(FlowType f) {
            edge->f += f;
            graph->edges[BackEdge(e)].f -= f;
        }
        void pushCapacity(FlowType c) {
            edge->c += c;
            //graph->edges[BackEdge(e)].c -= c;
        }

    private:
        vtype v; // Vertex that iterator belongs to.
        etype e;
        Network* graph;
        Edge* edge;

        etype BackEdge(etype e) {
            return e ^ 1;
        }

        bool isBackward(etype edge) { return (edge & 1) == 1; }

        friend class Network;
    };

    Network() = default;

    Network(uint n, vtype s, vtype t) {
        init(n, s, t);
    }

    void init(uint n_, vtype s_, vtype t_) {
        n = n_;
        s = s_;
        t = t_;
        vertices.resize(n);
        edges.clear();
    }

    vtype getSource() { return s; }
    vtype getTarget() { return t; }
    uint getVertexCount() { return n; }
    void setSource(vtype s_) { s = s_; }
    void setTarget(vtype t_) { t = t_; }
    void insertUndirectedEdge(vtype u, vtype v, FlowType c) {
        insertEdgeLocal(u, v, c, 0);
        insertEdgeLocal(v, u, c, 0);
    }
    void insertDirectedEdge(vtype u, vtype v, FlowType c) {
        insertEdgeLocal(u, v, c, 0);
        insertEdgeLocal(v, u, 0, 0);
    }
    void insertRemainEdge(vtype u, vtype v, FlowType c) {
        insertEdgeLocal(u, v, c, 0);
        insertEdgeLocal(v, u, -c, 0);
    }

    void resetDeleted() {
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
    uint n;
    vtype s, t;
    std::vector<Edge> edges;
    std::vector<Vertex> vertices; // First and last (according to time of addition) edges of vertex.

    struct Edge {
        bool deleted = false;
        vtype u, v;
        FlowType c, f;
        etype prev = NullEdge, next = NullEdge;
        Edge(vtype u, vtype v, FlowType c, FlowType f)
                : u(u), v(v), c(c), f(f) {}
    };

    struct Vertex {
        etype first = NullEdge;
        etype last = NullEdge;
        etype backup_first = NullEdge;
    };

    void insertEdgeLocal(vtype u, vtype v, FlowType c, FlowType f) {
        edges.push_back(Edge(u, v, c, f));
        // Set first if necessary.
        if (vertices[u].first == NullEdge) {
            vertices[u].backup_first = vertices[u].first = edges.size() - 1;
        } else {
            etype tmp = vertices[u].last;
            // Set previous and next.
            edges[tmp].next = edges.size() - 1;
            edges.back().prev = tmp;
        }
        // Set last.
        vertices[u].last = edges.size() - 1;
    }


};