#include "HalfEdgeMesh.h"


ElysiumEngine::HalfEdgeVertex *ElysiumEngine::HalfEdge::getSelf()
{
    HalfEdge *iter = this;
    HalfEdge *end = this;
    do {
        iter = iter->next;
    } while (iter->next != end);
    return iter->endPt;
}

ElysiumEngine::HalfEdgeVertex::HalfEdgeVertex(const HalfEdgeVertex &rhs)
{
    edge = rhs.edge;
    index = rhs.index;
    vertex = rhs.vertex;
    marker = rhs.marker;
}

int ElysiumEngine::HalfEdge::getCount()
{
    int count = 1;
    HalfEdge *iter = this;
    HalfEdge *end = this;
    do {
        ++count;
        iter = iter->next;
    } while (iter->next != end);
    return count;
}