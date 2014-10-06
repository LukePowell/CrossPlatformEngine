#pragma once
#include <map>

namespace ElysiumEngine
{
	class Material;

	class MaterialLibrary
	{
	public:
		Material *getMaterial(std::string mat);
		void loadMaterialsFromFile(std::string filename);
	private:
		std::map<std::string, Material *> materials;
	};
}