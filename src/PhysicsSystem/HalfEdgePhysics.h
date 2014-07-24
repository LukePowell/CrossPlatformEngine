#pragma once
#include "GraphicsSystem/HalfEdgeMesh.h"
#include "Maths/MathLib.h"
#include "Transform.h"

#include <list>
#include <vector>

namespace ElysiumEngine
{
	struct EdgeToSplit
	{
		float t;
		HalfEdge *edge;
        Vec4 point;
	};

	std::list<EdgeToSplit> calculateHalfEdgesToSplit(Vec4 &normal, float offset, HalfEdgeMesh *mesh, Matrix &translate, std::vector<EdgeToSplit> &duplicates);
    
    void splitHalfEdgeMesh(Vec4 &normal, float offset, HalfEdgeMesh *mesh, Transform &trans);
}