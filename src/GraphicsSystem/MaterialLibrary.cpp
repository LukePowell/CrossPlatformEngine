#include "MaterialLibrary.h"
#include "GraphicsSystem.h"
#include <cctype>
#include <cstdlib>
#include "Mesh.h"

void ElysiumEngine::MaterialLibrary::loadMaterialsFromFile(std::string file)
{
	FILE *input = fopen(file.c_str(), "rt");
	if (!input)
	{
		std::cout << "Could not open material library " << file << std::endl;
		return;
	}

	std::vector<std::string> tokens;
	char buffer[1000];

	Material *current;
	while (!feof(input))
	{
		tokens.clear();

		if (!fgets(buffer, 1000, input))
		{
			break;
		}

		customTokenize(buffer, tokens);

		if (tokens.empty())
			continue;

		if (tokens[0] == "newmtl")
		{
			current = new Material();
			materials[tokens[1]] = current;
		}
		else if (tokens[0] == "Ka")
		{
			current->Ka = Vec4(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()));
		}
		else if (tokens[0] == "Kd")
		{
			current->Kd = Vec4(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()));
		}
		else if (tokens[0] == "Ks")
		{
			current->Ks = Vec4(std::atof(tokens[1].c_str()), std::atof(tokens[2].c_str()), std::atof(tokens[3].c_str()));
		}
		else if (tokens[0] == "Ns")
		{
			current->Ns = std::atof(tokens[1].c_str());
		}
	}
}

ElysiumEngine::Material *ElysiumEngine::MaterialLibrary::getMaterial(std::string tag)
{
	return materials[tag];
}