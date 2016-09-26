#pragma once
#include "../stdafx.h"
#include "../Other/Smth.h"
#include "IGraph.h"

//template<template<typename> typename template<typename> typename AT>
template<typename vtype, typename etype, typename VST, typename EST>
class Graph : public IGraph<vtype, etype, VST, EST> {

public:
    Graph() = default;
    ~Graph() = default;

    //Граф с size вершинами.
    Graph(size_t size) {
        vertices.resize(size);
    }

    // Добавить ребро u -> v.
    void insertDirectedEdge(size_t u, size_t v, EST data) {
        insertEdgeLocal(u, v, data);
    }

    void insertUndirectedEdge(size_t u, size_t v, EST data) {
        insertEdgeLocal(u, v, data);
        insertEdgeLocal(v, u, data);
    }

    // Получить значение вершины с номером vNum.
    VST& GetVertexVal(vtype vNum) {
        return vertices[vNum].val;
    }

    // Получить количество вершин.
    size_t GetSize() const { return vertices.size(); }

    GraphEdgeIterator begin(vtype v) { return GraphEdgeIterator(*this, v, true); }
    GraphEdgeIterator end() { return GraphEdgeIterator(*this, 0, false); }

private:
    struct GraphEdge : public IGraph::Edge {
        vtype u, v;
        etype next = NullEdge;
        GraphEdge(vtype u, vtype v, EST data)
                : u(u), v(v), val(data) {}
    };

    static const vtype NullEdge = -1;
    std::vector<GraphEdge> edges;
    std::vector<Vertex> vertices;

    void insertEdgeLocal(vtype u, vtype v, EST data) {
        edges.push_back(GraphEdge(u, v, data));
        // Set first if necessary.
        if (vertices[u].first == NullEdge) {
            vertices[u].backup_first = vertices[u].first = edges.size() - 1;
        } else {
            etype tmp = vertices[u].last;
            // Set previous and next.
            edges[tmp].next = edges.size() - 1;
        }
        // Set last.
        vertices[u].last = edges.size() - 1;
    }

    class GraphEdgeIterator : public std::iterator<std::forward_iterator_tag, Edge> {
    public:
        GraphEdgeIterator(Graph& graph_, vtype v_, bool first) {
            graph = &graph_;
            if (first) {
                v = v_;
                e = graph->vertices[v].first;
                edge = &graph->edges[e];
            } else {
                e = graph->NullEdge;
            }
        }

        bool hasNext() { return edge->next != graph->NullEdge; }
        //bool hasPrev() { return edge->prev != graph->NullEdge; }

        bool operator==(const GraphEdgeIterator& other) const { return graph == other.graph && e == other.e; }
        bool operator!=(const GraphEdgeIterator& other) const { return !(*this == other); }

        GraphEdgeIterator& operator++() {
            if (!hasNext()) {
                e = graph->NullEdge;
            } else {
                e = edge->next;
                edge = &graph->edges[e];
            }
            return *this;
        }

    private:
        vtype v; // Vertex that iterator belongs to.
        etype e;
        Graph* graph;
        GraphEdge* edge;

        friend class Network;
    };
};


