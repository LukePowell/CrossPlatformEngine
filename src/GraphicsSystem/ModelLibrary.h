#pragma once
#include "Mesh.h"
#include "HalfEdgeMesh.h"
#include <string>
#include <map>

namespace ElysiumEngine
{
	class Strip;

	class ModelLibrary
	{
	public:
		Mesh *loadModel(std::string name, bool isStatic);
		HalfEdgeMesh *loadHalfEdgeMesh(std::string mame);//Only used on dynamic objects
        Mesh *meshFromHalfEdge(HalfEdgeMesh *halfEdgeMesh);
		std::vector<Strip> stripeMesh(Mesh *mesh, HalfEdgeMesh *halfEdge);
	private:
		std::map<std::string, Mesh *> meshes;
		std::map<std::string, HalfEdgeMesh *> halfEdgeMeshes;
	};
}