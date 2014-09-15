#pragma once
#include <vector>
#include "Maths/MathLib.h"

namespace ElysiumEngine
{
    struct HalfEdgeVertex;
    struct HalfEdgeFace;
    
    struct HalfEdge
    {
        //HalfEdge(HalfEdge *rhs, HalfEdgeVertex *endPt){
        HalfEdge(){userData=nullptr;opposite = nullptr;}
        HalfEdgeVertex *endPt;
        
        HalfEdge *next;
        HalfEdge *previous;
        HalfEdge *opposite;
        
        HalfEdgeFace *face;
        
        void *userData;
        
        unsigned int marker;
        
        HalfEdgeVertex *getSelf();
        int getCount();
        bool isBoundary();
    };
    
    struct HalfEdgeVertex
    {
        HalfEdgeVertex(const HalfEdgeVertex &rhs);
        
        HalfEdgeVertex(HalfEdge *edge, int index, unsigned int marker) : edge(edge), index(index), marker(marker) {}
    
        HalfEdgeVertex(HalfEdge *edge,Vec4 pos, unsigned int marker) : edge(edge), index(0),vertex(pos), marker(marker) {}
        
        HalfEdge *edge;
        int index;
        Vec4 vertex;
        
        unsigned int marker;
    };
    
    struct HalfEdgeFace
    {
        HalfEdge *edge;
        int indices[3];
        unsigned int marker;
    };
    
    class HalfEdgeMesh
    {
    public:
        HalfEdgeMesh(){}
        HalfEdgeMesh(const HalfEdgeMesh &base){}
        
        std::vector<HalfEdgeFace *> faces;
        std::vector<HalfEdge *> halfEdges;
        std::vector<HalfEdgeVertex *> halfEdgeVertices;
    };
}