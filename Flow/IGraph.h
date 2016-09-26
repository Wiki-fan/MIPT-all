#pragma once
#include "../stdafx.h"

//template<template<typename> typename template<typename> typename AT>
template<typename etype, typename vtype, typename VST, typename EST>
class IGraph {
    // Forward declaration.
    struct Vertex;
    struct Edge;
public:
    IGraph() = default;
    virtual ~IGraph() = default;

    //Граф с size вершинами.
    virtual IGraph(size_t size) = 0;

    // Добавить ребро u -> v.
    virtual void insertEdge(vtype u, vtype v, EST weight) = 0;

    /*// Добавить вершину со значением val. Возвращает номер добавленной вершины.
    size_t AddVertex( VT val )
    {
        vertices.push_back( Vertex( val ));
        return vertices.size()-1;
    }*/

    /*// Получить контейнер рёбер, входящих в вершину.
    const AT<Edge>& GetIncomingEdges( size_t vNum ) const
    {
        return vertices[vNum].in;
    }

    // Получить контейнер рёбер, выходящих из вершины.
    const AT<Edge>& GetOutcomingEdges( size_t vNum ) const
    {
        return vertices[vNum].out;
    }*/

    /*// Получить значение вершины с номером vNum.
    VST GetVertexVal( size_t vNum )
    {
        return vertices[vNum].val;
    }*/

    // Получить количество вершин.
    virtual vtype getVerticesCount() = 0;
    virtual etype getEdgesCount() = 0;

protected:
    struct Edge {
        EST val;
        virtual vtype getV() = 0;
        virtual vtype getU() = 0;
        virtual vtype getStored() { return val; }
    };

    struct Vertex {
        VST val;
        Vertex() : val(0) {}
        explicit Vertex(int _val) : val(_val) {}
        ~Vertex() {}
        virtual vtype getStored() = 0;
    };

    class IGraphEdgeIterator : public std::iterator<std::forward_iterator_tag, Edge> {
    public:
        virtual IGraphEdgeIterator(IGraph& IGraph_, vtype v_, bool first) = 0;

        virtual bool hasNext() = 0;

        virtual bool operator==(const IGraphEdgeIterator& other) const = 0;
        virtual bool operator!=(const IGraphEdgeIterator& other) const = 0;

        virtual IGraphEdgeIterator& operator++() = 0;

    private:
        friend class IGraph;
    };
};

