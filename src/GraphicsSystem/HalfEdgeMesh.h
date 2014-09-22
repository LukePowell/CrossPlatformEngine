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
        
        HalfEdge *next = nullptr;
        HalfEdge *previous = nullptr;
        HalfEdge *opposite;
        
		HalfEdgeFace *face = nullptr;
        
        void *userData;
        
        unsigned int marker = 0;
        
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
		HalfEdge *edge = nullptr;
        int indices[3];
		unsigned int marker = 0;
		Vec4 normal;
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