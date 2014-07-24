#include "ISystem.h"
#include "Hashing.h"

ElysiumEngine::ISystem::ISystem(std::string name)
{
	this->name = name;
    this->id = ElysiumEngine::hash(name.c_str());
}

int ElysiumEngine::ISystem::getID()
{
	return this->id;
}