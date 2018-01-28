#pragma once
#include <vector>
#include <limits>
#include <cstddef>

using uint = unsigned int;

template<typename vtype = int, typename etype = int, typename FlowType = int>
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

        inline bool hasNext() { return edge->next != graph->NullEdge; }

        inline bool operator==(const NetworkEdgeIterator& other) const { return graph == other.graph && e == other.e; }
        inline bool operator!=(const NetworkEdgeIterator& other) const { return !(*this == other); }

        // Set start pointer to the next Edge or to NullEdge if no other left.
        inline void shiftBeginToNext() {
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
        inline void setCapacity(FlowType c) { edge->capacity = c; }

        bool isBackward() { return checkBackward(e); }
    private:
        vtype v; // Vertex that iterator belongs to.
        etype e;
        Network* graph;
        Edge* edge;

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
            e.flow = 0;
        }
    }

    // Returns iterator of vertex v edges.
    NetworkEdgeIterator begin(vtype v) { return NetworkEdgeIterator(*this, v, true); }
    NetworkEdgeIterator end() { return NetworkEdgeIterator(*this, 0, false); }
    NetworkEdgeIterator getEdgeByNum(etype num) { return num != -1 ? NetworkEdgeIterator(*this, num) : end(); }
private:
    static const etype NullEdge = std::numeric_limits<etype>::max();
    vtype s, t;
    std::vector<Edge> edges;
    std::vector<Vertex> vertices; // First and last (according to time of addition) edges of vertex.

    struct Edge {
        bool deleted = false;
        vtype from, to;
        FlowType capacity, flow;
        etype next = NullEdge;
        Edge(vtype u, vtype v, FlowType c, FlowType f)
                : from(u), to(v), capacity(c), flow(f) {}
    };

    struct Vertex {
        etype first = NullEdge;
        etype last = NullEdge;
        etype backup_first = NullEdge;
    };

    void insertEdgeLocal(vtype from, vtype to, FlowType capacity, FlowType flow) {
        edges.push_back(Edge(from, to, capacity, flow));
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